// Stranded Engine
// 线性数学库 通用接口

// 说明：
// 提供引擎所需线性数学结构的基础定义和通用结构声明。
// 设计方式：
// 重载运算强制内联，其余方法导出为动态库接口。

#pragma once

#include "EngineBase.h"
#include "Vector.h"
#include "Matrix.h"


STE_EXPORT_BEGIN

// Vector3

// 求向量叉积
STE_DECLARE_INTERFACE(STEVector3Cross, void)(Vector3* out, const Vector3* v1, const Vector3* v2);
// 求向量点积
STE_DECLARE_INTERFACE(STEVector3Dot, float)(const Vector3* v1, const Vector3* v2);
// 求向量长度
STE_DECLARE_INTERFACE(STEVector3Length, float)(const Vector3* v);
// 对向量单位化
STE_DECLARE_INTERFACE(STEVector3Normalize, void)(Vector3* v);


// Matrix

// 生成单位矩阵
STE_DECLARE_INTERFACE(STEMatrixIdentity, void)(Matrix* m);
// 生成沿(x,y,z)平移变换的仿射矩阵
STE_DECLARE_INTERFACE(STEMatrixTranslate, void)(Matrix* m, float x, float y, float z);
// 生成仿射矩阵的逆
STE_DECLARE_INTERFACE(STEMatrixInverseTranslate, void)(Matrix* m);
// 生成绕坐标轴x/y/z旋转angle角度的旋转矩阵
STE_DECLARE_INTERFACE(STEMatrixRotateByAxis, void)(Matrix* m, double angle, int x, int y, int z);
// 对向量应用线性变换m
STE_DECLARE_INTERFACE(STEMatrixMultiplyVector3, void)(Vector3* out, const Matrix* m, const Vector3* v);
// Transforms a 3D vector by a given matrix, projecting the result back into w = 1.
// STE_DECLARE_INTERFACE(STEVector3TransformCoord, void)(Vector3* out, const Vector3* v, const Matrix* m);


STE_EXPORT_END