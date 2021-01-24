// Stranded Engine Renderer
// XMesh 模块声明

// 注意：该文件不作为对外接口，不暴露至引擎外部。
// 所有方法应该被渲染器实现封装。

#pragma once

#include "XStructures.h"


class CXModel {
public:
    CXModel();

    void SetDevice(LPDIRECT3DDEVICE9 device);

    bool LoadXFile(const char* fileName);

    void Update(float time, const D3DXMATRIX* matWorld); // 计算帧动画在给定时间后的世界变换矩阵

    void Render(); // 渲染X模型


    void ChangeAnimationSpeed(float amt);

    void GetBoundingSphere(D3DXVECTOR3* center, float* radius);


    void Release();

private:
    void SetAnimation(unsigned int index);
    void SetupMatrices(stD3DFrameEx* inFrame, LPD3DXMATRIX parentMatrix);
    void UpdateMatrices(const D3DXFRAME* inFrame, const D3DXMATRIX* parentMatrix);
    void DrawFrame(LPD3DXFRAME frame);


private:
    IDirect3DDevice9* m_device;              // 虚拟设备句柄

    D3DXFRAME* m_root;                // 模型的第一帧
    ID3DXAnimationController* m_animControl;         // 模型的动画控制器

    // 骨骼
    D3DXMATRIX* m_boneMatrices;
    unsigned int               m_maxBones;

    // 当前mesh
    stD3DContainerEx* m_currentContainer;

    // 动画参数
    unsigned int               m_currentAni;
    unsigned int               m_numAnimations;
    unsigned long              m_currentTrack;
    float                      m_speed;          // 动画速度
    float                      m_currentTime;
    float                      m_transition;     // 过渡速度

    // 外接球参数
    D3DXVECTOR3                m_center;
    float                      m_radius;
};