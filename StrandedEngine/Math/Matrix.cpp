// Stranded Engine
// Matrix 接口定义文件

#include <math.h>
#include "STEMath.h"
#include "Matrix.h"

#define PI		D3DX_PI


void STDCALL STEMatrixIdentity(Matrix* m) {
	m->matrix[0] = 1.0f; m->matrix[1] = 0.0f; m->matrix[2] = 0.0f; m->matrix[3] = 0.0f;
	m->matrix[4] = 0.0f; m->matrix[5] = 1.0f; m->matrix[6] = 0.0f; m->matrix[7] = 0.0f;
	m->matrix[8] = 0.0f; m->matrix[9] = 0.0f; m->matrix[10] = 1.0f; m->matrix[11] = 0.0f;
	m->matrix[12] = 0.0f; m->matrix[13] = 0.0f; m->matrix[14] = 0.0f; m->matrix[15] = 1.0f;
}

void STDCALL STEMatrixTranslate(Matrix* m, float x, float y, float z) {
		m->matrix[12] = x;
		m->matrix[13] = y;
		m->matrix[14] = z;
		m->matrix[15] = 1.0f;
}

void STDCALL STEMatrixInverseTranslate(Matrix* m) {
	m->matrix[12] = -m->matrix[12];
	m->matrix[13] = -m->matrix[13];
	m->matrix[14] = -m->matrix[14];
}


void STDCALL STEMatrixRotateByAxis(Matrix* m, double angle, int x, int y, int z) {
	angle = angle * PI / 180;
	float cosAngle = cosf((float)angle);
	float sineAngle = sinf((float)angle);

	if (z)
	{
		m->matrix[0] = cosAngle;
		m->matrix[1] = sineAngle;
		m->matrix[4] = -sineAngle;
		m->matrix[5] = cosAngle;
	}

	if (y)
	{
		m->matrix[0] = cosAngle;
		m->matrix[2] = -sineAngle;
		m->matrix[8] = sineAngle;
		m->matrix[10] = cosAngle;
	}

	if (x)
	{
		m->matrix[5] = cosAngle;
		m->matrix[6] = sineAngle;
		m->matrix[9] = -sineAngle;
		m->matrix[10] = cosAngle;
	}
}

void STDCALL STEMatrixMultiplyVector3(Vector3* out, const Matrix* m, const Vector3* v) {
	// 设置中间变量result，以防out与v是同一引用。
	Vector3 result;

	result.x = (v->x * m->matrix[0]) + (v->y * m->matrix[4]) + (v->z * m->matrix[8]) + m->matrix[12];
	result.y = (v->x * m->matrix[1]) + (v->y * m->matrix[5]) + (v->z * m->matrix[9]) + m->matrix[13];
	result.z = (v->x * m->matrix[2]) + (v->y * m->matrix[6]) + (v->z * m->matrix[10]) + m->matrix[14];
	
	out->x = result.x;
	out->y = result.y;
	out->z = result.z;
}

//void STDCALL STEVector3TransformCoord(Vector3* out, const Vector3* v, const Matrix* m) {
//	D3DXVec3TransformCoord(
//		(D3DXVECTOR3*)out, 
//		(D3DXVECTOR3*)v, 
//		(D3DXMATRIX*)m);
//}


//Vector3 Matrix::VectorMatrixMultiply3x3(Vector3& v) {
//	Vector3 out;
//
//	out.x = (v.x * matrix[0]) + (v.y * matrix[4]) + (v.z * matrix[8]);
//	out.y = (v.x * matrix[1]) + (v.y * matrix[5]) + (v.z * matrix[9]);
//	out.z = (v.x * matrix[2]) + (v.y * matrix[6]) + (v.z * matrix[10]);
//
//	return out;
//}