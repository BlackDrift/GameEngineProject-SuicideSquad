#include "Rendering.h"

bool Rendering::InitializeD3D12(HWND hWnd, int screenWidth, int screenHeight)
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

    // Create a descriptor for the swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 2; 
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    // Create the swap chain
    IDXGISwapChain* swapChain;
    result = dxgiFactory->CreateSwapChain(device, &swapChainDesc, &swapChain);
    if (FAILED(result)) {
        // Handle error
        device->Release();
        adapter->Release();
        dxgiFactory->Release();
        return false;
    }

    // Clean up resources
    swapChain->Release();
    device->Release();
    adapter->Release();
    dxgiFactory->Release();

    // Device, adapter, and swap chain are now ready for rendering

    return true;
}
