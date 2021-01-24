// StrandedGame  ��Ϸ������
#include <Windows.h>
#include <windowsx.h>
#include "GameMain.h"

/*********************ȫ�ֱ���***********************/
// Base Params
HWND						g_hWnd			= nullptr;
HINSTANCE					g_hInstance		= nullptr;

// Mouse Params
bool						g_LMBDown		= false;
RECT						g_windowRect;
POINT						g_midWindow		= { 0,0 };
POINT						g_mousePos		= { 0,0 };
/****************************************************/

// Main Functions
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  // ����������ٴ��ں����˳���Ϣ���Ͳ����ͷ���Ϸ��Դ��������Դ��
		break;
	case WM_MOUSEMOVE:
		if (g_gameState != GS_LEVEL)
		{
			g_mousePos.x = GET_X_LPARAM(lParam);
			g_mousePos.y = GET_Y_LPARAM(lParam);
		}
		break;
	default: 
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

ATOM RegisterMyClass(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = static_cast<WNDPROC>(WndProc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = "StrandedGame";

	return RegisterClass(&wc);
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	// ע�ᴰ����
	if (!RegisterMyClass(hInstance))
	{
		MessageBox(nullptr, "RegisterClass() - FAILED", nullptr, 0);
		return 0;
	}

	// ��������
	g_hWnd = CreateWindow("StrandedGame",
		GAME_TITLE,
		WS_EX_TOPMOST,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
		nullptr /*parent hwnd*/, nullptr /* menu */, hInstance, nullptr /*extra*/);

	if (!g_hWnd)
	{
		MessageBox(nullptr, "CreateWindow() - FAILED", nullptr, 0);
		return 0;
	}

	g_hInstance = hInstance;

	// ��ʾ�����´���
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	// ��ʼ����Ϸ
	if (!InitializeEngine())
	{
		MessageBox(nullptr, "InitializeEngine() - FAILED", nullptr, 0);
		return 0;
	}
	if (!InitializeGame())
	{
		MessageBox(nullptr, "InitializeGame() - FAILED", nullptr, 0);
		return 0;
	}


	// ����Ϣѭ��
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameLoop();
		}
	}

	// �ͷ���Ϸ��Դ
	ShutdownGame();
	ShutdownEngine();

	return msg.wParam;
}