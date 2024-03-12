#include "D3dUtil.h"

Microsoft::WRL::ComPtr<ID3D12Resource> D3dUtil::CreateDefaultBuffer(
    ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList,
    const void* initData,
    UINT64 byteSize,
    Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
{
    Microsoft::WRL::ComPtr<ID3D12Resource> defaultBuffer;

    CD3DX12_HEAP_PROPERTIES heapD = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC bSize = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    CD3DX12_HEAP_PROPERTIES heapU = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_BARRIER transitionC = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_COPY_DEST);
    CD3DX12_RESOURCE_BARRIER transitionD = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);

    // Create the actual default buffer resource.
    try {
        device->CreateCommittedResource(
            &heapD,
            D3D12_HEAP_FLAG_NONE,
            &bSize,
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(defaultBuffer.GetAddressOf()));
    }
    catch (...) {
        throw;
    }
    // In order to copy CPU memory data into our default buffer, we need
    // to create an intermediate upload heap.
    try {
        device->CreateCommittedResource(
            &heapU,
            D3D12_HEAP_FLAG_NONE,
            &bSize,
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(uploadBuffer.GetAddressOf()));
    }
    catch (...) {
        throw;
    }
    // Describe the data we want to copy into the default buffer.
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;
    // Schedule to copy the data to the default buffer resource.
    // At a high level, the helper function UpdateSubresources
    // will copy the CPU memory into the intermediate upload heap.
    // Then, using ID3D12CommandList::CopySubresourceRegion,
    // the intermediate upload heap data will be copied to mBuffer.
    cmdList->ResourceBarrier(1, &transitionC);
    UpdateSubresources<1>(cmdList,
        defaultBuffer.Get(), uploadBuffer.Get(),
        0, 0, 1, &subResourceData);
    cmdList->ResourceBarrier(1, &transitionD);
    // Note: uploadBuffer has to be kept alive after the above function
    // calls because the command list has not been executed yet that
    // performs the actual copy.
    // The caller can Release the uploadBuffer after it knows the copy
    // has been executed.
    return defaultBuffer;
}