#include "Rendering.h"

bool Rendering::InitializeD3D12(HWND hWnd, int screenWidth, int screenHeight)
{
    IDXGIFactory2* dxgiFactory;
	IDXGIAdapter1* adapter = nullptr;
	ID3D12Device* device = nullptr;
    m4xMsaaState = false;
    m4xMsaaQuality = 1;
	HRESULT result2 = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
    HRESULT result = CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(result)) {
        // Handle error
        ErrorMessage = L"CreateDXGIFactory2 didn't work...";
        return false;
    }

   
    for (UINT adapterIndex = 0; dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex) {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        // Check if the adapter supports DirectX 12
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        if (SUCCEEDED(result2)) {
            break;
        }

        // Release the adapter if it doesn't support DirectX 12
        adapter->Release();
        adapter = nullptr;
    }

    if (adapter == nullptr) {
        // Handle error
		ErrorMessage = L"The adapter didn't work...";
        dxgiFactory->Release();
        return false;
    }

    if (FAILED(result2)) {
        // Handle error
		ErrorMessage = L"CreateDevice didn't work...";
        adapter->Release();
        dxgiFactory->Release();
        return false;
    }

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    DX::ThrowIfFailed(device->CreateCommandQueue(
        &queueDesc, IID_PPV_ARGS(&mCommandQueue)));
    DX::ThrowIfFailed(device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf())));
    DX::ThrowIfFailed(device->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        mDirectCmdListAlloc.Get(), // Associated command allocator
        nullptr, // Initial PipelineStateObject
        IID_PPV_ARGS(mCommandList.GetAddressOf())));
    // Start off in a closed state. This is because the first time we
    // refer to the command list we will Reset it, and it needs to be
    // closed before calling Reset.
    mCommandList->Close();

    swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
    HRESULT result3 = dxgiFactory->CreateSwapChain(
        mCommandQueue.Get(),
        &sd,
        swapChain.GetAddressOf());

    if (FAILED(result3)) {
        // Handle error
        ErrorMessage = L"CreateSwapChain didn't work...";
        device->Release();
        adapter->Release();
        dxgiFactory->Release();
        return false;
    }

    // Clean up resources
    device->Release();
    adapter->Release();
    dxgiFactory->Release();

    // Device, adapter, and swap chain are now ready for rendering

    return true;
}
