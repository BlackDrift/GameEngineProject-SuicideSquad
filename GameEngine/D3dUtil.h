#pragma once
#include "d3dx12.h"
#include "stdafx.h"
#include <d3dcompiler.h>
#define Check(x) \
    if (!(x)) { MessageBoxA(0, #x, "Check Failed", MB_OK); __debugbreak(); }
#define CheckSucceeded(hresult) \
    Check(SUCCEEDED(hresult));

class D3dUtil
{
public:
        Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
        ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList,
        const void* initData,
        UINT64 byteSize,
        Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);
};

