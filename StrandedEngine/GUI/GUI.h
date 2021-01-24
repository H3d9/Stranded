// Stranded Engine Renderer
// GUI 模块声明

// 注意：该文件不作为对外接口，不暴露至引擎外部。
// 所有方法应该被渲染器封装。

#pragma once

#include "EngineBase.h"


/* 标准库引用 */

#include <string>
#include <vector>
using std::wstring;
using std::vector;


/* GUI 内部结构定义 */

struct StaticMeshGUI               //GUI静态网格(生成GUI的辅助结构)
{
	StaticMeshGUI();

	LPDIRECT3DVERTEXBUFFER9 vbPtr; //顶点缓存指针
	int numVerts;                  //顶点数量
	int vertexSize;                //顶点大小
	DWORD FVF;                     //顶点类型
};

struct GUIControl           //GUI控件（只记录索引），用作GUI对象的成员。每个GUI对象中有一个GUI控件列表。
	                        //一个GUI控件只记录一个背景/按钮/菜单，多个GUI组件构成的一套界面是一个GUI对象。
{
	GUIControl();

	int m_type;               //对象类型（宏定义，GUI开头）
	int m_id;                 //标识在当前GUI对象的所有GUI控件中，这个控件的唯一id。由用户指定，用于处理回调函数。
	unsigned long m_color;

	int m_listID;             //若为text则是使用的字体id，否则是顶点缓存id

	int m_upTex, m_downTex, m_overTex;   //按钮贴图的id
	int m_backdropTex;                   //背景贴图的id

	float m_xPos, m_yPos;
	float m_width, m_height;

	wstring m_text;                        //静态字符串
	const wchar_t* m_p_text;               //动态字符串 注意：该指针指向常量字符串
};

class GUIObject          //GUI对象，是一套菜单/背景/按钮的组合
{
public:
	GUIObject();
	~GUIObject();

public: /* 操作GUI对象，添加内容 */
	/* 为GUI对象添加背景图片 */
	bool AddBackdrop(int texID, int staticID);  //背景仅有一个，它需要一个贴图id，一个顶点缓存id
	
	/* 为GUI对象添加文本和按钮 */
	/* 第一个参数“id”的说明见GUIControl的m_id */
	bool AddStaticText(int id, const wchar_t* text, int x, int y, unsigned long color, int fontID);
	bool AddDynamicText(int id, const wchar_t* text, int x, int y, unsigned long color, int fontID);
	bool AddButton(int id, int x, int y, int width, int height, 
		int upID, int overID, int downID, int staticID); //最后4个参数：3个按钮贴图id，1个顶点缓存id

public: /* 接口 */
	GUIControl* GetBackDrop();
	GUIControl* GetControl(int index);
	int         GetTotalControls();

public:
	void ShutDown();

private:
	vector<GUIControl> m_Controls;        //GUI控件列表
	int                m_totalControls;

	int                m_backdropID;      //背景在列表中的id
};