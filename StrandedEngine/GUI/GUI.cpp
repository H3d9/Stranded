// Stranded Engine Renderer
// GUI 模块定义

#include "GUI.h"


GUIControl::GUIControl() :
	m_type(0),
	m_id(0),
	m_color(0),
	m_listID(0),
	m_upTex(0), m_downTex(0), m_overTex(0),
	m_backdropTex(0),
	m_xPos(0.0f), m_yPos(0.0f),
	m_width(0.0f), m_height(0.0f),
	m_text(L""), m_p_text(nullptr)
{
}

GUIObject::GUIObject() : m_totalControls(0), m_backdropID(-1)
{
	m_Controls.clear();
}

GUIObject::~GUIObject()
{ 
	GUIObject::ShutDown();
}


bool GUIObject::AddBackdrop(int texID, int staticID)
{
	if(texID < 0 || staticID < 0) return false;

	/* 如果已经有背景，则替换；否则新建一个 */
	if (m_backdropID < 0)
	{
		m_Controls.push_back(GUIControl());

		m_Controls[m_totalControls].m_type = GUI_BACKDROP;
		m_Controls[m_totalControls].m_backdropTex = texID;
		m_Controls[m_totalControls].m_listID = staticID;
		m_backdropID = m_totalControls;
		m_totalControls++;
	}
	else
	{
		m_Controls[m_backdropID].m_backdropTex = texID;
		m_Controls[m_backdropID].m_listID = staticID;
	}

	return true;
}

bool GUIObject::AddStaticText(int id, const wchar_t * text, int x, int y, unsigned long color, int fontID)
{
	if (!text || fontID < 0) return false;

	m_Controls.push_back(GUIControl());

	m_Controls[m_totalControls].m_type = GUI_STATICTEXT;
	m_Controls[m_totalControls].m_id = id;
	m_Controls[m_totalControls].m_color = color;
	m_Controls[m_totalControls].m_xPos = (float)x;
	m_Controls[m_totalControls].m_yPos = (float)y;
	m_Controls[m_totalControls].m_listID = fontID;

	m_Controls[m_totalControls].m_text = text;

	m_totalControls++;
	return true;
}

bool GUIObject::AddDynamicText(int id, const wchar_t * text, int x, int y, unsigned long color, int fontID)
{
	if (!text || fontID < 0) return false;

	m_Controls.push_back(GUIControl());

	m_Controls[m_totalControls].m_type = GUI_DYNAMICTEXT;
	m_Controls[m_totalControls].m_id = id;
	m_Controls[m_totalControls].m_color = color;
	m_Controls[m_totalControls].m_xPos = (float)x;
	m_Controls[m_totalControls].m_yPos = (float)y;
	m_Controls[m_totalControls].m_listID = fontID;

	m_Controls[m_totalControls].m_text = text;

	m_totalControls++;
	return true;
}

bool GUIObject::AddButton(int id, int x, int y, int width, int height, int upID, int overID, int downID, int staticID)
{
	if (downID < 0 || upID < 0 || overID < 0 || staticID < 0) return false;

	m_Controls.push_back(GUIControl());

	m_Controls[m_totalControls].m_type = GUI_BUTTON;
	m_Controls[m_totalControls].m_id = id;
	m_Controls[m_totalControls].m_xPos = (float)x;
	m_Controls[m_totalControls].m_yPos = (float)y;
	m_Controls[m_totalControls].m_width = (float)width;
	m_Controls[m_totalControls].m_height = (float)height;
	m_Controls[m_totalControls].m_upTex = upID;
	m_Controls[m_totalControls].m_downTex = downID;
	m_Controls[m_totalControls].m_overTex = overID;
	m_Controls[m_totalControls].m_listID = staticID;

	m_totalControls++;
	return true;
}

GUIControl* GUIObject::GetBackDrop()
{
	if (m_backdropID < 0) return nullptr;

	return &m_Controls[m_backdropID];
}

GUIControl* GUIObject::GetControl(int index)
{
	if (index < 0 || index >= m_totalControls)
	{
		return nullptr;
	}

	return &m_Controls[index];
}

int GUIObject::GetTotalControls()
{
	return m_totalControls;
}

void GUIObject::ShutDown()
{
	/* 释放控件列表 */
	m_Controls.clear();
	m_totalControls = 0;
}
