// Stranded Engine Renderer XMesh
// XStructures 模块声明

// 注意：该文件不作为对外接口，不暴露至引擎外部。
// 所有方法应该被XMesh封装。

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

// 注：d3dFrame 结构中含有一个 mesh container 指针。
struct stD3DFrameEx: public D3DXFRAME
{
   D3DXMATRIX finalMatrix;
};

// 注：mesh container 是一个mesh容器。
struct stD3DContainerEx: public D3DXMESHCONTAINER
{
   LPDIRECT3DTEXTURE9 *textures;
   D3DMATERIAL9 *mat;
   LPD3DXMESH originalMesh;
   D3DXMATRIX **boneMatrices;
};

// 注：该类专用来创建和销毁mesh。（写法参见COM接口）
class CD3DAllocate : public ID3DXAllocateHierarchy
{
   public:	
      STDMETHOD(CreateFrame)(THIS_ LPCSTR name, LPD3DXFRAME *outFrame);

      STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR name, CONST D3DXMESHDATA *mesh, 
         CONST D3DXMATERIAL *mats, CONST D3DXEFFECTINSTANCE *effects,
         DWORD numMats, CONST DWORD *indices, LPD3DXSKININFO skin, 
         LPD3DXMESHCONTAINER *outContainer);

      STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME inFrame);
      STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER container);
};