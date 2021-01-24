#include "GameBase.h"
#include "EngineBasic.h"
#include <stdio.h>

// Game Params
GameState					g_gameState = GS_MENU;			//��Ϸ״̬
Ray							g_viewRay;						//���߷���
float						angleH = 0.5f * PI;				//�ӽǵ�ˮƽ�Ƕ�
float						angleV = 0.0f;					//�ӽǵĴ�ֱ�Ƕ�
float				        g_walkSpeed = 0.1f;

// FPS Check Params
int							g_fps = 0;
wchar_t						g_fpsBuf[20] = { 0 };
wchar_t						g_mousebuf[50] = { 0 };
float						g_time = GetTickCount() * 0.001f;
float						g_lastTime = GetTickCount() * 0.001f;

// ��ʼ��Game
bool InitializeGame()
{
	/* ��ʼ���˵�GUI */
	if (!InitializeGeneralMenu())
	{
		return false;
	}

	if (!InitializeMenu())
	{
		return false;
	}

	/* ��ʼ����Ļ����λ�� */
	GetWindowRect(g_hWnd, &g_windowRect);

	g_midWindow.x = (g_windowRect.left + g_windowRect.right) / 2;
	g_midWindow.y = (g_windowRect.top + g_windowRect.bottom) / 2;

	return true;
}

// �ͷ�Game
void ShutdownGame()
{
	if (!g_Renderer) return;

	g_Renderer->ReleaseXModels();
	g_Renderer->ReleaseGUIs();
	//ע�⣺���ﲻ���ͷ����壬�������ͷ���Ⱦ������

	g_gameWorld.Shutdown();

	g_GUIMainMenuID = -1;
	g_GUIStartID = -1;
	g_GUICreditsID = -1;
	g_GUILoadingID = -1;
	g_currentMenu = MS_MAIN_MENU;
}

// ��Ϸ��ѭ��
void GameLoop()
{
	GameProcessInput();
	//CalcFPS();

	/* ����GameState����Ⱦ���� */
	switch (g_gameState)
	{
	case GS_LEVEL:
		RenderLevel();
		break;
	case GS_MENU:
		RenderMenu();
		break;
	case GS_LEVEL_SWITCH: //�˵���ؿ��л����м�̬
		{
			RenderMenu();

			ShutdownGame();

			if (LoadLevel("maps/level1/level1.lvl"))
			{
				g_gameState = GS_LEVEL;
				SetCursorPos(g_midWindow.x, g_midWindow.y); //����ؿ�ʱ�����ŵ���Ļ����
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
	case GS_MENU_SWITCH: //�ؿ���˵��л����м�̬
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

	/* �������Ϸ�ڣ����ж���û�г�ʱ */
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

/* ��Ϸ��ѭ�� - �ӹ��� */
void GameProcessInput()
{
	if (!g_InputSystem) return;

	/* ���������豸״̬ */
	g_InputSystem->UpdateDevices();

	/* ��ȡ���״̬ */
	g_LMBDown = g_InputSystem->MouseButtonDown(MOUSE_LB) != 0;

	/* ����Esc���� */
	
	if (g_InputSystem->KeyUp(KBD_ESCAPE))
	{
		if (g_gameState == GS_MENU) // λ�ڲ˵�ʱ
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
		else if (g_gameState == GS_LEVEL) // λ�ڹؿ���ʱ
		{
			g_gameState = GS_MENU_SWITCH;
		}
	}

	/* �������ؿ����������ת��������ؿ�ʱ�����������Ļ���ģ� */
	if (g_gameState == GS_LEVEL)
	{
		g_InputSystem->UpdateByMouseRotation( // ע�⣺����Թ���
			&angleH, &angleV, 
			&g_mousePos, &g_midWindow, 
			&g_camera, &g_viewRay);

		/* ������갴�� */
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


	/* ����������� */
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
//	swprintf_s(g_mousebuf, 50, L"���λ��:(%d,%d)", g_mousePos.x, g_mousePos.y);
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

	/* ��ײ��� */
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
	/* ���˼�� */
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

	/* ��ײ��� */
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
	/* ���˼�� */
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


