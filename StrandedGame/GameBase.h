#pragma once
#include "GameDef.h"

#include "Menu.h"         // InitializeGame() �����ͷ�����ڳ�ʼ���˵�
#include "Levels.h"       // GameLoop() ��ȡ�ؿ������ͷ��

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
bool InitializeGame();    // ��ʼ����Ϸ�������ShutDownGame()���ú���ֻ���ز˵�GUI������ģ��Ҫ��LoadLevel()��
void ShutdownGame();      // �ͷ���Ϸ����Ҫ��������Դ�������˵�GUI/�����ģ�ͣ��Լ���ID��Ӧ��ȫ�ֱ���
void GameLoop();


/****************************************
 *
 *      GameLoop() SubRoutines
 *      ��̬������������cpp��
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