#pragma once

#include <vector>
#include <d3d12.h>
#include "d3dx12.h"
#include <DirectXMath.h>


class Shader
{
private:

	ID3D12PipelineState* m_pPipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	m_PipelineDesc;
	ID3DBlob* m_VSByteCode = nullptr;
	ID3DBlob* m_PSByteCode = nullptr;
	bool								m_M4XMsaaState;
	UINT								m_M4XMsaaQuality;

	ID3D12RootSignature* m_pRootSignature;
	Microsoft::WRL::ComPtr<ID3DBlob>					m_SerializedRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob>					m_ErrorBlob = nullptr;

	HRESULT								m_HResult;

	std::vector<D3D12_INPUT_ELEMENT_DESC>	m_InputLayout;

	int									m_rootParamSize;
	int									m_isDescTable;

public:
	ID3D12Device* m_Device;
	struct ConstantBufferStruct
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4X4 worldViewProjMatrix;
	};

	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
	};

	Shader();
	~Shader();

	void InitializeShader(ID3D12Device* device, std::string name = "default");

	bool InitializePipelineState();

	void InitializePipelineLayout();

	void InitializePipelineLayoutShader();

	bool InitializeRootSignatureShader();

	bool InitializeRootSignature();

	ID3D12RootSignature* GetRootSignature() {
		return m_pRootSignature;
	};

	ID3D12PipelineState* GetPipelineState() {
		return m_pPipelineState;
	};

	int GetRootParamSize() { return m_rootParamSize; };
	int GetIsDescTable() { return m_isDescTable; };

	void Update();

	HRESULT CompileShaderS(const WCHAR* filename, const char* entrypoint, const char* profile, ID3DBlob** out_code);

	std::vector<CD3DX12_STATIC_SAMPLER_DESC> GetStaticSamplers();

};
