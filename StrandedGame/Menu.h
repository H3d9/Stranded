#pragma once
#include "GameDef.h"
#include "GameBase.h"

// Menu Params
extern int				g_GUIGeneralID;			 //通用 GUI （在所有菜单中都渲染，如FPS）
extern int				g_GUIMainMenuID;			 //主菜单 GUI
extern int				g_GUIStartID;			 //开始游戏菜单 GUI
extern int				g_GUICreditsID;			 //Credits界面 GUI
extern int				g_GUILoadingID;			 //读取界面GUI
extern MenuState		g_currentMenu;			 //当前菜单状态，默认为主菜单

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
