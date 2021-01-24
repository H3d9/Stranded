// StrandedGame  ��Ϸ�������ļ�
#pragma once

// ����ƽ̨�޶�api
#include <windows.h>

// ���� Stranded Engine
#include "Engine.h"


/*****************ǰ����������*********************/
struct		GameWorld;
enum		GameState;
enum		MenuState;
enum		MissionStatus;


/*****************ȫ�ֱ�������*********************/
// Base Params
extern HWND						g_hWnd;				// ��Ϸ�����ھ��
extern HINSTANCE				g_hInstance;		// Ӧ�ó���ʵ�����
extern IRenderer*				g_Renderer;			// ��Ⱦ��
extern IIOSystem*				g_InputSystem;		// ����ϵͳ
extern IScriptSystem*			g_ScriptSystem;		// �ű�ϵͳ

// Mouse Params
extern RECT						g_windowRect;
extern POINT					g_midWindow;
extern bool						g_LMBDown;
extern POINT					g_mousePos;

// Sound Params  //currently useless
extern int						g_menuSound;
extern int						g_shotSound;   

// Font Params   // ���������ID
extern int						g_arialID;
extern int						g_arial24ID;
extern int						g_arial36ID;
extern int						g_timesID;
extern int						g_timesBigID;

// Menu Params
extern int						g_GUIGeneralID; //ͨ�� GUI �������в˵��ж���Ⱦ����FPS��
extern int						g_GUIMainMenuID; //���˵� GUI
extern int						g_GUIStartID;   //��ʼ�˵� GUI
extern int						g_GUICreditsID;	//��л GUI
extern MenuState				g_currentMenu;	//��ǰ�˵���״̬��Ĭ�������˵�

// Game Params
extern GameState				g_gameState;  // ��ǰ��Ϸ����״̬����λ�����˵���λ�ڹؿ��е�
extern Ray						g_viewRay;    // ����ӽ�
extern float					angleH;       // ����ӽǵ�ˮƽ�Ƕ�
extern float					angleV;       // ����ӽǵĴ�ֱ�Ƕ�
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
#define WINDOW_WIDTH		1920             /* ��ʾ�ֱ��� */
#define WINDOW_HEIGHT		1080
#define GAME_TITLE			"Stranded"       /* ���ڱ��� */
#define GAME_FULLSCREEN		1                /* 0 - false, 1 - true */
//#define GAME_MULTISAMPLE	MS_SAMPLES_4     /* ��ѡ���ƶ��������ڲ���Ⱦ���ĺ궨���� */

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
