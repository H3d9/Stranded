// Stranded Engine
// D3D��Ⱦ�� �ӿ�����

#pragma once

#include "IRenderer.h"


/* ���������� */

#include "GUI.h"
#include "XMesh.h"


/* ��׼������ */

#include <string>
#include <vector>
using std::string;
using std::vector;


/* D3D��Ⱦ�� �ڲ��ṹ���� */

struct D3DTexture
{
	string fileName;
	LPDIRECT3DTEXTURE9 image;
	int Width, Height;
};


/** IRenderer�ӿ�ʵ�֣� D3DRenderer */

class D3DRenderer : public IRenderer {
public:
	D3DRenderer();
	virtual ~D3DRenderer();

	virtual void bump(const char* format, ...) override;

public:	/* ������� */
	virtual bool Initialize(
		int width, 
		int height, 
		HWND mainWindow, 
		bool fullScreen) override;
	virtual void Release() override;

	virtual void StartRendering() override;
	virtual void EndRendering() override;

private:  /* ��ʼ������ */
	void SetupScene();


public: /* ������ͼ */
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

private: /* ����GUI�ؼ��ĸ�������������ʹ�õĽṹStaticMeshGUI��GUI.h������ */
	bool AddStaticMesh_GUI(void* vertexData, int* staticId);
	bool RenderStaticMeshes_GUI(int staticId);
	void ReleaseStaticMeshes_GUI();


public:	/* ģ����� */
	virtual bool LoadXModel(
		const char* fileName, 
		int* xModelId) override;
	virtual void UpdateXAnimation(
		int xModelId, 
		float time, 
		const Matrix* mat) override;
	virtual bool RenderXModel(int xModelId) override;
	virtual void ReleaseXModels() override;

public: /* ������ײ */
	virtual void GetBoundingSphere(
		int xModelId, 
		Vector3* center, 
		float* radius) override;

public: /* ����ϵͳ */
	virtual void EnablePointSprites(
		float size, 
		float min, 
		float a,
		float b, 
		float c) override;
	virtual void DisablePointSprites() override;

public: /* ��Ӱ����� */
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

	IDirect3DDevice9*		m_Device;             //�����豸
	D3DCOLOR				m_Color;              //����ʱ��Ļˢ�����õ���ɫ

	D3DXMATRIX				m_proj;				  //ͶӰ����

	vector<D3DTexture>		m_textureList;        //������ͼ�б�
	int						m_numTextures;        //������ͼ������

	vector<CXModel>			m_xModels;		      //Xmodel�б�
	int						m_numXModels;         //Xmodel����

	vector<ID3DXFont*>		m_fonts;              //�����б�
	int						m_numFonts;           //��������

	vector<GUIObject>		m_guiList;			    //GUI�б������õ����GUI����
	int						m_totalGUIs;			//GUI����

	vector<StaticMeshGUI>	m_staticMeshGUIList;    //GUI��̬�����б�
	int						m_numStaticGUIMeshes;   //GUI��̬���������
};

/*  D3D��Ⱦ����ͨ�ýӿڵĵ��ù�ϵ��
	1. ���ĳ���ؼ�ʱ��ͨ����Ⱦ���ĳ�Ա������ʹ�øÿؼ��������Դ�������ؼ� ->
	2. ��Ⱦ��ͨ��D3D�ṩ�Ľӿ�����Ⱦ���ڲ����ݽṹ��ʹ��D3D��ʽ�������ؼ���Ҫ����ͼ�Ͷ��㻺�棬
	   ��������Щ������±�id��ͨ�ýӿ� ->
	3. ͨ�ýӿڽ��������Ŀؼ���������Ⱦ���ڴ����ÿؼ�ʱ���ص��±�id���ϣ��������������ڲ��Ŀؼ����� ->
	4. ����Ⱦ�׶Σ�D3D��Ⱦ���ĳ�ԱRenderGUI()������ͨ�ýӿ��еĿؼ����飬�����տؼ������
	   ʹ����Ҫ�ķ���������Ⱦ������ͨ�ýӿ�ֻ��������������ʵ���汻���浽��Ⱦ���ĳ�Ա��
	   ��˱�����ͨ�ýӿ���ĳ����������ʱ����ȡ����Ⱦ�ö�������Ļ���������
	   Ȼ����Ⱦ����RenderGUI()�������������ڲ�����ȡ���ÿؼ�����Ļ��棬������ʵ�ʵ���Ⱦ��
	*/