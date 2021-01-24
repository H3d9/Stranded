// Stranded Engine
// STE引擎 主要接口声明

#pragma once

// 使用Stranded Engine时，应包含此头文件。
// 此外，用户需要手动将以下目录添加至包含目录，以供编译器索引：
// (当前目录)
// ../Math
// ../Physics
// ../Camera
// ../Render
// ../IO
// ../Scripts


/* 引用基础定义 */

#include "EngineBase.h"


/* 引用通用接口 */

#include "STEMath.h"
#include "STEPhysics.h"
#include "STECamera.h"


/* 引用抽象接口 */

#include "IRenderer.h"
#include "IIOSystem.h"
#include "IScriptSystem.h"

/* 工厂函数 */

STE_EXPORT_BEGIN

// 接口：CreateRenderer
// 功能：创建渲染器类对象
STE_DECLARE_INTERFACE(STECreateRenderer, IRenderer*)();

// 接口：CreateIOSystem
// 功能：创建输入系统对象
STE_DECLARE_INTERFACE(STECreateIOSystem, IIOSystem*)(HWND hwnd, HINSTANCE hInst, bool exclusive);

// 接口：CreateScriptSystem
// 功能：创建脚本系统对象
STE_DECLARE_INTERFACE(STECreateScriptSystem, IScriptSystem*)();


STE_EXPORT_END
