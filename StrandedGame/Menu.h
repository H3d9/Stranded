#pragma once
#include "GameDef.h"
#include "GameBase.h"

// Menu Params
extern int				g_GUIGeneralID;			 //ͨ�� GUI �������в˵��ж���Ⱦ����FPS��
extern int				g_GUIMainMenuID;			 //���˵� GUI
extern int				g_GUIStartID;			 //��ʼ��Ϸ�˵� GUI
extern int				g_GUICreditsID;			 //Credits���� GUI
extern int				g_GUILoadingID;			 //��ȡ����GUI
extern MenuState		g_currentMenu;			 //��ǰ�˵�״̬��Ĭ��Ϊ���˵�

/**********************************************
 *
 *      Menu Functions
 *
 **********************************************/
bool InitializeGeneralMenu();
bool InitializeMenu();

/* GameLoop() SubRoutines */
void RenderMenu();
void MenuCallback(int id, int state);
