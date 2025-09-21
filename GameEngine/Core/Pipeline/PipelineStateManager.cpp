#include "stdafx.h"
#include "PipelineStateManager.h"

#include "../Graphics/GraphicsDevice.h"
#include "../Shader/ShaderManager.h"

PipelineStateManager::PipelineStateManager(GraphicsDevice& graphicsDevice, ShaderManager& shaderManager) : m_graphicsDevice(graphicsDevice), m_shaderManager(shaderManager)
{
}

bool PipelineStateManager::Initialize()
{
	bool isError = false;

	// ���[�g�V�O�l�`���̍쐬 (���Ԃ��v��)
	DebugHelper::MeasureTime([&]()
		{
			if (!CreateRootSignature())
			{
				isError = true;
			}

		}, L"RootSignature�̍쐬: ", true);
	if (isError) return false;

	// ���̓��C�A�E�g�̍쐬
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// �f�t�H���gPSO�ݒ�\���̂̍쐬
	D3D12_GRAPHICS_PIPELINE_STATE_DESC defaultPsoDesc = {};
	defaultPsoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	defaultPsoDesc.pRootSignature = m_fixedrootSignatureArray[PSOType::TYPE_DEFAULT].Get();
	defaultPsoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_DYNAMIC_INDEX_BUFFER_STRIP_CUT;
	defaultPsoDesc.NodeMask = 0;
	defaultPsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	defaultPsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	defaultPsoDesc.DepthStencilState.DepthEnable = FALSE;
	defaultPsoDesc.DepthStencilState.StencilEnable = FALSE;
	defaultPsoDesc.NumRenderTargets = 1;
	defaultPsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	defaultPsoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	defaultPsoDesc.SampleDesc.Count = 1;
	defaultPsoDesc.SampleDesc.Quality = 0;
	defaultPsoDesc.SampleMask = UINT_MAX;
	defaultPsoDesc.VS = CD3DX12_SHADER_BYTECODE(m_shaderManager.LoadShader(L"GameEngine\\Shaders\\BasicShader\\BasicVertexShader.hlsl", "BasicVS", "vs_5_0"));
	defaultPsoDesc.PS = CD3DX12_SHADER_BYTECODE(m_shaderManager.LoadShader(L"GameEngine\\Shaders\\BasicShader\\BasicPixelShader.hlsl", "BasicPS", "ps_5_0"));

	// ������PSO�쐬 (���Ԃ��v��)
	DebugHelper::MeasureTime([&]()
		{
			if (!CreatePipelineStateObject(defaultPsoDesc))
			{
				isError = true;
			}
		}, L"PipelineStateObject�̍쐬: ", true, DebugHelper::TimeType::MilliSecond);
	if (isError) return false;

	return true;
}

ID3D12PipelineState* PipelineStateManager::GetDynamicPSO(PSOKey key)
{
	auto foundPSO = m_dynamicPsoMap.find(key);
	if (foundPSO != m_dynamicPsoMap.end())
	{
		return foundPSO->second.Get();
	}
	return nullptr;
}

ID3D12PipelineState* PipelineStateManager::GetFixedPSO(PSOType type)
{
	auto foundPSO = m_fixedPsoArray[type];
	if (foundPSO != nullptr)
	{
		return foundPSO.Get();
	}
	return nullptr;
}

ID3D12RootSignature* PipelineStateManager::GetDynamicRootSignature(PSOKey key)
{
	auto foundRootSignature = m_rootSignatures.find(key);
	if (foundRootSignature != m_rootSignatures.end())
	{
		return foundRootSignature->second.Get();
	}
	return nullptr;
}

ID3D12RootSignature* PipelineStateManager::GetFixedRootSignature(PSOType type)
{
	auto foundRootSignature = m_fixedrootSignatureArray[type];
	if (foundRootSignature != nullptr)
	{
		return foundRootSignature.Get();
	}
	return nullptr;
}

bool PipelineStateManager::CreateRootSignature()
{
	// ���[�g�V�O�l�`���̐ݒ�\����
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc = {};
	rootDesc.Init_1_1(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSignature, error;
	if (FAILED(D3D12SerializeVersionedRootSignature(&rootDesc, rootSignature.GetAddressOf(), error.GetAddressOf())))
	{
		DebugHelper::Print(L"Error: Failed to serialize root signature.");
		return false;
	}

	// ���[�g�V�O�l�`���̍쐬
	ComPtr<ID3D12RootSignature> targetRootSignature;
	if (FAILED(m_graphicsDevice.GetDevice()->CreateRootSignature(0, rootSignature->GetBufferPointer(), rootSignature->GetBufferSize(), IID_PPV_ARGS(targetRootSignature.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to create root signature.");
		return false;
	}

	// ���[�g�V�O�l�`���̕ۑ�
	m_fixedrootSignatureArray[PSOType::TYPE_DEFAULT] = targetRootSignature;

	return true;
}

bool PipelineStateManager::CreatePipelineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
{
	// �f�t�H���gPSO�̍쐬
	ComPtr<ID3D12PipelineState> pso = nullptr;
	if (FAILED(m_graphicsDevice.GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pso.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to create default pipeline state object.");
		return false;
	}

	// �f�t�H���gPSO�̃L���b�V���ǉ�
	m_fixedPsoArray[PSOType::TYPE_DEFAULT] = pso;

	return true;
}
