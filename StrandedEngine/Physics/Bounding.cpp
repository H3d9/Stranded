// Stranded Engine
// 外接几何体 接口定义文件

// 说明：裁剪了未用到的函数
// 注意：msvc使用c/cpp文件作为翻译单元。
// 必须在cpp中引用导出声明的头文件，否则导出声明将被编译器忽略。

#include "STEPhysics.h"
#include "Bounding.h"


bool STDCALL STEBoundingBoxPointIsInside(const BoundingBox* box, const Vector3* v) {
	if (box->m_max.x <= v->x) return false;
	if (box->m_min.x >= v->x) return false;
	if (box->m_max.y <= v->y) return false;
	if (box->m_min.y >= v->y) return false;
	if (box->m_max.z <= v->z) return false;
	if (box->m_min.z >= v->z) return false;

	return true;
}

bool STDCALL STEBoundingSpherePointIsInside(const BoundingSphere* sphere, const Vector3* v)
{
	// 求点距离球心是否超出球的半径。
	Vector3 intersect = sphere->m_center - *v;

	if (sqrtf(
			intersect.x * intersect.x 
			+ intersect.y * intersect.y 
			+ intersect.z * intersect.z)
		< sphere->m_radius)
		return true;

	return false;
}
