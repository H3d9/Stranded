// StrandedGame  游戏主定义文件
#pragma once

// 引用平台限定api
#include <windows.h>

// 引用 Stranded Engine
#include "Engine.h"


/*****************前向引用声明*********************/
struct		GameWorld;
enum		GameState;
enum		MenuState;
enum		MissionStatus;


/*****************全局变量声明*********************/
// Base Params
extern HWND						g_hWnd;				// 游戏主窗口句柄
extern HINSTANCE				g_hInstance;		// 应用程序实例句柄
extern IRenderer*				g_Renderer;			// 渲染器
extern IIOSystem*				g_InputSystem;		// 输入系统
extern IScriptSystem*			g_ScriptSystem;		// 脚本系统

// Mouse Params
extern RECT						g_windowRect;
extern POINT					g_midWindow;
extern bool						g_LMBDown;
extern POINT					g_mousePos;

// Sound Params  //currently useless
extern int						g_menuSound;
extern int						g_shotSound;   

// Font Params   // 各种字体的ID
extern int						g_arialID;
extern int						g_arial24ID;
extern int						g_arial36ID;
extern int						g_timesID;
extern int						g_timesBigID;

// Menu Params
extern int						g_GUIGeneralID; //通用 GUI （在所有菜单中都渲染，如FPS）
extern int						g_GUIMainMenuID; //主菜单 GUI
extern int						g_GUIStartID;   //开始菜单 GUI
extern int						g_GUICreditsID;	//鸣谢 GUI
extern MenuState				g_currentMenu;	//当前菜单的状态，默认是主菜单

// Game Params
extern GameState				g_gameState;  // 当前游戏所处状态，如位于主菜单，位于关卡中等
extern Ray						g_viewRay;    // 玩家视角
extern float					angleH;       // 玩家视角的水平角度
extern float					angleV;       // 玩家视角的垂直角度
extern float					g_walkSpeed;

// Level Params
extern GameWorld				g_gameWorld;
extern Camera					g_camera;

// FPS Check
extern int						g_fps;
extern wchar_t					g_fpsBuf[20], g_mousebuf[50];
extern float					g_time;
extern float					g_lastTime;
/*************************************************/


/* Macros */

/**********************************************
 *
 *      Game Base Macros
 *
 **********************************************/
#define WINDOW_WIDTH		1920             /* 显示分辨率 */
#define WINDOW_HEIGHT		1080
#define GAME_TITLE			"Stranded"       /* 窗口标题 */
#define GAME_FULLSCREEN		1                /* 0 - false, 1 - true */
//#define GAME_MULTISAMPLE	MS_SAMPLES_4     /* 该选项移动至引擎内部渲染器的宏定义中 */

/**********************************************
*
*   GUI Control Macros ( callback used only )
*
**********************************************/
#define STATIC_TEXT_ID		1
#define DYNAMIC_TEXT_ID		2
#define BUTTON_START_ID		3
#define BUTTON_CREDITS_ID	4
#define BUTTON_QUIT_ID		5
#define BUTTON_BACK_ID		6
#define BUTTON_LEVEL_1_ID	7

/**********************************************
*
*      Calculate Macros
*
**********************************************/
#ifndef PI
#define PI (float)3.141592654
#endif
#define PERCENT_OF(a,b) (int)((a)*(b))
//#define PI  ((float) 3.141592654)

/**********************************************
*
*      Structs And Enums Definition
*
**********************************************/
enum GameState
{
	GS_MENU = 1,
	GS_LEVEL,
	GS_LEVEL_SWITCH,
	GS_MENU_SWITCH,
};

enum MenuState
{
	MS_MAIN_MENU = 1,
	MS_START_MENU,
	MS_CREDITS_MENU,
	MS_LOADING_MENU,
};
