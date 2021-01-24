// Stranded Engine Renderer
// XMesh 模块定义

#include "XMesh.h"


CXModel::CXModel():
	m_root(nullptr),
	m_animControl(nullptr),
	m_boneMatrices(nullptr),
	m_maxBones(0),
	m_currentContainer(nullptr),
	m_center(D3DXVECTOR3(0, 0, 0)),
	m_radius(0),
	m_currentAni(0),
	m_numAnimations(0),
	m_currentTrack(0),
	m_speed(1.0f),
	m_currentTime(0),
	m_transition(0.25f),
	m_device(nullptr)
{
}

void CXModel::SetDevice(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
}

bool CXModel::LoadXFile(const char *fileName)
{
   if(!m_device) return false;

   // 从X文件读取模型第一帧至 m_root (D3DXFRAME 结构)  <- stD3DFrameEx
   // 注：D3DXFRAME 结构含有 一个包含mesh对象的 D3DXMESHCONTAINER 结构  <- stD3DContainerEx
   // 并读取动画控制器至 m_animControl (ID3DXAnimationController 指针)
   CD3DAllocate alh;
   if(FAILED(D3DXLoadMeshHierarchyFromX(fileName, D3DXMESH_MANAGED,
      m_device, &alh, nullptr, &m_root, &m_animControl))) return false;

   // 记录m_animControl中动画集合的个数到 m_numAnimations。
   if(m_animControl)
      m_numAnimations = m_animControl->GetMaxNumAnimationSets();

   // 设置骨骼，计算外接球
   if(m_root)
      {
         SetupMatrices((stD3DFrameEx*)m_root, nullptr);

         m_boneMatrices = new D3DXMATRIX[m_maxBones];
         ZeroMemory(m_boneMatrices, sizeof(D3DXMATRIX)*m_maxBones);

         D3DXFrameCalculateBoundingSphere(m_root, &m_center, &m_radius);
      }

   // 设置初始动画
   SetAnimation(0);

   return true;
}

void CXModel::Update(float time, const D3DXMATRIX* matWorld)
{
   time /= m_speed;
   m_currentTime += time;

   if(m_animControl) m_animControl->AdvanceTime(time, nullptr);

   UpdateMatrices(m_root, matWorld);

   stD3DContainerEx* pMesh = m_currentContainer;

   if(pMesh && pMesh->pSkinInfo)
      {
         unsigned int numBones = pMesh->pSkinInfo->GetNumBones();

         for(unsigned int i = 0; i < numBones; i++)
            D3DXMatrixMultiply(&m_boneMatrices[i], pMesh->pSkinInfo->GetBoneOffsetMatrix(i), pMesh->boneMatrices[i]);

         void *srcPtr;
         pMesh->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&srcPtr);

         void *destPtr;
         pMesh->originalMesh->LockVertexBuffer(0, (void**)&destPtr);

         pMesh->pSkinInfo->UpdateSkinnedMesh(m_boneMatrices, NULL, srcPtr, destPtr);

         pMesh->originalMesh->UnlockVertexBuffer();
         pMesh->MeshData.pMesh->UnlockVertexBuffer();
      }
}

void CXModel::Render()
{
	if(m_root)
	   DrawFrame(m_root);
}

void CXModel::ChangeAnimationSpeed(float amt)
{
	m_speed += amt;
}

void CXModel::GetBoundingSphere(D3DXVECTOR3* center, float* radius)
{
	if (center) *center = m_center;
	if (radius) *radius = m_radius;
}

void CXModel::Release()
{
	if (m_root)
	{
		CD3DAllocate alh;
		D3DXFrameDestroy(m_root, &alh);
		m_root = nullptr;
	}

	if (m_animControl) m_animControl->Release();
	m_animControl = nullptr;

	if (m_boneMatrices) delete[] m_boneMatrices;
	m_boneMatrices = nullptr;
}

/* Utilities */

void CXModel::SetupMatrices(stD3DFrameEx *inFrame, LPD3DXMATRIX parentMatrix)
{
	if (!m_device) return;

	stD3DContainerEx* containerEx = (stD3DContainerEx*)inFrame->pMeshContainer;

	if (containerEx)
	{
		if (!m_currentContainer)
			m_currentContainer = containerEx;

		if (containerEx->pSkinInfo && containerEx->MeshData.pMesh)
		{
			D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];

			if (FAILED(containerEx->MeshData.pMesh->GetDeclaration(Declaration)))
				return;

			containerEx->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED,
				Declaration, m_device, &containerEx->originalMesh);

			m_maxBones = max(m_maxBones, containerEx->pSkinInfo->GetNumBones());

			for (unsigned int i = 0; i < containerEx->pSkinInfo->GetNumBones(); i++)
			{
				stD3DFrameEx *temp = (stD3DFrameEx*)D3DXFrameFind(m_root,
					containerEx->pSkinInfo->GetBoneName(i));

				containerEx->boneMatrices[i] = &temp->finalMatrix;
			}

		}
	}

	if (inFrame->pFrameSibling)
		SetupMatrices((stD3DFrameEx*)inFrame->pFrameSibling, parentMatrix);

	if (inFrame->pFrameFirstChild)
		SetupMatrices((stD3DFrameEx*)inFrame->pFrameFirstChild, &inFrame->finalMatrix);
}

void CXModel::UpdateMatrices(const D3DXFRAME *inFrame, const D3DXMATRIX *parentMatrix)
{
   stD3DFrameEx *frame = (stD3DFrameEx*)inFrame;

   if(parentMatrix != nullptr)
      D3DXMatrixMultiply(&frame->finalMatrix, &frame->TransformationMatrix, parentMatrix);
   else
      frame->finalMatrix = frame->TransformationMatrix;

   if(frame->pFrameSibling != nullptr)
      UpdateMatrices(frame->pFrameSibling, parentMatrix);

   if(frame->pFrameFirstChild != nullptr)
      UpdateMatrices(frame->pFrameFirstChild, &frame->finalMatrix);
}

void CXModel::DrawFrame(LPD3DXFRAME frame)
{
   if(!m_device) return;

   LPD3DXMESHCONTAINER meshContainer = frame->pMeshContainer;

   while(meshContainer)
      {
         stD3DFrameEx *frameEx = (stD3DFrameEx*)frame;		
         m_device->SetTransform(D3DTS_WORLD, &frameEx->finalMatrix);

         stD3DContainerEx *containerEx = (stD3DContainerEx*)meshContainer;

         for(unsigned int i = 0; i < containerEx->NumMaterials; i++)
            {
               m_device->SetMaterial(&containerEx->mat[i]);
               m_device->SetTexture(0, containerEx->textures[i]);

               LPD3DXMESH pDrawMesh;
               
               if(containerEx->pSkinInfo)
                  pDrawMesh = containerEx->originalMesh;
               else
                  pDrawMesh = containerEx->MeshData.pMesh;

               pDrawMesh->DrawSubset(i);
            }

         meshContainer = meshContainer->pNextMeshContainer;
      }

   if(frame->pFrameSibling != NULL)
      DrawFrame(frame->pFrameSibling);

   if(frame->pFrameFirstChild != NULL)
      DrawFrame(frame->pFrameFirstChild);
}

void CXModel::SetAnimation(unsigned int index)
{
	if (index >= m_numAnimations || index == m_currentAni)
		return;

	m_currentAni = index;

	LPD3DXANIMATIONSET set;
	m_animControl->GetAnimationSet(m_currentAni, &set);

	unsigned long nextTrack = (m_currentTrack == 0 ? 1 : 0);

	// Set next track.
	m_animControl->SetTrackAnimationSet(nextTrack, set);
	set->Release();

	// Take way all tracks.
	m_animControl->UnkeyAllTrackEvents(m_currentTrack);
	m_animControl->UnkeyAllTrackEvents(nextTrack);

	// Key current track.
	m_animControl->KeyTrackEnable(m_currentTrack, FALSE, m_currentTime + m_transition);
	m_animControl->KeyTrackSpeed(m_currentTrack, 0.0f, m_currentTime, m_transition, D3DXTRANSITION_LINEAR);
	m_animControl->KeyTrackWeight(m_currentTrack, 0.0f, m_currentTime, m_transition, D3DXTRANSITION_LINEAR);

	// Key next track.
	m_animControl->SetTrackEnable(nextTrack, TRUE);
	m_animControl->KeyTrackSpeed(nextTrack, 1.0f, m_currentTime, m_transition, D3DXTRANSITION_LINEAR);
	m_animControl->KeyTrackWeight(nextTrack, 1.0f, m_currentTime, m_transition, D3DXTRANSITION_LINEAR);

	m_currentTrack = nextTrack;
}