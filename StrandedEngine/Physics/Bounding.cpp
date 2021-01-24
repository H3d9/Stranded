// Stranded Engine
// ��Ӽ����� �ӿڶ����ļ�

// ˵�����ü���δ�õ��ĺ���
// ע�⣺msvcʹ��c/cpp�ļ���Ϊ���뵥Ԫ��
// ������cpp�����õ���������ͷ�ļ������򵼳������������������ԡ�

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
	// �����������Ƿ񳬳���İ뾶��
	Vector3 intersect = sphere->m_center - *v;

	if (sqrtf(
			intersect.x * intersect.x 
			+ intersect.y * intersect.y 
			+ intersect.z * intersect.z)
		< sphere->m_radius)
		return true;

	return false;
}
