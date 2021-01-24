// Stranded Engine
// Ray 接口定义文件

#include "STEPhysics.h"
#include "Ray.h"


bool STDCALL STERayIntersectSphere(const Ray* ray, const Vector3* pos, float radius, float* dist) {
    Vector3 RSDir;

    float RayToSphereLength = 0.0f;
    float IntersectPoint = 0.0f;
    float SquaredPoint = 0.0f;

    // 从Ray到球心的方向向量.
    RSDir = *pos - ray->m_origin;

    // 若Ray起点在球内，则相交.
    if (STEVector3Length(&RSDir) <= radius)
        return true;

    // IntersectPoint是RSDir在Ray上的投影长度。（m_direction为单位向量）
    IntersectPoint = STEVector3Dot(&RSDir, &ray->m_direction);

    // 若Ray与方向向量成钝角，则不可能相交.
    if (IntersectPoint < 0)
        return false;

    // 对上述3个向量的模使用余弦定理.
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

