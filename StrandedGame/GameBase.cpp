#include "GameBase.h"
#include "EngineBasic.h"
#include <stdio.h>

// Game Params
GameState					g_gameState = GS_MENU;			//游戏状态
Ray							g_viewRay;						//视线方向
float						angleH = 0.5f * PI;				//视角的水平角度
float						angleV = 0.0f;					//视角的垂直角度
float				        g_walkSpeed = 0.1f;

// FPS Check Params
int							g_fps = 0;
wchar_t						g_fpsBuf[20] = { 0 };
wchar_t						g_mousebuf[50] = { 0 };
float						g_time = GetTickCount() * 0.001f;
float						g_lastTime = GetTickCount() * 0.001f;

// 初始化Game
bool InitializeGame()
{
	/* 初始化菜单GUI */
	if (!InitializeGeneralMenu())
	{
		return false;
	}

	if (!InitializeMenu())
	{
		return false;
	}

	/* 初始化屏幕中心位置 */
	GetWindowRect(g_hWnd, &g_windowRect);

	g_midWindow.x = (g_windowRect.left + g_windowRect.right) / 2;
	g_midWindow.y = (g_windowRect.top + g_windowRect.bottom) / 2;

	return true;
}

// 释放Game
void ShutdownGame()
{
	if (!g_Renderer) return;

	g_Renderer->ReleaseXModels();
	g_Renderer->ReleaseGUIs();
	//注意：这里不可释放字体，更不可释放渲染器对象。

	g_gameWorld.Shutdown();

	g_GUIMainMenuID = -1;
	g_GUIStartID = -1;
	g_GUICreditsID = -1;
	g_GUILoadingID = -1;
	g_currentMenu = MS_MAIN_MENU;
}

// 游戏主循环
void GameLoop()
{
	GameProcessInput();
	//CalcFPS();

	/* 根据GameState来渲染场景 */
	switch (g_gameState)
	{
	case GS_LEVEL:
		RenderLevel();
		break;
	case GS_MENU:
		RenderMenu();
		break;
	case GS_LEVEL_SWITCH: //菜单与关卡切换的中间态
		{
			RenderMenu();

			ShutdownGame();

			if (LoadLevel("maps/level1/level1.lvl"))
			{
				g_gameState = GS_LEVEL;
				SetCursorPos(g_midWindow.x, g_midWindow.y); //进入关卡时将鼠标放到屏幕中心
				ShowCursor(false);
			}
			else
			{
				if (InitializeGame())
					g_gameState = GS_MENU;
				else
				{
					MessageBox(0, "InitializeGame() - failed", 0, 0);
					g_gameState = GS_LEVEL;
				}
			}
		}
		break;
	case GS_MENU_SWITCH: //关卡与菜单切换的中间态
		{
			ShowCursor(true);
			ShutdownGame();

			if (InitializeGame())
			{
				g_currentMenu = MS_MAIN_MENU;
				g_gameState = GS_MENU;
			}
			else
			{
				MessageBox(0, "InitializeGame() - failed", 0, 0);
				g_gameState = GS_LEVEL;
			}
		}
		break;
	}

	/* 如果在游戏内，则判断有没有超时 */
	if(g_gameState == GS_LEVEL)
	{
		auto timePassed = g_gameWorld.m_startTime - g_gameWorld.m_currentTime;
		
		if(timePassed >= g_gameWorld.m_totalTime)
			g_gameWorld.m_missionStatus = MISSION_TIMEUP;
		if (g_gameWorld.m_totalCollectedArtifacts >= g_gameWorld.m_totalArtifacts)
			g_gameWorld.m_missionStatus = MISSION_COMPLETE;
		if (g_gameWorld.m_spottedCount >= 1)
			g_gameWorld.m_missionStatus = MISSION_FAILED;
	}
}

/* 游戏主循环 - 子过程 */
void GameProcessInput()
{
	if (!g_InputSystem) return;

	/* 更新输入设备状态 */
	g_InputSystem->UpdateDevices();

	/* 读取鼠标状态 */
	g_LMBDown = g_InputSystem->MouseButtonDown(MOUSE_LB) != 0;

	/* 处理Esc按键 */
	
	if (g_InputSystem->KeyUp(KBD_ESCAPE))
	{
		if (g_gameState == GS_MENU) // 位于菜单时
		{
			switch (g_currentMenu)
			{
			case MS_MAIN_MENU:
				DestroyWindow(g_hWnd);
				break;
			default:
				g_currentMenu = MS_MAIN_MENU;
				break;
			}
		}
		else if (g_gameState == GS_LEVEL) // 位于关卡中时
		{
			g_gameState = GS_MENU_SWITCH;
		}
	}

	/* 如果进入关卡，处理鼠标转动（进入关卡时将鼠标置于屏幕中心） */
	if (g_gameState == GS_LEVEL)
	{
		g_InputSystem->UpdateByMouseRotation( // 注意：耦合性过高
			&angleH, &angleV, 
			&g_mousePos, &g_midWindow, 
			&g_camera, &g_viewRay);

		/* 处理鼠标按键 */
		if (g_InputSystem->MouseButtonDown(MOUSE_LB))
		{
			//g_SoundSystem->Play(g_shotSound);
			for (int i = 0; i < g_gameWorld.m_totalCharacters; i++)
			{
				if (g_gameWorld.m_characters[i].m_visible)
				{
					if (CollisionTestCharacter(i, g_viewRay))
					{
						g_gameWorld.m_characters[i].m_visible = 0;
						g_gameWorld.m_enemyKilled++;
					}
				}
			}
		}
	}


	/* 处理键盘输入 */
	if (g_gameState == GS_LEVEL && g_gameWorld.m_missionStatus == MISSION_ONGOING)
	{
		if (g_InputSystem->KeyDown(KBD_W))
		{
			GoAndCheckCollisions(true);
		}
		if (g_InputSystem->KeyDown(KBD_S))
		{
			GoAndCheckCollisions(false);
		}

		if (g_InputSystem->KeyDown(KBD_A))
		{
			Vector3 dir;
			g_InputSystem->GetLeftDirection(&dir, &g_camera);
			GoAndCheckCollisions(&dir);
		}

		if (g_InputSystem->KeyDown(KBD_D))
		{
			Vector3 dir;
			g_InputSystem->GetRightDirection(&dir, &g_camera);
			GoAndCheckCollisions(&dir);
		}
	}
}

//void CalcFPS()
//{
//	swprintf_s(g_mousebuf, 50, L"鼠标位置:(%d,%d)", g_mousePos.x, g_mousePos.y);
//	
//	g_time = GetTickCount() * 0.001f;
//	if (g_time - g_lastTime > 1.0f)
//	{
//		swprintf_s(g_fpsBuf, 20, L"FPS:%d", g_fps);
//		g_lastTime = g_time;
//		g_fps = 0;
//	}
//	else
//	{
//		g_fps++;
//	}
//}



void GoAndCheckCollisions(bool goAhead)
{
	auto d = goAhead ? 1.0f : -1.0f;

	Vector3 Direction =
		Vector3(g_camera.m_view.x, 0.0f, g_camera.m_view.z) -
		Vector3(g_camera.m_pos.x, 0.0f, g_camera.m_pos.z);

	g_camera.m_pos.x += g_walkSpeed * Direction.x * d;
	g_camera.m_pos.z += g_walkSpeed * Direction.z * d;

	g_camera.m_view.x += g_walkSpeed * Direction.x * d;
	g_camera.m_view.z += g_walkSpeed * Direction.z * d;

	/* 碰撞检测 */
	for (auto i = 0; i < g_gameWorld.m_totalStaticModels; i++)
	{
		if (CollisionTestStatic(i))
		{
			g_camera.m_pos.x -= g_walkSpeed * Direction.x * d;
			g_camera.m_pos.z -= g_walkSpeed * Direction.z * d;

			g_camera.m_view.x -= g_walkSpeed * Direction.x * d;
			g_camera.m_view.z -= g_walkSpeed * Direction.z * d;
			break;
		}
	}
	/* 敌人检测 */
	for (auto i = 0; i < g_gameWorld.m_totalCharacters; i++)
	{
		if (CharacterVisCheck(i))
		{
			if (!g_gameWorld.m_characterSpot[i])
				g_gameWorld.m_spottedCount++;
			g_gameWorld.m_characterSpot[i] = 1;
		}

		if (CollisionTestCharacter(i))
		{
			g_camera.m_pos.x -= g_walkSpeed * Direction.x * d;
			g_camera.m_pos.z -= g_walkSpeed * Direction.z * d;

			g_camera.m_view.x -= g_walkSpeed * Direction.x * d;
			g_camera.m_view.z -= g_walkSpeed * Direction.z * d;
			break;
		}
	}
}

void GoAndCheckCollisions(Vector3* Direction)
{

	g_camera.m_pos.x += g_walkSpeed * Direction->x;
	g_camera.m_pos.z += g_walkSpeed * Direction->z;

	g_camera.m_view.x += g_walkSpeed * Direction->x;
	g_camera.m_view.z += g_walkSpeed * Direction->z;

	/* 碰撞检测 */
	for (auto i = 0; i < g_gameWorld.m_totalStaticModels; i++)
	{
		if (CollisionTestStatic(i))
		{
			g_camera.m_pos.x -= g_walkSpeed * Direction->x;
			g_camera.m_pos.z -= g_walkSpeed * Direction->z;

			g_camera.m_view.x -= g_walkSpeed * Direction->x;
			g_camera.m_view.z -= g_walkSpeed * Direction->z;
			break;
		}
	}
	/* 敌人检测 */
	for (auto i = 0; i < g_gameWorld.m_totalCharacters; i++)
	{
		if (CharacterVisCheck(i))
		{
			if (!g_gameWorld.m_characterSpot[i])
				g_gameWorld.m_spottedCount++;
			g_gameWorld.m_characterSpot[i] = 1;
		}

		if (CollisionTestCharacter(i))
		{
			g_camera.m_pos.x -= g_walkSpeed * Direction->x;
			g_camera.m_pos.z -= g_walkSpeed * Direction->z;

			g_camera.m_view.x -= g_walkSpeed * Direction->x;
			g_camera.m_view.z -= g_walkSpeed * Direction->z;
			break;
		}
	}
}


