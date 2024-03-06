#include "Rendering.h"

bool Rendering::InitializeD3D12(HWND hWnd, int screenWidth, int screenHeight)
{
    IDXGIFactory2* dxgiFactory;
	IDXGIAdapter1* adapter = nullptr;
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

    // CREATE FENCE
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;

    DX::ThrowIfFailed(device->CreateFence(
        0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
    mRtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    mDsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    mCbvSrvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


    // CREATE COMMAND QUEUE
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
        IID_PPV_ARGS(&mCommandList)));
    // Start off in a closed state. This is because the first time we
    // refer to the command list we will Reset it, and it needs to be
    // closed before calling Reset.
    mCommandList->Close();

    // Release the previously created Swap Chain
    swapChain.Reset();

    // CREATE SWAP CHAIN
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

    // CREATE DESCRIPTOR HEAPS
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = swapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;

	DX::ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	DX::ThrowIfFailed(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));

    // CREATE RENDER TARGET VIEW
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(
        mRtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < swapChainBufferCount; i++)
    {
        // Get the ith buffer in the swap chain.
        DX::ThrowIfFailed(swapChain->GetBuffer(
            i, IID_PPV_ARGS(&mSwapChainBuffer[i])));
        // Create an RTV to it.
        device->CreateRenderTargetView(
            mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
        // Next entry in heap.
        rtvHeapHandle.Offset(1, mRtvDescriptorSize);
    }

    // Create the depth/stencil buffer and view.
    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDesc.Alignment = 0;
    depthStencilDesc.Width = screenWidth;
    depthStencilDesc.Height = screenHeight;
    depthStencilDesc.DepthOrArraySize = 1;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.Format = mDepthStencilFormat;
    depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    D3D12_CLEAR_VALUE optClear;
    optClear.Format = mDepthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;
    CD3DX12_HEAP_PROPERTIES pHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    DX::ThrowIfFailed(device->CreateCommittedResource(
		&pHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())));
    // Create descriptor to mip level 0 of entire resource using the
    // format of the resource.
    device->CreateDepthStencilView(
        mDepthStencilBuffer.Get(),
        nullptr,
        DepthStencilView());
    // Transition the resource from its initial state to be used as a depth buffer.
    CD3DX12_RESOURCE_BARRIER pBarriers = CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    mCommandList->ResourceBarrier(1, &pBarriers);

    D3D12_VIEWPORT vp;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = static_cast<float>(screenWidth);
    vp.Height = static_cast<float>(screenHeight);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    mCommandList->RSSetViewports(1, &vp);

    // Clean up resources
    //device->Release();
    //adapter->Release();
    //dxgiFactory->Release();

    return true;
}

D3D12_CPU_DESCRIPTOR_HANDLE Rendering::CurrentBackBufferView()const
{
    // CD3DX12 constructor to offset to the RTV of the current back buffer.
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        mRtvHeap->GetCPUDescriptorHandleForHeapStart(),// handle start
        mCurrBackBuffer, // index to offset
        mRtvDescriptorSize); // byte size of descriptor
}

D3D12_CPU_DESCRIPTOR_HANDLE Rendering::DepthStencilView()const
{
    return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

void Rendering::Draw() {
    mDirectCmdListAlloc->Reset();
    mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr);

    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), DirectX::Colors::LightSteelBlue, 0, NULL);

    //Vertex vertices[] =
    //{
    //    { DirectX::XMFLOAT3(0.f, 0.5f, 0.5f), DirectX::XMFLOAT4(DirectX::Colors::Red) },
    //    { DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(DirectX::Colors::Green) },
    //    { DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(DirectX::Colors::Blue) },
    //};

    //const UINT64 vbByteSize = 8 * sizeof(Vertex);

    //D3dUtil buffer;

    //Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
    //Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
    //VertexBufferGPU = buffer.D3dUtil::CreateDefaultBuffer(device, mCommandList, vertices, vbByteSize, VertexBufferUploader);

    mCommandList->Close();
    ID3D12CommandList* cLists[] = { mCommandList };
    mCommandQueue->ExecuteCommandLists(1 ,cLists);

    UINT syncInterval = 0;
    UINT presentFlags = 0;
    swapChain->Present(syncInterval, presentFlags);
}