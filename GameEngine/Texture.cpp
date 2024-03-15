#include "stdafx.h"
#include "Texture.h"

#include "DDSTextureLoader.h"



Texture::Texture(Shader shader)
	: m_resource(0), m_srvHeap(0)
{
	this->shader = shader;
}

void Texture::loadFromDisk(const char* path, D3D12_SRV_DIMENSION dims)
{
	/*ID3D12Device* device = shader.m_Device;
	ID3D12CommandQueue* cmdQueue = shader.d3dCommandQueue;
	ID3D12CommandAllocator* cmdAllocator = shader.d3dCommandAllocator;
	ID3D12GraphicsCommandList* cmdList = shader.d3dCommandList;

	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator, 0);


	wchar_t widePath[MAX_PATH];
	mbstowcs_s(0, widePath, path, MAX_PATH);

	Microsoft::WRL::ComPtr<ID3D12Resource> res;
	Microsoft::WRL::ComPtr<ID3D12Resource> upload;
	DirectX::CreateDDSTextureFromFile12(device, cmdList, widePath, res, upload);


	cmdList->Close();
	ID3D12CommandList* lists[] = { cmdList };
	cmdQueue->ExecuteCommandLists(1, lists);

	shader.m_currentFenceValue++;

	shader.d3dCommandQueue->Signal(shader.d3dFence, shader.m_currentFenceValue);

	if (shader.d3dFence->GetCompletedValue() < shader.m_currentFenceValue) {

		shader.d3dFence->SetEventOnCompletion(shader.m_currentFenceValue, shader.m_fenceEvent);
		WaitForSingleObject(shader.m_fenceEvent, INFINITE);
	}


	m_resource = res.Get();
	m_resource->AddRef();

	this->createHeapAndView(m_resource->GetDesc().Format, dims);*/
}

void Texture::create()
{
	//ID3D12Device* device = shader.d3dDevice;

	//DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UINT;

	//// Create texture resource.
	//// TODO : This is in the upload heap!!! Move to default heap using the transfer helper.
	//CD3DX12_HEAP_PROPERTIES heapP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	//CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, 2, 2);

	//device->CreateCommittedResource(
	//	&heapP,
	//	D3D12_HEAP_FLAG_NONE,
	//	&resourceDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	0,
	//	IID_PPV_ARGS(&m_resource));

	//// Dummy 2x2 black / magenta checker.
	//UINT* data;
	//m_resource->Map(0, 0, (void**)&data);

	//data[0] = 0xFF000000;
	//data[1] = 0xFFFF00FF;
	//data[2] = 0xFFFF00FF;
	//data[3] = 0xFF000000;

	//m_resource->Unmap(0, 0);

	//this->createHeapAndView(format, D3D12_SRV_DIMENSION_TEXTURE2D);
}

void Texture::destroy()
{
	if (m_resource) {
		m_resource->Release();
		m_resource = 0;

		m_srvHeap->Release();
		m_srvHeap = 0;
	}
}


void Texture::createHeapAndView(DXGI_FORMAT fmt, D3D12_SRV_DIMENSION dims)
{
	//ID3D12Device* device = shader.d3dDevice;

	//// CPU descriptor heap and view.
	//D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = { };
	//srvHeapDesc.NumDescriptors = 1;
	//srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap));

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.Format = fmt;
	//srvDesc.ViewDimension = dims;
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//srvDesc.Texture2D.MipLevels = 1;
	//srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
	//device->CreateShaderResourceView(m_resource, &srvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());

	//m_shaderResourceView = m_srvHeap->GetCPUDescriptorHandleForHeapStart();
}