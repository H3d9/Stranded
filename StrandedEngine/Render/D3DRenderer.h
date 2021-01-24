// Stranded Engine
// D3D渲染器 接口声明

#pragma once

#include "IRenderer.h"


/* 依赖项引用 */

#include "GUI.h"
#include "XMesh.h"


/* 标准库引用 */

#include <string>
#include <vector>
using std::string;
using std::vector;


/* D3D渲染器 内部结构定义 */

struct D3DTexture
{
	string fileName;
	LPDIRECT3DTEXTURE9 image;
	int Width, Height;
};


/** IRenderer接口实现： D3DRenderer */

class D3DRenderer : public IRenderer {
public:
	D3DRenderer();
	virtual ~D3DRenderer();

	virtual void bump(const char* format, ...) override;

public:	/* 引擎相关 */
	virtual bool Initialize(
		int width, 
		int height, 
		HWND mainWindow, 
		bool fullScreen) override;
	virtual void Release() override;

	virtual void StartRendering() override;
	virtual void EndRendering() override;

private:  /* 初始化场景 */
	void SetupScene();


public: /* 纹理贴图 */
	virtual bool AddTexture2D(const char* fileName, int* texId) override;
	virtual void ApplyTexture(int stage, int texId) override;
	virtual void ReleaseTextures() override;


public: /* GUI */
	virtual bool CreateGUIObject(int& guiID) override;
	virtual bool CreateFontObject(
		int& fontID, 
		const char* fontName, 
		int weight, 
		bool italic, 
		int size) override;
	virtual bool AddGUIBackdrop(int guiID, const char* fileName) override;
	virtual bool AddGUIStaticText(
		int guiID, 
		int id, 
		const wchar_t* text, 
		int x, 
		int y, 
		unsigned long color, 
		int fontID) override;
	virtual bool AddGUIDynamicText(
		int guiID, 
		int id, 
		const wchar_t* text, 
		int x, 
		int y, 
		unsigned long color, 
		int fontID) override;
	virtual bool AddGUIButton(
		int guiID, 
		int id, 
		int x, 
		int y, 
		const char* upTex, 
		const char* overTex, 
		const char* downTex) override;
	virtual void RenderGUI(
		int guiID, 
		bool LMBDown, 
		int mouseX, 
		int mouseY,
		void(*funcPtr)(int id, int state)) override;
	virtual void DisplayText(
		int fontID, 
		int x,
		int y, 
		unsigned long color,
		const wchar_t* text) override;
	virtual void ReleaseGUIs() override;
	virtual void ReleaseFonts() override;

private: /* 生成GUI控件的辅助函数。其中使用的结构StaticMeshGUI在GUI.h中声明 */
	bool AddStaticMesh_GUI(void* vertexData, int* staticId);
	bool RenderStaticMeshes_GUI(int staticId);
	void ReleaseStaticMeshes_GUI();


public:	/* 模型相关 */
	virtual bool LoadXModel(
		const char* fileName, 
		int* xModelId) override;
	virtual void UpdateXAnimation(
		int xModelId, 
		float time, 
		const Matrix* mat) override;
	virtual bool RenderXModel(int xModelId) override;
	virtual void ReleaseXModels() override;

public: /* 物理碰撞 */
	virtual void GetBoundingSphere(
		int xModelId, 
		Vector3* center, 
		float* radius) override;

public: /* 粒子系统 */
	virtual void EnablePointSprites(
		float size, 
		float min, 
		float a,
		float b, 
		float c) override;
	virtual void DisablePointSprites() override;

public: /* 摄影机相关 */
	virtual void SetCamera(Vector3 pos, Vector3 view, Vector3 up) override;
	virtual void SetWorldMatrix(Matrix* mat) override;
	virtual void UpdateHudPositionFromCamera(
		Matrix* hud, 
		Camera* camera, 
		float* angleH, 
		float* angleV) override;



private:
	int						m_screenWidth;
	int						m_screenHeight;
	bool					m_fullScreen;

	IDirect3DDevice9*		m_Device;             //虚拟设备
	D3DCOLOR				m_Color;              //清屏时屏幕刷新所用的颜色

	D3DXMATRIX				m_proj;				  //投影矩阵

	vector<D3DTexture>		m_textureList;        //纹理贴图列表
	int						m_numTextures;        //纹理贴图的数量

	vector<CXModel>			m_xModels;		      //Xmodel列表
	int						m_numXModels;         //Xmodel数量

	vector<ID3DXFont*>		m_fonts;              //字体列表
	int						m_numFonts;           //字体数量

	vector<GUIObject>		m_guiList;			    //GUI列表，可能用到多个GUI界面
	int						m_totalGUIs;			//GUI总数

	vector<StaticMeshGUI>	m_staticMeshGUIList;    //GUI静态网格列表
	int						m_numStaticGUIMeshes;   //GUI静态网格的数量
};

/*  D3D渲染器与通用接口的调用关系：
	1. 添加某个控件时，通过渲染器的成员函数，使用该控件所需的资源来创建控件 ->
	2. 渲染器通过D3D提供的接口在渲染器内部数据结构中使用D3D格式来创建控件需要的贴图和顶点缓存，
	   并返回这些缓存的下标id给通用接口 ->
	3. 通用接口将被创建的控件类型与渲染器在创建该控件时返回的下标id整合，并保存至自身内部的控件数组 ->
	4. 在渲染阶段，D3D渲染器的成员RenderGUI()将遍历通用接口中的控件数组，并按照控件的类别，
	   使用需要的方法进行渲染。由于通用接口只保存索引，而真实缓存被保存到渲染器的成员，
	   因此遍历到通用接口中某个对象类型时，就取出渲染该对象所需的缓存索引，
	   然后渲染器的RenderGUI()按索引从自身内部缓存取出该控件所需的缓存，来进行实际的渲染。
	*/