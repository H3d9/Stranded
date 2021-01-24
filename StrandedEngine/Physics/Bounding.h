// Stranded Engine
// ͨ��������� ��Ӽ�����

#pragma once

#include "STEMath.h"


/* ��Ӻ� */
struct BoundingBox {
	Vector3 m_min;
	Vector3 m_max;
}; // struct BoundingBox

/* ����� */
struct BoundingSphere {
	Vector3 m_center;
	float m_radius;
}; // struct BoundingSphere