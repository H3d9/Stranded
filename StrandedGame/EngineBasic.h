#pragma once
#include "GameDef.h"

// Base Params
extern IRenderer*				g_Renderer;			// 渲染器
extern IIOSystem*				g_InputSystem;		// 输入系统
extern IScriptSystem*			g_ScriptSystem;		// 脚本系统

// Font Params
extern int							g_arialID;
extern int							g_arial24ID;
extern int							g_arial36ID; 
extern int							g_timesID;
extern int							g_timesBigID;

// Sound Params
extern int							g_menuSound;
extern int							g_shotSound;  //currently useless

/**********************************************
 *
 *      Engine Base
 *
 **********************************************/
bool InitializeEngine();
void ShutdownEngine();