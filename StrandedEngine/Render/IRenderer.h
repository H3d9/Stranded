// Stranded Engine
// ��Ⱦ�� ����ӿ�

// ˵����
// �ṩ��Ⱦ���ĳ��������Ϊ�ӿڡ�
// ���ģʽ��
// factory������������Engine/Engine.h|cpp��

#pragma once

#include "EngineBase.h"
#include "STEMath.h"
#include "STECamera.h"


class IRenderer {
public:
	virtual ~IRenderer() {}

public:
	// �����ã�����һ���Ի���
	virtual void bump(const char* format, ...) = 0; 


	/* ������� */
public: 
	/* ��ʼ������ */
	virtual bool Initialize(
		int width, 
		int height, 
		HWND mainWindow /* ��Ϸ�Ĵ��ھ�� */ , 
		bool fullScreen) = 0;
	/* �ر����� */
	virtual void Release() = 0;

	/* ��ʼ���� */
	virtual void StartRendering() = 0;
	/* �������� */
	virtual void EndRendering() = 0;
	

	/* ���պͲ��� */
public:	


	/* ������ͼ */
public:	
	// ��ȡһ����ͼ���ڴ棬������һ�����
	virtual bool AddTexture2D(const char* fileName, int* texId) = 0;
	// �ñ����Ӧ����ͼ
	virtual void ApplyTexture(int stage, int texId) = 0;
	// �ͷ�������ͼ��Դ
	virtual void ReleaseTextures() = 0;


	/* GUI ��� */
public: 
	/* ����GUI */  //����һ��GUI�������ڽ���һ�ײ˵�/����/��ť����ϡ�
	virtual bool CreateGUIObject(int& guiID) = 0;
	//����һ������������ڽ���������趨
	virtual bool CreateFontObject(
		int& fontID, 
		const char* fontName, 
		int weight, 
		bool italic, 
		int size) = 0;
	//��һ��GUI�����д�����������ť��
	virtual bool AddGUIBackdrop(int guiID, const char* fileName) = 0;
	virtual bool AddGUIStaticText(
		int guiID, 
		int id, 
		const wchar_t* text, 
		int x, 
		int y, 
		unsigned long color, 
		int fontID) = 0;
	virtual bool AddGUIDynamicText(
		int guiID, 
		int id, 
		const wchar_t* text,
		int x, 
		int y, 
		unsigned long color, 
		int fontID) = 0;
	virtual bool AddGUIButton(
		int guiID, 
		int id, 
		int x, 
		int y,
		const char* upTex, 
		const char* overTex, 
		const char* downTex) = 0;
	
	/* ��ʾGUI */   //Ӧ�ð�����StartRendering() �� EndRendering() ֮�䡣��ʾ�������������ⱻ����
	virtual void RenderGUI(
		int guiID, 
		bool LMBDown, 
		int mouseX, 
		int mouseY,
		void(*funcPtr)(int id, int state)) = 0;	
	virtual void DisplayText(
		int fontID, 
		int x, 
		int y, 
		unsigned long color, 
		const wchar_t* text) = 0;
	
	/* �ͷ�GUI */
	virtual void ReleaseGUIs() = 0;  //���ٳ���ʱ���ɵ���
	virtual void ReleaseFonts() = 0; //ͨ���ڳ������ʱ����


	/* ģ����� */
public:	
	// ��ȡXģ��
	virtual bool LoadXModel(
		const char* fileName, 
		int* xModelId) = 0; 
	// �������ʱ����ģ�Ͷ����ı任����
	virtual void UpdateXAnimation(
		int xModelId, 
		float time, 
		const Matrix* mat) = 0;
	// ��ȾXģ��
	virtual bool RenderXModel(int xModelId) = 0;
	// �ͷ�Xģ��
	virtual void ReleaseXModels() = 0;


	/* ������ײ */
public: 
	virtual void GetBoundingSphere(
		int xModelId, 
		Vector3* center, 
		float* radius) = 0;


	/* ����ϵͳ */
public:
	virtual void EnablePointSprites(
		float size, 
		float min, 
		float a, 
		float b, 
		float c) = 0;
	virtual void DisablePointSprites() = 0;


	/* ��Ӱ����� */
public: 
	virtual void SetCamera(Vector3 pos, Vector3 view, Vector3 up) = 0;
	virtual void SetWorldMatrix(Matrix* mat) = 0;
	virtual void UpdateHudPositionFromCamera(
		Matrix* hud, 
		Camera* camera, 
		float* angleH, 
		float* angleV) = 0;
}; // interface Renderer
