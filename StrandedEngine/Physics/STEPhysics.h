// Stranded Engine
// ����ģ��� ͨ�ýӿ�

// ˵����
// �ṩ�����Ļ����ṹ�����ͨ�ýӿ�������
// ��Ʒ�ʽ��
// ��������ǿ�����������෽������Ϊ��̬��ӿڡ�

#pragma once

#include "STEMath.h"
#include "Ray.h"
#include "Bounding.h"


STE_EXPORT_BEGIN

// Ray

// ��������ray��������pos���뾶radius�������Ƿ��ཻ����dist!=NULL����������롣
STE_DECLARE_INTERFACE(STERayIntersectSphere, bool)(const Ray* ray, const Vector3* pos, float radius, float* dist);


// Bounding

STE_DECLARE_INTERFACE(STEBoundingBoxPointIsInside, bool)(const BoundingBox* box, const Vector3* v);
STE_DECLARE_INTERFACE(STEBoundingSpherePointIsInside, bool)(const BoundingSphere* sphere, const Vector3* v);

STE_EXPORT_END