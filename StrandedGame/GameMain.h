// StrandedGame  ��Ϸ������ͷ�ļ�
#pragma once

#include "GameDef.h"
#include "EngineBasic.h"
#include "GameBase.h"

// Base Params
extern HWND						g_hWnd;
extern HINSTANCE				g_hInstance;

// Mouse Params
extern bool						g_LMBDown;
extern POINT					g_mousePos;

// Main Functions
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
ATOM RegisterMyClass(HINSTANCE hInstance);