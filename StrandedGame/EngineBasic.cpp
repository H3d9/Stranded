


#include "EngineBasic.h"


IRenderer*					g_Renderer = nullptr;					// 渲染器
IIOSystem*					g_InputSystem = nullptr;				// 输入系统
IScriptSystem*				g_ScriptSystem = nullptr;				// 脚本系统

int							g_arialID = -1;
int							g_arial24ID = -1;
int							g_arial36ID = -1;
int							g_timesID = -1;
int							g_timesBigID = -1;


bool InitializeEngine()
{
	/* 创建渲染器 */
	if (NULL == (g_Renderer = STECreateRenderer())) {
		return false;
	}

	/* 初始化渲染器 */
	if (!g_Renderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, g_hWnd, GAME_FULLSCREEN)) {
		return false;
	}

	/* 创建输入系统 */
	if (NULL == (g_InputSystem = STECreateIOSystem(g_hWnd, g_hInstance, false))) {
		return false;
	}

	/* 初始化输入系统 */
	if (!g_InputSystem->Initialize()) {
		return false;
	}

	/* 创建脚本系统 */
	if (NULL == (g_ScriptSystem = STECreateScriptSystem())) {
		return false;
	}

	/* 创建Arial字体 */
	if (!g_Renderer->CreateFontObject(g_arialID, "Arial", 0, true, 18)) {
		return false;
	}

	if (!g_Renderer->CreateFontObject(g_arial24ID, "Arial", 0, true, 24)) {
		return false;
	}

	if (!g_Renderer->CreateFontObject(g_arial36ID, "Arial", 0, true, 36)) {
		return false;
	}

	/* 创建Times字体 */
	if (!g_Renderer->CreateFontObject(g_timesID, "Times New Roman", 0, false, 18)) {
		return false;
	}

	if (!g_Renderer->CreateFontObject(g_timesBigID, "Times New Roman", 0, false, 36)) {
		return false;
	}

	return true;
}

void ShutdownEngine()
{
	if (g_Renderer) {
		g_Renderer->Release();
		g_Renderer = nullptr;
	}

	if (g_InputSystem) {
		g_InputSystem->Release();
		g_InputSystem = nullptr;
	}

	if (g_ScriptSystem) {
		g_ScriptSystem->Release();
		g_ScriptSystem = nullptr;
	}
}
