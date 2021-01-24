// Stranded Engine
// 渲染器 抽象接口

// 说明：
// 提供渲染器的抽象基类作为接口。
// 设计模式：
// factory。工厂定义在Engine/Engine.h|cpp。

#pragma once

#include "EngineBase.h"
#include "STEMath.h"
#include "STECamera.h"


class IRenderer {
public:
	virtual ~IRenderer() {}

public:
	// 测试用：弹出一个对话框
	virtual void bump(const char* format, ...) = 0; 


	/* 引擎相关 */
public: 
	/* 初始化引擎 */
	virtual bool Initialize(
		int width, 
		int height, 
		HWND mainWindow /* 游戏的窗口句柄 */ , 
		bool fullScreen) = 0;
	/* 关闭引擎 */
	virtual void Release() = 0;

	/* 开始绘制 */
	virtual void StartRendering() = 0;
	/* 结束绘制 */
	virtual void EndRendering() = 0;
	

	/* 光照和材质 */
public:	


	/* 纹理贴图 */
public:	
	// 读取一个贴图到内存，并给它一个标号
	virtual bool AddTexture2D(const char* fileName, int* texId) = 0;
	// 用标号来应用贴图
	virtual void ApplyTexture(int stage, int texId) = 0;
	// 释放所有贴图资源
	virtual void ReleaseTextures() = 0;


	/* GUI 相关 */
public: 
	/* 创建GUI */  //创建一个GUI对象，用于建立一套菜单/背景/按钮的组合。
	virtual bool CreateGUIObject(int& guiID) = 0;
	//创建一个字体对象用于界面的字体设定
	virtual bool CreateFontObject(
		int& fontID, 
		const char* fontName, 
		int weight, 
		bool italic, 
		int size) = 0;
	//在一个GUI对象中创建背景，按钮等
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
	
	/* 显示GUI */   //应该包含在StartRendering() 和 EndRendering() 之间。显示文字最后调用以免被覆盖
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
	
	/* 释放GUI */
	virtual void ReleaseGUIs() = 0;  //销毁场景时即可调用
	virtual void ReleaseFonts() = 0; //通常在程序结束时调用


	/* 模型相关 */
public:	
	// 读取X模型
	virtual bool LoadXModel(
		const char* fileName, 
		int* xModelId) = 0; 
	// 计算给定时间后的模型动画的变换矩阵
	virtual void UpdateXAnimation(
		int xModelId, 
		float time, 
		const Matrix* mat) = 0;
	// 渲染X模型
	virtual bool RenderXModel(int xModelId) = 0;
	// 释放X模型
	virtual void ReleaseXModels() = 0;


	/* 物理碰撞 */
public: 
	virtual void GetBoundingSphere(
		int xModelId, 
		Vector3* center, 
		float* radius) = 0;


	/* 粒子系统 */
public:
	virtual void EnablePointSprites(
		float size, 
		float min, 
		float a, 
		float b, 
		float c) = 0;
	virtual void DisablePointSprites() = 0;


	/* 摄影机相关 */
public: 
	virtual void SetCamera(Vector3 pos, Vector3 view, Vector3 up) = 0;
	virtual void SetWorldMatrix(Matrix* mat) = 0;
	virtual void UpdateHudPositionFromCamera(
		Matrix* hud, 
		Camera* camera, 
		float* angleH, 
		float* angleV) = 0;
}; // interface Renderer
