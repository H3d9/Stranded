// Stranded Engine
// D3D渲染器 接口定义

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
	/* 释放虚拟设备 */
	if (m_Device) {
		m_Device->Release();
		m_Device = nullptr;
	}
}

void D3DRenderer::bump(const char* format, ...)
{
	char buffer[1024]; // 测试信息不应超过1024B。
	va_list arg;
	va_start(arg, format);
	vsprintf(buffer, format, arg);
	va_end(arg);
	MessageBoxA(0, buffer, "message", MB_OK);

}

/* 引擎相关 */

bool D3DRenderer::Initialize(int width, int height, HWND mainWindow, bool fullScreen) {
	/* 初始化成员 */
	this->m_screenWidth			= width;
	this->m_screenHeight		= height;
	this->m_fullScreen			= fullScreen;

	/* 创建D3D设备，并获取显示设置和硬件能力存放入mode，以进行相应设置 */
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
	
	/* 填充 D3DPRESENT_PARAMETERS 结构 */
	D3DPRESENT_PARAMETERS Params;
	ZeroMemory(&Params, sizeof(Params));

	if (m_fullScreen)
	{
		Params.Windowed = false;
		Params.FullScreen_RefreshRateInHz = mode.RefreshRate;  //全屏刷新率和显示设置一致
		Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //设置显示频率为后台刷新完一帧时
		Params.BackBufferWidth = width;                        //后台缓存宽度
		Params.BackBufferHeight = height;                      //后台缓存高度
	}
	else
	{
		Params.Windowed = true;
		Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //设置非全屏就立即显示
	}
	Params.hDeviceWindow = mainWindow;          //设备窗口句柄（游戏主窗口）
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;  //前台与后台缓存交换方法
	Params.BackBufferFormat = mode.Format;      //后台缓存格式，与显示设置一致
	Params.BackBufferCount = 1;                 //后台缓存个数为1
	Params.EnableAutoDepthStencil = true;       //自动深度和模板缓存
	Params.AutoDepthStencilFormat = D3DFMT_D16; //缓存格式

	//多重采样是预定义值，如不支持则设为无
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

	/* 创建虚拟设备 */
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

	/* 初始化场景 */
	SetupScene();

	return true;
}

void D3DRenderer::StartRendering() {
	static const DWORD clearFlags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;

	//注：默认clearColor, clearDepth, clearStencil 分别为 1,1,0
	//if (clearColor)   clearFlags |= D3DCLEAR_TARGET;
	//if (clearDepth)   clearFlags |= D3DCLEAR_ZBUFFER;
	//if (clearStencil) clearFlags |= D3DCLEAR_STENCIL;

	/* 清屏 */
	m_Device->Clear(0, 0, clearFlags, m_Color, 1.0f, 0);

	/* 开始绘制 */
	m_Device->BeginScene();
}

void D3DRenderer::EndRendering() {
	/* 结束绘制 */
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

	/* 计算并设置投影矩阵 */
	D3DXMatrixPerspectiveFovLH(
		&m_proj,
		D3DX_PI / 4,
		static_cast<float>(m_screenWidth) / static_cast<float>(m_screenHeight),
		0.1f,
		1000.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &m_proj);

	/* 设置清屏颜色 */
	m_Color = D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);

	/* 关闭光照 */
	m_Device->SetRenderState(D3DRS_LIGHTING, false);
	//m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

/* 纹理贴图 */

bool D3DRenderer::AddTexture2D(const char* fileName, int* texId) {

	if (!texId) {
		return false;
	}
	
	// 分配贴图编号
	int index = m_numTextures;

	m_textureList.push_back(D3DTexture());

	/* 设置纹理的文件名 */
	m_textureList[index].fileName = fileName;

	/* 从文件创建纹理。
	 * 使用扩展函数可获取贴图属性如长宽 */
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
	/* 释放贴图列表 */
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

/* GUI 相关 */

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

	/* 创建字体 */
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

	/* 添加贴图得到贴图id */
	if (!AddTexture2D(fileName, &texID)) return false;

	D3DCOLOR white = D3DCOLOR_XRGB(255, 255, 255);
	
	/* 添加顶点缓存得到顶点缓存id */
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

	/* 渲染背景 */
	GUIControl* pBackdrop = gui->GetBackDrop();

	if (pBackdrop)
	{
		ApplyTexture(0, pBackdrop->m_backdropTex);
		RenderStaticMeshes_GUI(pBackdrop->m_listID);
		ApplyTexture(0, -1);
	}

	/* 渲染按钮与文本 */
	for (int i = 0; i < gui->GetTotalControls(); i++)
	{
		int status = BUTTON_UP;

		/* 获取当前的GUI控件指针 */
		GUIControl* pCtrl = gui->GetControl(i);
		if (!pCtrl)
		{
			continue;
		}

		switch (pCtrl->m_type)
		{
		case GUI_STATICTEXT:
			/* 显示静态文本 */
			DisplayText(pCtrl->m_listID, (int)pCtrl->m_xPos, (int)pCtrl->m_yPos,
				pCtrl->m_color, pCtrl->m_text.c_str());
			break;
		case GUI_DYNAMICTEXT:
			/* 显示动态文本 */
			DisplayText(pCtrl->m_listID, (int)pCtrl->m_xPos, (int)pCtrl->m_yPos,
				pCtrl->m_color, pCtrl->m_p_text); //D3DCOLOR_XRGB(255, 255, 255)
			break;
		case GUI_BUTTON:
			/* 启用Alpha融合 */
			m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			/* 检查鼠标状态 */
			if (mouseX > pCtrl->m_xPos && mouseX < pCtrl->m_xPos + pCtrl->m_width &&
				mouseY > pCtrl->m_yPos && mouseY < pCtrl->m_yPos + pCtrl->m_height)
			{
				if (LMBDown) status = BUTTON_DOWN;
				else status = BUTTON_OVER;
			}

			/* 设置按钮贴图 */
			if (status == BUTTON_UP) ApplyTexture(0, pCtrl->m_upTex);
			if (status == BUTTON_OVER) ApplyTexture(0, pCtrl->m_overTex);
			if (status == BUTTON_DOWN) ApplyTexture(0, pCtrl->m_downTex);

			/* 绘制按钮贴图 */
			RenderStaticMeshes_GUI(pCtrl->m_listID);

			/* 关闭alpha融合与贴图 */
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

	/* 清空GUISystem列表 */ 
	m_guiList.clear();
	m_totalGUIs = 0;
}

void D3DRenderer::ReleaseFonts()
{
	/* 删除字体数组中的内容 */
	for (int i = 0; i < m_numFonts; i++)
	{
		if (m_fonts[i])
		{
			m_fonts[i]->Release();
			m_fonts[i] = nullptr;
		}
	}

	/* 释放字体列表 */
	m_fonts.clear();
	m_numFonts = 0;
}


bool D3DRenderer::AddStaticMesh_GUI(void* vertexData, int* staticId)
{
	/* 获取当前网格的个数 */
	int index = m_numStaticGUIMeshes;

	m_staticMeshGUIList.push_back(StaticMeshGUI());

	/* 填充新的 StaticMeshGUI 结构体中的基本数据 */
	m_staticMeshGUIList[index].numVerts = 4;
	m_staticMeshGUIList[index].FVF = D3DFVF_GUI;
	m_staticMeshGUIList[index].vertexSize = sizeof(GUIVertex);

	/* 创建顶点缓存 */
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

	/* 返回该静态网格的索引值 */
	if (staticId) {
		*staticId = m_numStaticGUIMeshes;
	}
	m_numStaticGUIMeshes++;

	return true;
}

bool D3DRenderer::RenderStaticMeshes_GUI(int staticId) {

	/* 将顶点缓存绑定到数据流 */
	if (FAILED(m_Device->SetStreamSource(0, m_staticMeshGUIList[staticId].vbPtr,
		0, m_staticMeshGUIList[staticId].vertexSize))) {
		return false;
	}

	/* 设置顶点格式 */
	if (FAILED(m_Device->SetFVF(m_staticMeshGUIList[staticId].FVF))) {
		return false;
	}

	/* 不存在索引缓存 使用DrawPrimitive() */
	if (FAILED(m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,
		m_staticMeshGUIList[staticId].numVerts / 2))) {
		return false;
	}

	return true;
}

void D3DRenderer::ReleaseStaticMeshes_GUI()
{
	/* 清理静态网格数组中的内容 */
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

/* 模型相关 */

bool D3DRenderer::LoadXModel(const char* fileName, int* xModelId)
{
	/* 创建新动态数组元素 */
	m_xModels.push_back(CXModel());

	m_xModels[m_numXModels].SetDevice(m_Device);
	if (!m_xModels[m_numXModels].LoadXFile(fileName))
		return false;

	// 从文件中读模型信息，顺便读取材质信息
	// （材质以D3DXMATERIAL格式保存，每份材质都包含两个成员：D3D的光照材质和贴图）
	//（D3D光照材质直接保存在该格式的matD3D成员中，而贴图以文件名的格式保存）
	//ID3DXMesh* mesh = 0;
	//ID3DXBuffer* matBuffer = nullptr;
	//DWORD numMats = 0;
	//if (FAILED(D3DXLoadMeshFromX("maps/GoldAwm.x", D3DXMESH_MANAGED, m_Device,
	//	0, &matBuffer, 0, &numMats, &mesh)))
	//	return false;
	////    一个模型可以包含多个材质，为当前模型的材质列表分配内存
	//D3DMATERIAL9* matList = new D3DMATERIAL9[numMats];
	//IDirect3DTexture9** texList = new IDirect3DTexture9*[numMats];
	//D3DXMATERIAL *mat = (D3DXMATERIAL*)matBuffer->GetBufferPointer();
	//for (auto i = 0; i < numMats; i++)
	//{
	////	 从D3DXMATERIAL缓存中提取D3D光照材质信息
	//	matList[i] = mat[i].MatD3D;
	////	 从D3DXMATERIAL缓存中提取贴图文件名，并加载该贴图
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

	/* 清理D3D设备 */
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

/* 物理碰撞 */

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

/* 粒子系统 */

void D3DRenderer::EnablePointSprites(float size, float min, float a, float b, float c)
{
	/* 启用点精灵，并设置点贴图的大小 */
	m_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_Device->SetRenderState(D3DRS_POINTSIZE, static_cast<DWORD>(size));

	/* 设置点贴图为可缩放状态，并设置最小大小和缩放参数 */
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

/* 摄影机相关 */

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
	//原始位置 
	D3DXMATRIX* mat = reinterpret_cast<D3DXMATRIX*>(hud);
	//注1：d3dxmatrix结构为4x4，a44 = 1.d3dvector的a44 = 0.
	//注2：仿射矩阵使用第4维描述平移。

	//D3DXMatrixIdentity(&mat);	
	//EA = A故此行被忽略

	static const D3DXVECTOR3 yaxis = { 0.0f, 1.0f, 0.0f };

	// 0. 摆正朝向(模型是反的)
	D3DXMatrixRotationAxis(mat, &yaxis, D3DX_PI);

	// 1.1 平移(相对世界原点)
	D3DXMATRIX trans1;
	D3DXMatrixTranslation(&trans1, camera->m_pos.x, camera->m_pos.y, camera->m_pos.z);
	*mat *= trans1;

	D3DXMATRIX mat_0 = *mat; //记录当前变换至mat_0，以准备逆变换。
							 //注：mat_0执行的变换：将枪位置摆正，然后移动到人物的中心位置。

	// 1.2 平移(相对人物)
	D3DXMATRIX trans2;
	D3DXMatrixTranslation(&trans2, 1.5f, -1.0f, 5.0f); /* 向右/向上/向前的移动距离 */
	*mat *= trans2;          //注：mat当前的变换：在mat_0的基础上，相对于人物本身移动到合适位置。

	// 2. 旋转（目标：把枪相对于人物视角旋转至合适位置。）
	D3DXMATRIX mat_1;
	D3DXMatrixInverse(&mat_1, nullptr, &mat_0); // 取mat_0的逆变换

	// 旋转分为沿水平轴（不确定）和垂直轴（y轴）。

	// vlookat: 当前视角的朝向
	D3DXVECTOR3 vlookat = D3DXVECTOR3(camera->m_view.x - camera->m_pos.x,
		camera->m_view.y - camera->m_pos.y, camera->m_view.z - camera->m_pos.z);
	// haxis: 当前视角朝向和y轴的叉积，方向垂直于当前视角朝向与y轴组成的垂直平面，且始终位于xoz平面内。
	D3DXVECTOR3 haxis;
	D3DXVec3Cross(&haxis, &vlookat, &yaxis);

	D3DXMATRIX rot_h, rot_v;
	D3DXMatrixRotationAxis(&rot_h, &yaxis, 0.5f * D3DX_PI - *angleH); //水平旋转
	D3DXMatrixRotationAxis(&rot_v, &haxis, -*angleV);  				 //垂直旋转
	/* 对下一行语句的注释：
	1. 默认D3D中的向量均为行向量，即矩阵 运算时从左向右。
	2. 因为mat的变换已经将模型移动两次（世界变换和相对人物变换），
	根据相似矩阵的几何意义，先将经过mat变换的向量进行坐标原点的变换（即mat_0的逆）。
	再进行适当的旋转操作（rot），再进行人物位置的变换（即mat_0）。即可。
	因为行向量从左到右依次对矩阵乘积，所以直接进行矩阵乘法即可得到所需结果的总变换。*/
	*mat = *mat * mat_1 * rot_h * rot_v * mat_0; /* P-1AP ~ A */
}



StaticMeshGUI::StaticMeshGUI() :
	vbPtr(0), numVerts(0), vertexSize(0), FVF(0)
{
}