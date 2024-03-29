#pragma once
#include <dxgi1_4.h>
#include <wrl.h>
#include <DirectXColors.h>
#include <initguid.h>
#include "d3dx12.h"
#include "stdafx.h"
#include "D3dUtil.h"
#include <d3d12.h>
#include "Shader.h"


class Rendering
{
public:
	
	LPCWSTR ErrorMessage;
	bool InitializeD3D12(HWND hWnd, int screenWidth, int screenHeight);
	void Draw();
	struct Vertex
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT4 Color;
	};

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	bool m4xMsaaState;
	UINT m4xMsaaQuality;
	static const int swapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	UINT mRtvDescriptorSize;
	UINT mDsvDescriptorSize;
	UINT mCbvSrvDescriptorSize;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;

	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ID3D12Device* device = nullptr;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	ID3D12GraphicsCommandList* mCommandList;
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[swapChainBufferCount];
};