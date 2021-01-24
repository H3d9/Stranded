// Stranded Engine
// ��Ӱ�� ͨ�ýӿ�

// ˵����
// �ṩ��Ӱ���Ļ��������ͨ�ýṹ������
// ��Ʒ�ʽ��
// ��������ǿ�����������෽������Ϊ��̬��ӿڡ�

#pragma once

#include "EngineBase.h"
#include "STEMath.h"


struct Camera {
    Vector3 m_pos;
    Vector3 m_view;
    Vector3 m_up;
}; // struct Camera


STE_EXPORT_BEGIN

STE_DECLARE_INTERFACE(STECameraSet, void)(
	Camera* camera,
	float x, float y, float z,
	float xv, float yv, float zv,
	float xu, float yu, float zu);
STE_DECLARE_INTERFACE(STECameraMove, void)(
	Camera* camera, 
	float amount);
STE_DECLARE_INTERFACE(STECameraRotate, void)(
	Camera* camera, 
	float angle, 
	float X, 
	float Y, 
	float Z);

STE_EXPORT_END