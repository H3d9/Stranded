// Stranded Engine 
// STE���� ��������

#pragma once

#ifndef STE_IMPLEMENTATION
#define	STE_API				_declspec(dllimport)
#else
#define	STE_API				_declspec(dllexport)

#include <d3d9.h>
#include <d3dx9.h>
#endif // !STE_IMPLEMENTATION


// windows ƽ̨�޶�
#include <Windows.h>


// x86_32 ƽ̨�޶�
#define	STDCALL				__stdcall


// �������� �޶�
#define	STE_EXPORT_BEGIN	extern "C" {
#define	STE_EXPORT_END		}

#define STE_DECLARE_INTERFACE(FunctionName, ReturnValue)	STE_API ReturnValue STDCALL FunctionName


/* ��Ⱦ */
#define SAMPLE_TYPE			D3DMULTISAMPLE_4_SAMPLES

// ��Ⱦ״̬
enum TextureState  //��ͼ״̬
{
	MIN_FILTER,
	MAG_FILTER,
	MIP_FILTER
};

enum FilterType   //��ͼ����״̬
{
	FILTER_POINT,
	FILTER_LINEAR,
	FILTER_ANISOTROPIC
};


/* I/Oӳ�� */
#include "IOKeyMap.h"

/* GUI */
// GUI ״̬
#define GUI_DYNAMICTEXT		1
#define GUI_STATICTEXT		2
#define GUI_BUTTON			3
#define GUI_BACKDROP		4

#define BUTTON_UP			1
#define BUTTON_OVER			2
#define BUTTON_DOWN			3

// ��ɫ
#define COLOR_ARGB(a,r,g,b) \
   ((unsigned long)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define COLOR_XRGB(r,g,b) COLOR_ARGB(0xff,r,g,b)


/* GUI�Ķ���Ľṹ */
struct GUIVertex
{
	float x, y, z, rhw;     //RHW�����㽫������3D�任��ֱ��ʹ��ԭʼx,yֵ����Ļ���ơ�
	unsigned long color;
	float u, v;
};
#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// scripts
#define MAX_COMMAND_SIZE   32
#define MAX_LINE_SIZE      3072
#define MAX_PARAM_SIZE     2048