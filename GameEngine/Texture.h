#pragma once
#include "d3d12.h"
#include "Shader.h"


class Texture
{
public:
	Shader shader;

	Texture(Shader shader);

	void loadFromDisk(const char* path, D3D12_SRV_DIMENSION dims);

	void create();

	void destroy();

	inline const D3D12_CPU_DESCRIPTOR_HANDLE& getShaderResourceView() const { return m_shaderResourceView; }

private:

	void createHeapAndView(DXGI_FORMAT fmt, D3D12_SRV_DIMENSION dims);


private:

	ID3D12Resource* m_resource;
	ID3D12DescriptorHeap* m_srvHeap;

	D3D12_CPU_DESCRIPTOR_HANDLE m_shaderResourceView;
};