// Stranded Engine 
// STE引擎 基础定义

#pragma once

#ifndef STE_IMPLEMENTATION
#define	STE_API				_declspec(dllimport)
#else
#define	STE_API				_declspec(dllexport)

#include <d3d9.h>
#include <d3dx9.h>
#endif // !STE_IMPLEMENTATION


// windows 平台限定
#include <Windows.h>


// x86_32 平台限定
#define	STDCALL				__stdcall


// 语言特性 限定
#define	STE_EXPORT_BEGIN	extern "C" {
#define	STE_EXPORT_END		}

#define STE_DECLARE_INTERFACE(FunctionName, ReturnValue)	STE_API ReturnValue STDCALL FunctionName


/* 渲染 */
#define SAMPLE_TYPE			D3DMULTISAMPLE_4_SAMPLES

// 渲染状态
enum TextureState  //贴图状态
{
	MIN_FILTER,
	MAG_FILTER,
	MIP_FILTER
};

enum FilterType   //贴图过滤状态
{
	FILTER_POINT,
	FILTER_LINEAR,
	FILTER_ANISOTROPIC
};


/* I/O映射 */
#include "IOKeyMap.h"

/* GUI */
// GUI 状态
#define GUI_DYNAMICTEXT		1
#define GUI_STATICTEXT		2
#define GUI_BUTTON			3
#define GUI_BACKDROP		4

#define BUTTON_UP			1
#define BUTTON_OVER			2
#define BUTTON_DOWN			3

// 颜色
#define COLOR_ARGB(a,r,g,b) \
   ((unsigned long)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define COLOR_XRGB(r,g,b) COLOR_ARGB(0xff,r,g,b)


/* GUI的顶点的结构 */
struct GUIVertex
{
	float x, y, z, rhw;     //RHW：顶点将不进行3D变换，直接使用原始x,y值在屏幕绘制。
	unsigned long color;
	float u, v;
};
#define D3DFVF_GUI (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// scripts
#define MAX_COMMAND_SIZE   32
#define MAX_LINE_SIZE      3072
#define MAX_PARAM_SIZE     2048