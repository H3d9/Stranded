// Stranded Engine
// Vector 3D 接口定义文件

#include <math.h>
#include "STEMath.h"
#include "Vector.h"


// 注意：不要跨动态库返回对象。
// 详见：windows via c/c++ Ch.20

void STDCALL STEVector3Cross(Vector3* out, const Vector3* v1, const Vector3* v2) {
	out->x = (v1->y * v2->z) - (v1->z * v2->y);
	out->y = (v1->z * v2->x) - (v1->x * v2->z);
	out->z = (v1->x * v2->y) - (v1->y * v2->x);
}

float STDCALL STEVector3Dot(const Vector3* v1, const Vector3* v2) {
   return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}


float STDCALL STEVector3Length(const Vector3* v) {
   return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

// 单位化向量
void STDCALL STEVector3Normalize(Vector3* v) {
   
	float length = STEVector3Length(v);
	if(length == 0.0f) return;

	v->x = v->x / length;
	v->y = v->y / length;
	v->z = v->z / length;
}


//void Vector3::Normalize(Vector3 &v1, Vector3 &v2, Vector3 &v3)
//{
//   Vector3 e1, e2;
//
//   e1.x = v2.x - v1.x;
//   e1.y = v2.y - v1.y;
//   e1.z = v2.z - v1.z;
//   e1.Normal();
//
//   e2.x = v3.x - v1.x;
//   e2.y = v3.y - v1.y;
//   e2.z = v3.z - v1.z;
//   e2.Normal();
//
//   CrossProduct(e1, e2);
//   Normal();
//}