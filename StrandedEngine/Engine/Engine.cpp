// Stranded Engine
// STE引擎 主要接口定义

#include "Engine.h"
#include "D3DRenderer.h"
#include "DirectInput.h"
#include "CommandScript.h"


IRenderer* STDCALL STECreateRenderer() {
	return new D3DRenderer();
}

IIOSystem* STDCALL STECreateIOSystem(HWND hwnd, HINSTANCE hInst, bool exclusive) {
	return new DirectInputSystem(hwnd, hInst, exclusive);
}

IScriptSystem* STDCALL STECreateScriptSystem()
{
	return new CommandScript();
}


