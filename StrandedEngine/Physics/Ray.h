// Stranded Engine
// 通用物理对象 Ray

#pragma once

#include "STEMath.h"


struct Ray {
    Vector3 m_origin;           // 原点坐标。
    Vector3 m_direction;        // 方向向量。使用者应保证该向量为单位向量。
}; // struct Ray
