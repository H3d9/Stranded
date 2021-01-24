// Stranded Engine Renderer
// XMesh ģ������

// ע�⣺���ļ�����Ϊ����ӿڣ�����¶�������ⲿ��
// ���з���Ӧ�ñ���Ⱦ��ʵ�ַ�װ��

#pragma once

#include "XStructures.h"


class CXModel {
public:
    CXModel();

    void SetDevice(LPDIRECT3DDEVICE9 device);

    bool LoadXFile(const char* fileName);

    void Update(float time, const D3DXMATRIX* matWorld); // ����֡�����ڸ���ʱ��������任����

    void Render(); // ��ȾXģ��


    void ChangeAnimationSpeed(float amt);

    void GetBoundingSphere(D3DXVECTOR3* center, float* radius);


    void Release();

private:
    void SetAnimation(unsigned int index);
    void SetupMatrices(stD3DFrameEx* inFrame, LPD3DXMATRIX parentMatrix);
    void UpdateMatrices(const D3DXFRAME* inFrame, const D3DXMATRIX* parentMatrix);
    void DrawFrame(LPD3DXFRAME frame);


private:
    IDirect3DDevice9* m_device;              // �����豸���

    D3DXFRAME* m_root;                // ģ�͵ĵ�һ֡
    ID3DXAnimationController* m_animControl;         // ģ�͵Ķ���������

    // ����
    D3DXMATRIX* m_boneMatrices;
    unsigned int               m_maxBones;

    // ��ǰmesh
    stD3DContainerEx* m_currentContainer;

    // ��������
    unsigned int               m_currentAni;
    unsigned int               m_numAnimations;
    unsigned long              m_currentTrack;
    float                      m_speed;          // �����ٶ�
    float                      m_currentTime;
    float                      m_transition;     // �����ٶ�

    // ��������
    D3DXVECTOR3                m_center;
    float                      m_radius;
};