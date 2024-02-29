#include "Rendering.h"

bool Rendering::InitializeD3D12(HWND hwnd, int screenWidth, int screenHeight)
{
    IDXGIFactory4* dxgiFactory;
    HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(result)) {
        // Handle error
        return false;
    }

    IDXGIAdapter1* adapter;
    for (UINT adapterIndex = 0; dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex) {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        // Check if the adapter supports DirectX 12
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
            break;
        }

        // Release the adapter if it doesn't support DirectX 12
        adapter->Release();
        adapter = nullptr;
    }

    if (adapter == nullptr) {
        // Handle error
        dxgiFactory->Release();
        return false;
    }

    ID3D12Device* device;
    result = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
    if (FAILED(result)) {
        // Handle error 
        adapter->Release();
        dxgiFactory->Release();
        return false;
    }

    adapter->Release();
    dxgiFactory->Release();

    return true;
}
