#pragma once
#include "GameDef.h"

#include "Menu.h"         // InitializeGame() 所需的头。用于初始化菜单
#include "Levels.h"       // GameLoop() 读取关卡所需的头。

// Game Params
extern GameState					g_gameState;
extern Ray							g_viewRay;
extern float						angleH;
extern float						angleV;
extern float		                g_walkSpeed;

// FPS Check Params
//extern int							g_fps;
//extern wchar_t						g_fpsBuf[20];
extern wchar_t						g_mousebuf[50];
extern float						g_time;
extern float						g_lastTime;


/**********************************************
 *
 *      Game Base Exports
 *
 **********************************************/
bool InitializeGame();    // 初始化游戏；相比于ShutDownGame()，该函数只加载菜单GUI。载入模型要用LoadLevel()。
void ShutdownGame();      // 释放游戏中需要的所有资源，包括菜单GUI/载入的模型，以及其ID对应的全局变量
void GameLoop();


/****************************************
 *
 *      GameLoop() SubRoutines
 *      静态函数，声明到cpp中
 *
 ****************************************/
void GameProcessInput();
//void CalcFPS();


/****************************************
 *
 *    GameProcessInput() SubRoutines
 *
 ****************************************/
void GoAndCheckCollisions(bool goAhead);
void GoAndCheckCollisions(Vector3* Direction);