// Stranded Engine
// 通用物理对象 外接几何体

#pragma once

#include "STEMath.h"


/* 外接盒 */
struct BoundingBox {
	Vector3 m_min;
	Vector3 m_max;
}; // struct BoundingBox

/* 外接球 */
struct BoundingSphere {
	Vector3 m_center;
	float m_radius;
}; // struct BoundingSphere