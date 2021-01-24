// Stranded Engine
// STE���� ��Ҫ�ӿ�����

#pragma once

// ʹ��Stranded Engineʱ��Ӧ������ͷ�ļ���
// ���⣬�û���Ҫ�ֶ�������Ŀ¼���������Ŀ¼���Թ�������������
// (��ǰĿ¼)
// ../Math
// ../Physics
// ../Camera
// ../Render
// ../IO
// ../Scripts


/* ���û������� */

#include "EngineBase.h"


/* ����ͨ�ýӿ� */

#include "STEMath.h"
#include "STEPhysics.h"
#include "STECamera.h"


/* ���ó���ӿ� */

#include "IRenderer.h"
#include "IIOSystem.h"
#include "IScriptSystem.h"

/* �������� */

STE_EXPORT_BEGIN

// �ӿڣ�CreateRenderer
// ���ܣ�������Ⱦ�������
STE_DECLARE_INTERFACE(STECreateRenderer, IRenderer*)();

// �ӿڣ�CreateIOSystem
// ���ܣ���������ϵͳ����
STE_DECLARE_INTERFACE(STECreateIOSystem, IIOSystem*)(HWND hwnd, HINSTANCE hInst, bool exclusive);

// �ӿڣ�CreateScriptSystem
// ���ܣ������ű�ϵͳ����
STE_DECLARE_INTERFACE(STECreateScriptSystem, IScriptSystem*)();


STE_EXPORT_END
