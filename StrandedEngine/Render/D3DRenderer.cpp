// Stranded Engine
// D3D��Ⱦ�� �ӿڶ���

#include "D3DRenderer.h"

#include <stdio.h>
#include <stdarg.h>



D3DRenderer::D3DRenderer(): 
	m_screenWidth(0), 
	m_screenHeight(0), 
	m_fullScreen(false),
	m_Device(nullptr), 
	m_Color(0), 
	m_numStaticGUIMeshes(0),
	m_numXModels(0), 
	m_totalGUIs(0), 
	m_numTextures(0), 
	m_numFonts(0)
{
}

D3DRenderer::~D3DRenderer() {
	ReleaseGUIs();
	ReleaseXModels();
	ReleaseTextures();
	ReleaseFonts();
	/* �ͷ������豸 */
	if (m_Device) {
		m_Device->Release();
		m_Device = nullptr;
	}
}

void D3DRenderer::bump(const char* format, ...)
{
	char buffer[1024]; // ������Ϣ��Ӧ����1024B��
	va_list arg;
	va_start(arg, format);
	vsprintf(buffer, format, arg);
	va_end(arg);
	MessageBoxA(0, buffer, "message", MB_OK);

}

/* ������� */

bool D3DRenderer::Initialize(int width, int height, HWND mainWindow, bool fullScreen) {
	/* ��ʼ����Ա */
	this->m_screenWidth			= width;
	this->m_screenHeight		= height;
	this->m_fullScreen			= fullScreen;

	/* ����D3D�豸������ȡ��ʾ���ú�Ӳ�����������mode���Խ�����Ӧ���� */
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DDISPLAYMODE mode;
	if (!d3d9 || FAILED(d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
	{
		return false;
	}

	DWORD processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	/*D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if (caps.VertexProcessingCaps != 0)
		processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	else
		processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;*/
	
	/* ��� D3DPRESENT_PARAMETERS �ṹ */
	D3DPRESENT_PARAMETERS Params;
	ZeroMemory(&Params, sizeof(Params));

	if (m_fullScreen)
	{
		Params.Windowed = false;
		Params.FullScreen_RefreshRateInHz = mode.RefreshRate;  //ȫ��ˢ���ʺ���ʾ����һ��
		Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //������ʾƵ��Ϊ��̨ˢ����һ֡ʱ
		Params.BackBufferWidth = width;                        //��̨������
		Params.BackBufferHeight = height;                      //��̨����߶�
	}
	else
	{
		Params.Windowed = true;
		Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //���÷�ȫ����������ʾ
	}
	Params.hDeviceWindow = mainWindow;          //�豸���ھ������Ϸ�����ڣ�
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;  //ǰ̨���̨���潻������
	Params.BackBufferFormat = mode.Format;      //��̨�����ʽ������ʾ����һ��
	Params.BackBufferCount = 1;                 //��̨�������Ϊ1
	Params.EnableAutoDepthStencil = true;       //�Զ���Ⱥ�ģ�建��
	Params.AutoDepthStencilFormat = D3DFMT_D16; //�����ʽ

	//���ز�����Ԥ����ֵ���粻֧������Ϊ��
	if (D3D_OK == d3d9->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT, 
			D3DDEVTYPE_HAL, 
			mode.Format, 
			!fullScreen, 
			SAMPLE_TYPE, 
			0)) {
		Params.MultiSampleType = SAMPLE_TYPE;
	} else {
		Params.MultiSampleType = D3DMULTISAMPLE_NONE;
	}	

	/* ���������豸 */
	LRESULT hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		mainWindow, 
		processing, 
		&Params, 
		&m_Device);

	d3d9->Release();


	if (FAILED(hr) || m_Device == nullptr)
	{
		return false;
	}

	/* ��ʼ������ */
	SetupScene();

	return true;
}

void D3DRenderer::StartRendering() {
	static const DWORD clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;

	//ע��Ĭ��clearColor, clearDepth, clearStencil �ֱ�Ϊ 1,1,0
	//if (clearColor)   clearFlags |= D3DCLEAR_TARGET;
	//if (clearDepth)   clearFlags |= D3DCLEAR_ZBUFFER;
	//if (clearStencil) clearFlags |= D3DCLEAR_STENCIL;

	/* ���� */
	m_Device->Clear(0, 0, clearFlags, m_Color, 1.0f, 0);

	/* ��ʼ���� */
	m_Device->BeginScene();
}

void D3DRenderer::EndRendering() {
	/* �������� */
	m_Device->EndScene();
	m_Device->Present(0, 0, 0, 0);
}

void D3DRenderer::Release() {
	delete this;
}


void D3DRenderer::SetupScene()
{
	if (!m_Device) {
		return;
	}

	/* ���㲢����ͶӰ���� */
	D3DXMatrixPerspectiveFovLH(
		&m_proj,
		D3DX_PI / 4,
		static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight),
		0.1f,
		1000.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &m_proj);

	/* ����������ɫ */
	m_Color = D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);

	/* �رչ��� */
	m_Device->SetRenderState(D3DRS_LIGHTING, false);
	//m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

/* ������ͼ */

bool D3DRenderer::AddTexture2D(const char* fileName, int* texId) {

	if (!texId) {
		return false;
	}
	
	// ������ͼ���
	int index = m_numTextures;

	m_textureList.push_back(D3DTexture());

	/* ����������ļ��� */
	m_textureList[index].fileName = fileName;

	/* ���ļ���������
	 * ʹ����չ�����ɻ�ȡ��ͼ�����糤�� */
	D3DXIMAGE_INFO info;
	if (FAILED(D3DXCreateTextureFromFileEx(
		m_Device,
		fileName,
		0,
		0,
		0,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR(0xFF000000),
		&info,
		0,
		&m_textureList[index].image))) {
		return false;
	}

	m_textureList[index].Width = info.Width;
	m_textureList[index].Height = info.Height;

	*texId = m_numTextures;
	m_numTextures++;

	return true;
}

void D3DRenderer::ApplyTexture(int stage, int texId)
{
	if (stage < 0 || texId < 0 || texId >= m_numTextures)
	{
		m_Device->SetTexture(0, 0);
		return;
	}

	m_Device->SetTexture(stage, m_textureList[texId].image);
}

void D3DRenderer::ReleaseTextures()
{
	/* �ͷ���ͼ�б� */
	for (int i = 0; i < m_numTextures; i++)
	{
		if (m_textureList[i].image)
		{
			m_textureList[i].image->Release();
			m_textureList[i].image = nullptr;
		}
	}

	this->m_textureList.clear();
	m_numTextures = 0;
}

/* GUI ��� */

bool D3DRenderer::CreateGUIObject(int& guiID)
{
	m_guiList.push_back(GUIObject());

	guiID = m_totalGUIs;
	m_totalGUIs++;
	return true;
}

bool D3DRenderer::CreateFontObject(int& fontID, const char* fontName, int weight, bool italic, int size)
{
	ID3DXFont* newfont;

	/* �������� */
	if (FAILED(D3DXCreateFont(m_Device, size, 0, weight, 1, italic, 0, 0, 0, 0,
		fontName, &newfont))) {
		return false;
	}

	m_fonts.push_back(newfont);
	fontID = m_numFonts;
	m_numFonts++;

	return true;
}

bool D3DRenderer::AddGUIBackdrop(int guiID, const char * fileName)
{
	int texID = -1, staticID = -1;

	/* �����ͼ�õ���ͼid */
	if (!AddTexture2D(fileName, &texID)) return false;

	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);
	
	/* ��Ӷ��㻺��õ����㻺��id */
	GUIVertex obj[] = {
		{(float)m_screenWidth,0,0,1,white,1,0},
		{(float)m_screenWidth,(float)m_screenHeight,0,1,white,1,1},
		{0,0,0,1,white,0,0},
		{0,(float)m_screenHeight,0,1,white,0,1},
	};

	if (!AddStaticMesh_GUI(obj, &staticID))
	{
		return false;
	}

	return m_guiList[guiID].AddBackdrop(texID, staticID);
}

bool D3DRenderer::AddGUIStaticText(int guiID, int id, const wchar_t * text, int x, int y, unsigned long color, int fontID)
{
	return m_guiList[guiID].AddStaticText(id, text, x, y, color, fontID);
}

bool D3DRenderer::AddGUIDynamicText(int guiID, int id, const wchar_t * text, int x, int y, unsigned long color, int fontID)
{
	return m_guiList[guiID].AddDynamicText(id, text, x, y, color, fontID);
}

bool D3DRenderer::AddGUIButton(int guiID, int id, int x, int y, const char * upTex, const char * overTex, const char * downTex)
{
	int upID = -1, overID = -1, downID = -1, staticID = -1;

	if (!AddTexture2D(upTex, &upID)) return false;
	if (!AddTexture2D(overTex, &overID)) return false;
	if (!AddTexture2D(downTex, &downID)) return false;

	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);
	int w = m_textureList[upID].Width;
	int h = m_textureList[upID].Height;

	GUIVertex obj[] = {
		{(float)(w + x), (float)(0 + y), 0, 1, white, 1, 0},
		{(float)(w + x), (float)(h + y), 0, 1, white, 1, 1},
		{(float)(0 + x), (float)(0 + y), 0, 1, white, 0, 0},
		{(float)(0 + x), (float)(h + y), 0, 1, white, 0, 1},
	};

	if (!AddStaticMesh_GUI(obj, &staticID)) return false;

	return m_guiList[guiID].AddButton(id, x, y, w, h, upID, overID, downID, staticID);
}

void D3DRenderer::RenderGUI(int guiID, bool LMBDown, int mouseX, int mouseY, void(*funcPtr)(int id, int state))
{
	GUIObject* gui = &m_guiList[guiID];

	/* ��Ⱦ���� */
	GUIControl* pBackdrop = gui->GetBackDrop();

	if (pBackdrop)
	{
		ApplyTexture(0, pBackdrop->m_backdropTex);
		RenderStaticMeshes_GUI(pBackdrop->m_listID);
		ApplyTexture(0, -1);
	}

	/* ��Ⱦ��ť���ı� */
	for (int i = 0; i < gui->GetTotalControls(); i++)
	{
		int status = BUTTON_UP;

		/* ��ȡ��ǰ��GUI�ؼ�ָ�� */
		GUIControl* pCtrl = gui->GetControl(i);
		if (!pCtrl)
		{
			continue;
		}

		switch (pCtrl->m_type)
		{
		case GUI_STATICTEXT:
			/* ��ʾ��̬�ı� */
			DisplayText(pCtrl->m_listID, (int)pCtrl->m_xPos, (int)pCtrl->m_yPos,
				pCtrl->m_color, pCtrl->m_text.c_str());
			break;
		case GUI_DYNAMICTEXT:
			/* ��ʾ��̬�ı� */
			DisplayText(pCtrl->m_listID, (int)pCtrl->m_xPos, (int)pCtrl->m_yPos,
				pCtrl->m_color, pCtrl->m_p_text); //D3DCOLOR_XRGB(255, 255, 255)
			break;
		case GUI_BUTTON:
			/* ����Alpha�ں� */
			m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			/* ������״̬ */
			if (mouseX > pCtrl->m_xPos && mouseX < pCtrl->m_xPos + pCtrl->m_width &&
				mouseY > pCtrl->m_yPos && mouseY < pCtrl->m_yPos + pCtrl->m_height)
			{
				if (LMBDown) status = BUTTON_DOWN;
				else status = BUTTON_OVER;
			}

			/* ���ð�ť��ͼ */
			if (status == BUTTON_UP) ApplyTexture(0, pCtrl->m_upTex);
			if (status == BUTTON_OVER) ApplyTexture(0, pCtrl->m_overTex);
			if (status == BUTTON_DOWN) ApplyTexture(0, pCtrl->m_downTex);

			/* ���ư�ť��ͼ */
			RenderStaticMeshes_GUI(pCtrl->m_listID);

			/* �ر�alpha�ں�����ͼ */
			m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			ApplyTexture(0, -1);

			break;
		}
		if (funcPtr)
		{
			funcPtr(pCtrl->m_id, status);
		}
	}
}

void D3DRenderer::DisplayText(int fontID, int x, int y, unsigned long color, const wchar_t * text)
{
	RECT textPos = { x,y,m_screenWidth,m_screenHeight };

	m_fonts[fontID]->DrawTextW(0, text, -1, &textPos, DT_SINGLELINE, color);
}

void D3DRenderer::ReleaseGUIs()
{
	ReleaseStaticMeshes_GUI();

	/* ���GUISystem�б� */ 
	m_guiList.clear();
	m_totalGUIs = 0;
}

void D3DRenderer::ReleaseFonts()
{
	/* ɾ�����������е����� */
	for (int i = 0; i < m_numFonts; i++)
	{
		if (m_fonts[i])
		{
			m_fonts[i]->Release();
			m_fonts[i] = nullptr;
		}
	}

	/* �ͷ������б� */
	m_fonts.clear();
	m_numFonts = 0;
}


bool D3DRenderer::AddStaticMesh_GUI(void* vertexData, int* staticId)
{
	/* ��ȡ��ǰ����ĸ��� */
	int index = m_numStaticGUIMeshes;

	m_staticMeshGUIList.push_back(StaticMeshGUI());

	/* ����µ� StaticMeshGUI �ṹ���еĻ������� */
	m_staticMeshGUIList[index].numVerts = 4;
	m_staticMeshGUIList[index].FVF = D3DFVF_GUI;
	m_staticMeshGUIList[index].vertexSize = sizeof(GUIVertex);

	/* �������㻺�� */
	if (FAILED(m_Device->CreateVertexBuffer(4 * sizeof(GUIVertex), D3DUSAGE_WRITEONLY,
		m_staticMeshGUIList[index].FVF, D3DPOOL_DEFAULT,
		&m_staticMeshGUIList[index].vbPtr, NULL))) {
		return false;
	}

	void* _tempData = nullptr;
	if (FAILED(m_staticMeshGUIList[index].vbPtr->Lock(0, 0, (void**)&_tempData, 0))) {
		return false;
	}

	memcpy(_tempData, vertexData, 4 * sizeof(GUIVertex));

	m_staticMeshGUIList[index].vbPtr->Unlock();

	/* ���ظþ�̬���������ֵ */
	if (staticId) {
		*staticId = m_numStaticGUIMeshes;
	}
	m_numStaticGUIMeshes++;

	return true;
}

bool D3DRenderer::RenderStaticMeshes_GUI(int staticId) {

	/* �����㻺��󶨵������� */
	if (FAILED(m_Device->SetStreamSource(0, m_staticMeshGUIList[staticId].vbPtr,
		0, m_staticMeshGUIList[staticId].vertexSize))) {
		return false;
	}

	/* ���ö����ʽ */
	if (FAILED(m_Device->SetFVF(m_staticMeshGUIList[staticId].FVF))) {
		return false;
	}

	/* �������������� ʹ��DrawPrimitive() */
	if (FAILED(m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,
		m_staticMeshGUIList[staticId].numVerts / 2))) {
		return false;
	}

	return true;
}

void D3DRenderer::ReleaseStaticMeshes_GUI()
{
	/* ����̬���������е����� */
	for (int i = 0; i < m_numStaticGUIMeshes; i++)
	{
		if (m_staticMeshGUIList[i].vbPtr)
		{
			m_staticMeshGUIList[i].vbPtr->Release();
			m_staticMeshGUIList[i].vbPtr = nullptr;
		}
	}

	m_staticMeshGUIList.clear();
	m_numStaticGUIMeshes = 0;

}

/* ģ����� */

bool D3DRenderer::LoadXModel(const char* fileName, int* xModelId)
{
	/* �����¶�̬����Ԫ�� */
	m_xModels.push_back(CXModel());

	m_xModels[m_numXModels].SetDevice(m_Device);
	if (!m_xModels[m_numXModels].LoadXFile(fileName))
		return false;

	// ���ļ��ж�ģ����Ϣ��˳���ȡ������Ϣ
	// ��������D3DXMATERIAL��ʽ���棬ÿ�ݲ��ʶ�����������Ա��D3D�Ĺ��ղ��ʺ���ͼ��
	//��D3D���ղ���ֱ�ӱ����ڸø�ʽ��matD3D��Ա�У�����ͼ���ļ����ĸ�ʽ���棩
	//ID3DXMesh* mesh = 0;
	//ID3DXBuffer* matBuffer = nullptr;
	//DWORD numMats = 0;
	//if (FAILED(D3DXLoadMeshFromX("maps/GoldAwm.x", D3DXMESH_MANAGED, m_Device,
	//	0, &matBuffer, 0, &numMats, &mesh)))
	//	return false;
	////    һ��ģ�Ϳ��԰���������ʣ�Ϊ��ǰģ�͵Ĳ����б�����ڴ�
	//D3DMATERIAL9* matList = new D3DMATERIAL9[numMats];
	//IDirect3DTexture9** texList = new IDirect3DTexture9*[numMats];
	//D3DXMATERIAL *mat = (D3DXMATERIAL*)matBuffer->GetBufferPointer();
	//for (auto i = 0; i < numMats; i++)
	//{
	////	 ��D3DXMATERIAL��������ȡD3D���ղ�����Ϣ
	//	matList[i] = mat[i].MatD3D;
	////	 ��D3DXMATERIAL��������ȡ��ͼ�ļ����������ظ���ͼ
	//	if (FAILED(D3DXCreateTextureFromFile(m_Device, mat[i].pTextureFilename,
	//		&texList[i])))
	//		texList[i] = nullptr;
	//}
	//if (matBuffer) matBuffer->Release();
	//

	if (xModelId) *xModelId = m_numXModels;
	m_numXModels++;

	return true;
}

void D3DRenderer::UpdateXAnimation(int xModelId, float time, const Matrix* mat)
{
	m_xModels[xModelId].Update(time, reinterpret_cast<const D3DXMATRIX*>(&mat->matrix));
}

bool D3DRenderer::RenderXModel(int xModelId)
{
	if (xModelId < 0) return false;

	/* ����D3D�豸 */
	m_Device->SetIndices(nullptr);
	m_Device->SetStreamSource(0, 0, 0, 0);
	m_Device->SetFVF(0);

	m_xModels[xModelId].Render();

	return true;
}

void D3DRenderer::ReleaseXModels()
{
	for (int i = 0; i < m_numXModels; i++)
	{
		m_xModels[i].Release();
	}

	m_xModels.clear();
	m_numXModels = 0;
}

/* ������ײ */

void D3DRenderer::GetBoundingSphere(int xModelId, Vector3* center, float* radius)
{
	D3DXVECTOR3 d3dcenter;
	float r;

	m_xModels[xModelId].GetBoundingSphere(&d3dcenter, &r);

	if (center)
	{
		center->x = d3dcenter.x;
		center->y = d3dcenter.y;
		center->z = d3dcenter.z;
	}

	if (radius)
	{
		*radius = r;
	}
}

/* ����ϵͳ */

void D3DRenderer::EnablePointSprites(float size, float min, float a, float b, float c)
{
	/* ���õ㾫�飬�����õ���ͼ�Ĵ�С */
	m_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_Device->SetRenderState(D3DRS_POINTSIZE, static_cast<DWORD>(size));

	/* ���õ���ͼΪ������״̬����������С��С�����Ų��� */
	m_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_Device->SetRenderState(D3DRS_POINTSIZE_MIN, static_cast<DWORD>(min));
	m_Device->SetRenderState(D3DRS_POINTSCALE_A, static_cast<DWORD>(a));
	m_Device->SetRenderState(D3DRS_POINTSCALE_B, static_cast<DWORD>(b));
	m_Device->SetRenderState(D3DRS_POINTSCALE_C, static_cast<DWORD>(c));
}

void D3DRenderer::DisablePointSprites()
{
	m_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
}

/* ��Ӱ����� */

void D3DRenderer::SetCamera(Vector3 pos, Vector3 view, Vector3 up)
{
	D3DXVECTOR3 cameraPos(pos.x, pos.y, pos.z);
	D3DXVECTOR3 lookAtPos(view.x, view.y, view.z);
	D3DXVECTOR3 upPos(up.x, up.y, up.z);

	D3DXMATRIX mat;
	D3DXMatrixLookAtLH(&mat, &cameraPos, &lookAtPos, &upPos);

	m_Device->SetTransform(D3DTS_VIEW, &mat);
}

void D3DRenderer::SetWorldMatrix(Matrix* mat)
{
	m_Device->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)mat->matrix);
}

void D3DRenderer::UpdateHudPositionFromCamera(Matrix* hud, Camera* camera, float* angleH, float* angleV)
{
	//ԭʼλ�� 
	D3DXMATRIX* mat = reinterpret_cast<D3DXMATRIX*>(hud);
	//ע1��d3dxmatrix�ṹΪ4x4��a44 = 1.d3dvector��a44 = 0.
	//ע2���������ʹ�õ�4ά����ƽ�ơ�

	//D3DXMatrixIdentity(&mat);	
	//EA = A�ʴ��б�����

	static const D3DXVECTOR3 yaxis = { 0.0f, 1.0f, 0.0f };

	// 0. ��������(ģ���Ƿ���)
	D3DXMatrixRotationAxis(mat, &yaxis, D3DX_PI);

	// 1.1 ƽ��(�������ԭ��)
	D3DXMATRIX trans1;
	D3DXMatrixTranslation(&trans1, camera->m_pos.x, camera->m_pos.y, camera->m_pos.z);
	*mat *= trans1;

	D3DXMATRIX mat_0 = *mat; //��¼��ǰ�任��mat_0����׼����任��
							 //ע��mat_0ִ�еı任����ǹλ�ð�����Ȼ���ƶ������������λ�á�

	// 1.2 ƽ��(�������)
	D3DXMATRIX trans2;
	D3DXMatrixTranslation(&trans2, 1.5f, -1.0f, 5.0f); /* ����/����/��ǰ���ƶ����� */
	*mat *= trans2;          //ע��mat��ǰ�ı任����mat_0�Ļ����ϣ���������ﱾ���ƶ�������λ�á�

	// 2. ��ת��Ŀ�꣺��ǹ����������ӽ���ת������λ�á���
	D3DXMATRIX mat_1;
	D3DXMatrixInverse(&mat_1, nullptr, &mat_0); // ȡmat_0����任

	// ��ת��Ϊ��ˮƽ�ᣨ��ȷ�����ʹ�ֱ�ᣨy�ᣩ��

	// vlookat: ��ǰ�ӽǵĳ���
	D3DXVECTOR3 vlookat = D3DXVECTOR3(camera->m_view.x - camera->m_pos.x,
		camera->m_view.y - camera->m_pos.y, camera->m_view.z - camera->m_pos.z);
	// haxis: ��ǰ�ӽǳ����y��Ĳ��������ֱ�ڵ�ǰ�ӽǳ�����y����ɵĴ�ֱƽ�棬��ʼ��λ��xozƽ���ڡ�
	D3DXVECTOR3 haxis;
	D3DXVec3Cross(&haxis, &vlookat, &yaxis);

	D3DXMATRIX rot_h, rot_v;
	D3DXMatrixRotationAxis(&rot_h, &yaxis, 0.5f * D3DX_PI - *angleH); //ˮƽ��ת
	D3DXMatrixRotationAxis(&rot_v, &haxis, -*angleV);  				 //��ֱ��ת
	/* ����һ������ע�ͣ�
	1. Ĭ��D3D�е�������Ϊ�������������� ����ʱ�������ҡ�
	2. ��Ϊmat�ı任�Ѿ���ģ���ƶ����Σ�����任���������任����
	�������ƾ���ļ������壬�Ƚ�����mat�任��������������ԭ��ı任����mat_0���棩��
	�ٽ����ʵ�����ת������rot�����ٽ�������λ�õı任����mat_0�������ɡ�
	��Ϊ���������������ζԾ���˻�������ֱ�ӽ��о���˷����ɵõ����������ܱ任��*/
	*mat = *mat * mat_1 * rot_h * rot_v * mat_0; /* P-1AP ~ A */
}



StaticMeshGUI::StaticMeshGUI() :
	vbPtr(0), numVerts(0), vertexSize(0), FVF(0)
{
}