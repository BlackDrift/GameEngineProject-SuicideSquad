#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>

class Rendering
{
public:
	bool InitializeD3D12(HWND hWnd, int screenWidth, int screenHeight);
};