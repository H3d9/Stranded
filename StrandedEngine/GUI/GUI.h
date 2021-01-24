// Stranded Engine Renderer
// GUI ģ������

// ע�⣺���ļ�����Ϊ����ӿڣ�����¶�������ⲿ��
// ���з���Ӧ�ñ���Ⱦ����װ��

#pragma once

#include "EngineBase.h"


/* ��׼������ */

#include <string>
#include <vector>
using std::wstring;
using std::vector;


/* GUI �ڲ��ṹ���� */

struct StaticMeshGUI               //GUI��̬����(����GUI�ĸ����ṹ)
{
	StaticMeshGUI();

	LPDIRECT3DVERTEXBUFFER9 vbPtr; //���㻺��ָ��
	int numVerts;                  //��������
	int vertexSize;                //�����С
	DWORD FVF;                     //��������
};

struct GUIControl           //GUI�ؼ���ֻ��¼������������GUI����ĳ�Ա��ÿ��GUI��������һ��GUI�ؼ��б�
	                        //һ��GUI�ؼ�ֻ��¼һ������/��ť/�˵������GUI������ɵ�һ�׽�����һ��GUI����
{
	GUIControl();

	int m_type;               //�������ͣ��궨�壬GUI��ͷ��
	int m_id;                 //��ʶ�ڵ�ǰGUI���������GUI�ؼ��У�����ؼ���Ψһid�����û�ָ�������ڴ���ص�������
	unsigned long m_color;

	int m_listID;             //��Ϊtext����ʹ�õ�����id�������Ƕ��㻺��id

	int m_upTex, m_downTex, m_overTex;   //��ť��ͼ��id
	int m_backdropTex;                   //������ͼ��id

	float m_xPos, m_yPos;
	float m_width, m_height;

	wstring m_text;                        //��̬�ַ���
	const wchar_t* m_p_text;               //��̬�ַ��� ע�⣺��ָ��ָ�����ַ���
};

class GUIObject          //GUI������һ�ײ˵�/����/��ť�����
{
public:
	GUIObject();
	~GUIObject();

public: /* ����GUI����������� */
	/* ΪGUI������ӱ���ͼƬ */
	bool AddBackdrop(int texID, int staticID);  //��������һ��������Ҫһ����ͼid��һ�����㻺��id
	
	/* ΪGUI��������ı��Ͱ�ť */
	/* ��һ��������id����˵����GUIControl��m_id */
	bool AddStaticText(int id, const wchar_t* text, int x, int y, unsigned long color, int fontID);
	bool AddDynamicText(int id, const wchar_t* text, int x, int y, unsigned long color, int fontID);
	bool AddButton(int id, int x, int y, int width, int height, 
		int upID, int overID, int downID, int staticID); //���4��������3����ť��ͼid��1�����㻺��id

public: /* �ӿ� */
	GUIControl* GetBackDrop();
	GUIControl* GetControl(int index);
	int         GetTotalControls();

public:
	void ShutDown();

private:
	vector<GUIControl> m_Controls;        //GUI�ؼ��б�
	int                m_totalControls;

	int                m_backdropID;      //�������б��е�id
};