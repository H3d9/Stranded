// Stranded Engine
// ������ѧ�� ͨ�ýӿ�

// ˵����
// �ṩ��������������ѧ�ṹ�Ļ��������ͨ�ýṹ������
// ��Ʒ�ʽ��
// ��������ǿ�����������෽������Ϊ��̬��ӿڡ�

#pragma once

#include "EngineBase.h"
#include "Vector.h"
#include "Matrix.h"


STE_EXPORT_BEGIN

// Vector3

// ���������
STE_DECLARE_INTERFACE(STEVector3Cross, void)(Vector3* out, const Vector3* v1, const Vector3* v2);
// ���������
STE_DECLARE_INTERFACE(STEVector3Dot, float)(const Vector3* v1, const Vector3* v2);
// ����������
STE_DECLARE_INTERFACE(STEVector3Length, float)(const Vector3* v);
// ��������λ��
STE_DECLARE_INTERFACE(STEVector3Normalize, void)(Vector3* v);


// Matrix

// ���ɵ�λ����
STE_DECLARE_INTERFACE(STEMatrixIdentity, void)(Matrix* m);
// ������(x,y,z)ƽ�Ʊ任�ķ������
STE_DECLARE_INTERFACE(STEMatrixTranslate, void)(Matrix* m, float x, float y, float z);
// ���ɷ���������
STE_DECLARE_INTERFACE(STEMatrixInverseTranslate, void)(Matrix* m);
// ������������x/y/z��תangle�Ƕȵ���ת����
STE_DECLARE_INTERFACE(STEMatrixRotateByAxis, void)(Matrix* m, double angle, int x, int y, int z);
// ������Ӧ�����Ա任m
STE_DECLARE_INTERFACE(STEMatrixMultiplyVector3, void)(Vector3* out, const Matrix* m, const Vector3* v);
// Transforms a 3D vector by a given matrix, projecting the result back into w = 1.
// STE_DECLARE_INTERFACE(STEVector3TransformCoord, void)(Vector3* out, const Vector3* v, const Matrix* m);


STE_EXPORT_END