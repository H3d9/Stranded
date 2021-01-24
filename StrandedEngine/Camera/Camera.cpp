// Stranded Engine
// 摄影机 接口定义

#include "STECamera.h"


void STDCALL STECameraSet(
	Camera* camera,
	float x, float y, float z,
	float xv, float yv, float zv,
	float xu, float yu, float zu) {
	camera->m_pos.x = x; camera->m_pos.y = y; camera->m_pos.z = z;
	camera->m_view.x = xv; camera->m_view.y = yv; camera->m_view.z = zv;
	camera->m_up.x = xu; camera->m_up.y = yu; camera->m_up.z = zu;
}

void STDCALL STECameraMove(Camera* camera, float amount)
{
   Vector3 dir = camera->m_view - camera->m_pos;
   STEVector3Normalize(&dir);

   camera->m_pos += dir * amount;
   camera->m_view += dir * amount;
}

void STDCALL STECameraRotate(Camera* camera, float angle, float X, float Y, float Z) {
   float ca = cosf(angle);
   float sa = sinf(angle);

	Vector3 dir = camera->m_view - camera->m_pos;
	STEVector3Normalize(&dir);

	Vector3 dir2;

	dir2.x = (ca + (1 - ca) * X) * dir.x;
	dir2.x += ((1 - ca) * X * Y - Z * sa)* dir.y;
	dir2.x += ((1 - ca) * X * Z + Y * sa) * dir.z;

	dir2.y = ((1 - ca) * X * Y + Z * sa) * dir.x;
	dir2.y += (ca + (1 - ca) * Y) * dir.y;
	dir2.y += ((1 - ca) * Y * Z - X * sa) * dir.z;

	dir2.z = ((1 - ca) * X * Z - Y * sa) * dir.x;
	dir2.z += ((1 - ca) * Y * Z + X * sa) * dir.y;
	dir2.z += (ca + (1 - ca) * Z) * dir.z;

	camera->m_view = camera->m_pos + dir2;
}