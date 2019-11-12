#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <assert.h>

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "xaudio2.lib")


#define SCREEN_WIDTH	(960)			// ウインドウの幅
#define SCREEN_HEIGHT	(540)			// ウインドウの高さ


HWND GetWindow();

XMFLOAT3 operator + (XMFLOAT3 a, XMFLOAT3 b);
XMFLOAT3 operator - (XMFLOAT3 a, XMFLOAT3 b);
XMFLOAT3 operator * (XMFLOAT3 a, float b);
XMFLOAT3 operator * (float a, XMFLOAT3 b);
XMFLOAT3 operator / (XMFLOAT3 a, float b);