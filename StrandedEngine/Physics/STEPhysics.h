// Stranded Engine
// 物理模拟库 通用接口

// 说明：
// 提供物理库的基础结构定义和通用接口声明。
// 设计方式：
// 重载运算强制内联，其余方法导出为动态库接口。

#pragma once

#include "STEMath.h"
#include "Ray.h"
#include "Bounding.h"


STE_EXPORT_BEGIN

// Ray

// 给出射线ray与球心在pos、半径radius的球体是否相交。如dist!=NULL，则给出距离。
STE_DECLARE_INTERFACE(STERayIntersectSphere, bool)(const Ray* ray, const Vector3* pos, float radius, float* dist);


// Bounding

STE_DECLARE_INTERFACE(STEBoundingBoxPointIsInside, bool)(const BoundingBox* box, const Vector3* v);
STE_DECLARE_INTERFACE(STEBoundingSpherePointIsInside, bool)(const BoundingSphere* sphere, const Vector3* v);

STE_EXPORT_END