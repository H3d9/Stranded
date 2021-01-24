// Stranded Engine
// Ray �ӿڶ����ļ�

#include "STEPhysics.h"
#include "Ray.h"


bool STDCALL STERayIntersectSphere(const Ray* ray, const Vector3* pos, float radius, float* dist) {
    Vector3 RSDir;

    float RayToSphereLength = 0.0f;
    float IntersectPoint = 0.0f;
    float SquaredPoint = 0.0f;

    // ��Ray�����ĵķ�������.
    RSDir = *pos - ray->m_origin;

    // ��Ray��������ڣ����ཻ.
    if (STEVector3Length(&RSDir) <= radius)
        return true;

    // IntersectPoint��RSDir��Ray�ϵ�ͶӰ���ȡ���m_directionΪ��λ������
    IntersectPoint = STEVector3Dot(&RSDir, &ray->m_direction);

    // ��Ray�뷽�������ɶ۽ǣ��򲻿����ཻ.
    if (IntersectPoint < 0)
        return false;

    // ������3��������ģʹ�����Ҷ���.
    SquaredPoint =
        (radius * radius)
        - STEVector3Dot(&RSDir, &RSDir)
        + (IntersectPoint * IntersectPoint);

    if (SquaredPoint < 0)
        return false;

    if (dist)
        *dist = IntersectPoint - (float)sqrt(SquaredPoint);

    return true;
}

