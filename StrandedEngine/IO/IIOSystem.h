// Stranded Engine
// IO系统 抽象接口

// 说明：
// 提供IO系统的抽象基类。
// 设计模式：
// factory。工厂定义在Engine/Engine.h|cpp。

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

    // 依据旧视角和鼠标的瞬时转动，重设新视角
    virtual void UpdateByMouseRotation(
        float* angleH, 
        float* angleV,
        POINT* g_mousePos, 
        POINT* g_midWindow, 
        Camera* camera, 
        Ray* viewRay) = 0;
    // 点按S后新的direction
    virtual void GetLeftDirection(Vector3* vec, const Camera* camera) = 0;
    // 点按D后新的direction
    virtual void GetRightDirection(Vector3* vec, const Camera* camera) = 0;




    virtual int KeyUp(IOKeyBoardMap key) = 0;
    virtual int KeyDown(IOKeyBoardMap key) = 0;

    virtual int MouseButtonUp(IOMouseMap button) = 0;
    virtual int MouseButtonDown(IOMouseMap button) = 0;
    virtual POINT GetMousePos() = 0;
    virtual long GetMouseWheelPos() = 0;

    virtual void Release() = 0;
}; // interface IOSystem