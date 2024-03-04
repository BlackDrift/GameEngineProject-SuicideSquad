#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <iostream>
class Shader
{
public:
	Shader(Microsoft::WRL::ComPtr<ID3D12Device> device, DXGI_FORMAT bBufferFormat, DXGI_FORMAT dStencilFormat, bool MSAAState, UINT MSSAAQuality);
	~Shader();

	/*ComPtr<ID3DBlob> mvsByteCode;
	ComPtr<ID3DBlob> mpsByteCode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mPSO = nullptr;*/

};