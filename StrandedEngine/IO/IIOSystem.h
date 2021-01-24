// Stranded Engine
// IOϵͳ ����ӿ�

// ˵����
// �ṩIOϵͳ�ĳ�����ࡣ
// ���ģʽ��
// factory������������Engine/Engine.h|cpp��

#pragma once 

#include "EngineBase.h"
#include "STECamera.h"
#include "STEPhysics.h"
#include "IOKeyMap.h"


class IIOSystem {
public:
    virtual ~IIOSystem() {}

public:
    virtual bool Initialize() = 0;
    virtual bool UpdateDevices() = 0;

    // ���ݾ��ӽǺ�����˲ʱת�����������ӽ�
    virtual void UpdateByMouseRotation(
        float* angleH, 
        float* angleV,
        POINT* g_mousePos, 
        POINT* g_midWindow, 
        Camera* camera, 
        Ray* viewRay) = 0;
    // �㰴S���µ�direction
    virtual void GetLeftDirection(Vector3* vec, const Camera* camera) = 0;
    // �㰴D���µ�direction
    virtual void GetRightDirection(Vector3* vec, const Camera* camera) = 0;




    virtual int KeyUp(IOKeyBoardMap key) = 0;
    virtual int KeyDown(IOKeyBoardMap key) = 0;

    virtual int MouseButtonUp(IOMouseMap button) = 0;
    virtual int MouseButtonDown(IOMouseMap button) = 0;
    virtual POINT GetMousePos() = 0;
    virtual long GetMouseWheelPos() = 0;

    virtual void Release() = 0;
}; // interface IOSystem