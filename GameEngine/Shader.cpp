#include "stdafx.h"
#include "Shader.h"
#include "D3dx12.h"
#include "D3DCompiler.h"
#include "Window.h"
#include <iostream>
#include "combaseapi.h"


#define ROUND_256(x) ((x + 255) & ~255)


void loadResource(std::string& out, UINT id)
{
    HINSTANCE hinstance = GetModuleHandle(0);

    HRSRC res = FindResource(hinstance, MAKEINTRESOURCE(id), RT_RCDATA);
    HGLOBAL resg = LoadResource(hinstance, res);
    SIZE_T size = SizeofResource(hinstance, res);

    out.assign(reinterpret_cast<const char*>(LockResource(resg)), size);

    FreeResource(resg);
}

const D3D12_INPUT_ELEMENT_DESC Shader::m_defaultInputLayout[] =
{
 { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
 { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3 * sizeof(float), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
 { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 6 * sizeof(float), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
 { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 8 * sizeof(float), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
};

const D3D12_GRAPHICS_PIPELINE_STATE_DESC Shader::m_defaultPipelineState =
{
 0,
 { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
 { 0 },
 CD3DX12_BLEND_DESC(D3D12_DEFAULT),
 UINT_MAX,
 CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
 CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT),
 { Shader::m_defaultInputLayout, sizeof(Shader::m_defaultInputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC) },
 D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
 D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
 1,
 { DXGI_FORMAT_R8G8B8A8_UNORM },
 DXGI_FORMAT_D24_UNORM_S8_UINT,
 { 1, 0 },
 0,
 { 0, 0 },
 D3D12_PIPELINE_STATE_FLAG_NONE
};



static void getOffset(INT* outRangeOffset, INT* outTableParam, ShaderParameter* rootParams, int shaderRegister, ShaderParameterType paramType)
{
    UINT numDescsInRange = 0;
    UINT numDescsTotal = 0;

    ShaderParameterType type = rootParams[0].type;
    ShaderParameterUpdate update = rootParams[0].updatePolicy;
    int baseSlot = rootParams[0].slot;

    while (rootParams->type != TYPE_UNKNOWN) {

        if (rootParams->slot == shaderRegister && rootParams->type == paramType) {
            *outRangeOffset = numDescsTotal;
            *outTableParam = rootParams->updatePolicy;
            return;
        }

        rootParams++;
        numDescsInRange++;

        if (rootParams->slot != baseSlot + numDescsInRange || type != rootParams->type || update != rootParams->updatePolicy) {

            numDescsInRange = 0;
            type = rootParams->type;
            baseSlot = rootParams->slot;

            update = rootParams->updatePolicy;
        }

        if (rootParams->type == TYPE_UNKNOWN) break;

        numDescsTotal++;
    }

    *outRangeOffset = -1;
    *outTableParam = -1;
}

static void generateSerializedRootSignature(ShaderParameter* rootParams, ID3DBlob** outRootSig)
{
    std::vector<D3D12_ROOT_PARAMETER> params;
    std::vector<D3D12_DESCRIPTOR_RANGE> descriptorTables[UPDATE_NUM_UPDATES];
    UINT numDescsInRange = 0;
    UINT baseRange = 0;

    for (int i = 0; i < UPDATE_NUM_UPDATES; ++i) {
        descriptorTables[i].reserve(64);
    }
    params.reserve(64);

    ShaderParameterType type = rootParams[0].type;
    ShaderParameterUpdate update = rootParams[0].updatePolicy;
    int baseSlot = rootParams[0].slot;

    while (true) {

        rootParams++;
        numDescsInRange++;

        if (rootParams->slot != baseSlot + numDescsInRange || type != rootParams->type || update != rootParams->updatePolicy) {

            D3D12_DESCRIPTOR_RANGE_TYPE rangeType;
            switch (type) {
            case TYPE_CONSTANT_BUFFER: rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; break;
            case TYPE_TEXTURE_2D: rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; break;
            default: rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; break;
            }

            CD3DX12_DESCRIPTOR_RANGE r; r.Init(rangeType, numDescsInRange, baseSlot, 0, numDescsInRange - 1);
            descriptorTables[update].push_back(r);

            numDescsInRange = 0;
            type = rootParams->type;
            baseSlot = rootParams->slot;

            if (update != rootParams->updatePolicy) {
                CD3DX12_ROOT_PARAMETER p; p.InitAsDescriptorTable(descriptorTables[update].size(), descriptorTables[update].data());
                params.push_back(p);
            }

            update = rootParams->updatePolicy;
        }

        if (rootParams->type == TYPE_UNKNOWN) break;
    }

    CD3DX12_ROOT_SIGNATURE_DESC desc(params.size(), params.data(), 0, 0, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    CD3DX12_STATIC_SAMPLER_DESC ssDesc; ssDesc.Init(0);
    desc.NumStaticSamplers = 1;
    desc.pStaticSamplers = &ssDesc;

    D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, outRootSig, 0);
}



Shader::Shader()
    : m_serializedRootSig(0), m_rootSignature(0)
{
    D3D12_DESCRIPTOR_HEAP_DESC spDesc;
    spDesc.NumDescriptors = 65536;
    spDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    spDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    spDesc.NodeMask = 0;
    this -> result = D3D12CreateDevice(0, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&this->d3dDevice));
    this->m_currentFenceValue = 0;
    this->d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->d3dFence));
    this->m_fenceEvent = CreateEventEx(0, 0, 0, EVENT_ALL_ACCESS);
    this->d3dDevice->CreateDescriptorHeap(&spDesc, IID_PPV_ARGS(&this->m_cbvHeap));
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = { };
    cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    d3dDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&d3dCommandQueue));
    this->d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&this->d3dCommandAllocator));
    this->d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, this->d3dCommandAllocator, 0, IID_PPV_ARGS(&this->d3dCommandList));
    this->d3dCommandList->Close();
    this->cbvDescriptorSize = this->d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    ZeroMemory(m_shaderBytecodes, SHADER_NUM_TYPES * sizeof(ID3DBlob*));
}

Shader::~Shader()
{

}

void Shader::destroy()
{
    if (m_pso) {
        m_pso->Release();
    }

    if (m_rootSignature) {
        m_rootSignature->Release();

        m_shaderBytecodes[SHADER_VS]->Release();
        m_shaderBytecodes[SHADER_PS]->Release();
    }
}

bool Shader::compile()
{
    ID3D12Device* device = this->d3dDevice;

    generateSerializedRootSignature(m_parameters.data(), &m_serializedRootSig);

    device->CreateRootSignature(0, m_serializedRootSig->GetBufferPointer(), m_serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

    return true;
}

bool Shader::isReady() const
{
    return m_shaderBytecodes[SHADER_VS] && m_shaderBytecodes[SHADER_PS] && m_rootSignature;
}

void Shader::getDefaultPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC& outDesc) const
{
    outDesc = m_defaultPipelineState;

    outDesc.pRootSignature = this->getRootSignature();
    outDesc.VS = this->getShaderBytecode<Shader::SHADER_VS>();
    outDesc.PS = this->getShaderBytecode<Shader::SHADER_PS>();
}



void Shader::setConstantBuffer(int shaderRegister, int offset)
{
    ID3D12Device* device = this->d3dDevice;
    ID3D12DescriptorHeap* cbvHeap = this->m_cbvHeap;
    ID3D12GraphicsCommandList* cmdList = this->d3dCommandList;
    UINT cbvDescriptorSize = this->cbvDescriptorSize;

    INT descriptorOffset, tableIndex;
    getOffset(&descriptorOffset, &tableIndex, m_parameters.data(), shaderRegister, TYPE_CONSTANT_BUFFER);

    cmdList->SetGraphicsRootDescriptorTable(
        tableIndex,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(cbvHeap->GetGPUDescriptorHandleForHeapStart(), offset, cbvDescriptorSize));
}

void Shader::setTexture2D(int shaderRegister, int offset)
{
    ID3D12Device* device = this->d3dDevice;
    ID3D12DescriptorHeap* cbvHeap = this->m_cbvHeap;
    ID3D12GraphicsCommandList* cmdList = this->d3dCommandList;
    UINT cbvDescriptorSize = this->cbvDescriptorSize;

    INT descriptorOffset, tableIndex;
    getOffset(&descriptorOffset, &tableIndex, m_parameters.data(), shaderRegister, TYPE_TEXTURE_2D);

    cmdList->SetGraphicsRootDescriptorTable(
        tableIndex,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(cbvHeap->GetGPUDescriptorHandleForHeapStart(), offset, cbvDescriptorSize));
}

void Shader::compileShaderSource(Type shaderType, const char* source, SIZE_T size)
{
    if (m_shaderBytecodes[shaderType]) return;

    static const char* entryPoints[] =
    {
     "vs_main",
     "ps_main",
    };
    static const char* versions[] =
    {
     "vs_5_0",
     "ps_5_0",
    };

    ID3DBlob* errors = 0;
    D3DCompile(source, size, 0, 0, 0, entryPoints[shaderType], versions[shaderType], D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_shaderBytecodes[shaderType], &errors);

    if (errors) {
        std::cout << reinterpret_cast<const char*>(errors->GetBufferPointer()) << '\n';
        errors->Release();
    }
}

ShaderDefault::ShaderDefault()
{
    m_parameters.push_back({ "cb_objectData",   0, TYPE_CONSTANT_BUFFER, UPDATE_PER_INSTANCE });
    m_parameters.push_back({ "cb_materialData", 1, TYPE_CONSTANT_BUFFER, UPDATE_PER_MATERIAL });
    m_parameters.push_back({ "textureDiffuse",  0, TYPE_TEXTURE_2D,      UPDATE_PER_MATERIAL });
    m_parameters.push_back({ "textureSkybox",   1, TYPE_TEXTURE_2D,      UPDATE_PER_MATERIAL });
    m_parameters.push_back({ "cb_frameData",    2, TYPE_CONSTANT_BUFFER, UPDATE_PER_FRAME });
    m_parameters.push_back({ 0,                 0, TYPE_UNKNOWN,         UPDATE_UNKNOWN });
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC ShaderDefault::init()
{
    // Load shader source and compile.
    std::string source;
    loadResource(source, ID_SHADER_DEFAULT);

    this->compileShaderSource<SHADER_VS>(source.data(), source.length());
    this->compileShaderSource<SHADER_PS>(source.data(), source.length());
    this->compile();

    // Pipeline state.
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    this->getDefaultPipelineState(psoDesc);
    return psoDesc;
}