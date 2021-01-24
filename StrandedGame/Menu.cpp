#include "Menu.h"

// Menu Params
int				g_GUIGeneralID = -1;		//通用 GUI （在所有菜单中都渲染，如FPS）
int				g_GUIMainMenuID = -1;			 //主菜单 GUI
int				g_GUIStartID = -1;			 //开始游戏菜单 GUI
int				g_GUICreditsID = -1;		//Credits界面 GUI
int				g_GUILoadingID = -1;		//读取界面GUI
MenuState		g_currentMenu = MS_MAIN_MENU;  //当前菜单状态，默认为主菜单

/* InitializeMenu - Sub Routines */

bool InitializeGeneralMenu()
{
	if (!g_Renderer->CreateGUIObject(g_GUIGeneralID))
	{
		return false;
	}

	/* 创建一个静态文本GUI */
	if (!g_Renderer->AddGUIStaticText(g_GUIGeneralID, STATIC_TEXT_ID, L"Version 1.0 by H3d9",
		PERCENT_OF(WINDOW_WIDTH, 0.79), PERCENT_OF(WINDOW_HEIGHT, 0.03),
		COLOR_XRGB(0, 255, 255), g_arialID))
	{
		return false;
	}

	/* 创建一个动态文本GUI（显示FPS） */
	if (!g_Renderer->AddGUIDynamicText(g_GUIGeneralID, DYNAMIC_TEXT_ID, g_fpsBuf,
		PERCENT_OF(WINDOW_WIDTH, 0.03), PERCENT_OF(WINDOW_HEIGHT, 0.03),
		COLOR_XRGB(0, 255, 0), g_timesID))
	{
		return false;
	}

	/* 创建一个动态文本GUI（显示鼠标位置） */
	if (!g_Renderer->AddGUIDynamicText(g_GUIGeneralID, DYNAMIC_TEXT_ID, g_mousebuf,
		PERCENT_OF(WINDOW_WIDTH, 0.4), PERCENT_OF(WINDOW_HEIGHT, 0.03),
		COLOR_XRGB(255, 0, 255), g_timesID))
	{
		return false;
	}

	return true;
}

bool InitializeMenu()
{
	/* 创建主菜单的GUI */
	if (!g_Renderer->CreateGUIObject(g_GUIMainMenuID))
	{
		return false;
	}
	if (!g_Renderer->AddGUIBackdrop(g_GUIMainMenuID, "menu/mainMenu.jpg"))
	{
		return false;
	}
	if (!g_Renderer->AddGUIButton(g_GUIMainMenuID, BUTTON_START_ID,
		PERCENT_OF(WINDOW_WIDTH, 0.05), PERCENT_OF(WINDOW_HEIGHT, 0.4),
		"menu/startUp.png", "menu/startOver.png", "menu/startDown.png"))
	{
		return false;
	}
	if (!g_Renderer->AddGUIButton(g_GUIMainMenuID, BUTTON_CREDITS_ID,
		PERCENT_OF(WINDOW_WIDTH, 0.05), PERCENT_OF(WINDOW_HEIGHT, 0.48),
		"menu/creditsUp.png", "menu/creditsOver.png", "menu/creditsDown.png"))
	{
		return false;
	}
	if (!g_Renderer->AddGUIButton(g_GUIMainMenuID, BUTTON_QUIT_ID,
		PERCENT_OF(WINDOW_WIDTH, 0.05), PERCENT_OF(WINDOW_HEIGHT, 0.56),
		"menu/quitUp.png", "menu/quitOver.png", "menu/quitDown.png"))
	{
		return false;
	}

	/* 创建开始菜单的GUI */
	if (!g_Renderer->CreateGUIObject(g_GUIStartID))
	{
		return false;
	}
	if (!g_Renderer->AddGUIBackdrop(g_GUIStartID, "menu/startMenu.jpg"))
	{
		return false;
	}
	if (!g_Renderer->AddGUIButton(g_GUIStartID, BUTTON_BACK_ID,
		PERCENT_OF(WINDOW_WIDTH, 0.05), PERCENT_OF(WINDOW_HEIGHT, 0.70),
		"menu/backUp.png", "menu/backOver.png", "menu/backDown.png"))
	{
		return false;
	}
	if (!g_Renderer->AddGUIButton(g_GUIStartID, BUTTON_LEVEL_1_ID,
		PERCENT_OF(WINDOW_WIDTH, 0.1), PERCENT_OF(WINDOW_HEIGHT, 0.15),
		"menu/level1Up.png", "menu/level1Over.png", "menu/level1Down.png"))
	{
		return false;
	}

	/* 创建credits的GUI */
	if (!g_Renderer->CreateGUIObject(g_GUICreditsID))
	{
		return false;
	}

	if (!g_Renderer->AddGUIBackdrop(g_GUICreditsID, "menu/creditsMenu.jpg"))
	{
		return false;
	}
	if (!g_Renderer->AddGUIButton(g_GUICreditsID, BUTTON_BACK_ID,
		PERCENT_OF(WINDOW_WIDTH, 0.05), PERCENT_OF(WINDOW_HEIGHT, 0.70),
		"menu/backUp.png", "menu/backOver.png", "menu/backDown.png"))
	{
		return false;
	}
	if (!g_Renderer->AddGUIStaticText(g_GUICreditsID, STATIC_TEXT_ID, L"懒得做了",
		PERCENT_OF(WINDOW_WIDTH, 0.3), PERCENT_OF(WINDOW_HEIGHT, 0.4),
		COLOR_XRGB(255, 255, 255), g_timesBigID))
	{
		return false;
	}

	/* 创建Loading的GUI */
	if(!g_Renderer->CreateGUIObject(g_GUILoadingID))
	{
		return false;
	}
	if(!g_Renderer->AddGUIBackdrop(g_GUILoadingID,"menu/loadingMenu.jpg"))
	{
		return false;
	}

	return true;
}

void RenderMenu()
{
	if (!g_Renderer) return;

	/* 启用渲染 */
	g_Renderer->StartRendering();

	/* 渲染GUI */
	switch (g_currentMenu)
	{
	case MS_MAIN_MENU:
		g_Renderer->RenderGUI(g_GUIMainMenuID, g_LMBDown, g_mousePos.x, g_mousePos.y, MenuCallback);
		break;
	case MS_START_MENU:
		g_Renderer->RenderGUI(g_GUIStartID, g_LMBDown, g_mousePos.x, g_mousePos.y, MenuCallback);
		break;
	case MS_CREDITS_MENU:
		g_Renderer->RenderGUI(g_GUICreditsID, g_LMBDown, g_mousePos.x, g_mousePos.y, MenuCallback);
		break;
	case MS_LOADING_MENU:
		g_Renderer->RenderGUI(g_GUILoadingID, g_LMBDown, g_mousePos.x, g_mousePos.y, MenuCallback);
		break;
	default:
		break;
	}

	// *文本应该在最后渲染，否则会被覆盖掉
	g_Renderer->RenderGUI(g_GUIGeneralID, g_LMBDown, g_mousePos.x, g_mousePos.y, 0);

	/* 停止渲染 */
	g_Renderer->EndRendering();
}

void MenuCallback(int id, int state)
{
	if (state == BUTTON_DOWN)
	{
		switch (id)
		{
		case BUTTON_START_ID:
			g_currentMenu = MS_START_MENU;
		break;
		case BUTTON_LEVEL_1_ID:
			//start the game.
			g_currentMenu = MS_LOADING_MENU;
			g_gameState = GS_LEVEL_SWITCH;
			break;
		case BUTTON_CREDITS_ID:
			g_currentMenu = MS_CREDITS_MENU;
			break;
		case BUTTON_BACK_ID:
			g_currentMenu = MS_MAIN_MENU;
			break;
		case BUTTON_QUIT_ID:
			ShutdownGame();
			DestroyWindow(g_hWnd);
			break;
		default:
			break;
		}
	}
}
