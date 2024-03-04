#pragma once
#include <dxgi1_4.h>
#include <wrl.h>
#include "stdafx.h"

class Rendering
{
public:
	
	LPCWSTR ErrorMessage;
	bool InitializeD3D12(HWND hWnd, int screenWidth, int screenHeight);

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	bool m4xMsaaState;
	UINT m4xMsaaQuality;
};