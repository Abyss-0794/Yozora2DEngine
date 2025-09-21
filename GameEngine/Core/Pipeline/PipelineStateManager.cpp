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

	// ルートシグネチャの作成 (時間も計測)
	DebugHelper::MeasureTime([&]()
		{
			if (!CreateRootSignature())
			{
				isError = true;
			}

		}, L"RootSignatureの作成: ", true);
	if (isError) return false;

	// 入力レイアウトの作成
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// デフォルトPSO設定構造体の作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC defaultPsoDesc = {};
	defaultPsoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
	defaultPsoDesc.pRootSignature = m_fixedrootSignatureArray[static_cast<size_t>(ROOTSIGType::TYPE_DRAW)].Get();
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
	defaultPsoDesc.VS = CD3DX12_SHADER_BYTECODE(m_shaderManager.GetShaderBlobs(m_shaderManager.TYPE_DEFAULT).vs.Get());
	defaultPsoDesc.PS = CD3DX12_SHADER_BYTECODE(m_shaderManager.GetShaderBlobs(m_shaderManager.TYPE_DEFAULT).ps.Get());

	// 複数のPSO作成 (時間も計測)
	DebugHelper::MeasureTime([&]()
		{
			if (!CreatePipelineStateObject(defaultPsoDesc))
			{
				isError = true;
			}
		}, L"PipelineStateObjectの作成: ", true, DebugHelper::TimeType::MilliSecond);
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

ID3D12RootSignature* PipelineStateManager::GetFixedRootSignature(ROOTSIGType type)
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
	// ディスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE1 rootDescRange[1] = {};
	rootDescRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

	// パラメーター
	CD3DX12_ROOT_PARAMETER1 rootParam[1] = {};
	rootParam[0].InitAsDescriptorTable(1, &rootDescRange[0], D3D12_SHADER_VISIBILITY_PIXEL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC staticSamp(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);

	// ルートシグネチャの設定構造体
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootDesc = {};
	rootDesc.Init_1_1(_countof(rootParam), rootParam, 1, &staticSamp, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSignature, error;
	if (FAILED(D3D12SerializeVersionedRootSignature(&rootDesc, rootSignature.GetAddressOf(), error.GetAddressOf())))
	{
		DebugHelper::Print(L"Error: Failed to serialize root signature.");
		return false;
	}

	// ルートシグネチャの作成
	ComPtr<ID3D12RootSignature> targetRootSignature;
	if (FAILED(m_graphicsDevice.GetDevice()->CreateRootSignature(0, rootSignature->GetBufferPointer(), rootSignature->GetBufferSize(), IID_PPV_ARGS(targetRootSignature.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to create root signature.");
		return false;
	}

	// ルートシグネチャの保存
	m_fixedrootSignatureArray[ROOTSIGType::TYPE_DRAW] = targetRootSignature;

	return true;
}

bool PipelineStateManager::CreatePipelineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
{
	// デフォルトPSOの作成
	{
		ComPtr<ID3D12PipelineState> pso = nullptr;
		if (FAILED(m_graphicsDevice.GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pso.GetAddressOf()))))
		{
			DebugHelper::Print(L"Error: Failed to create default pipeline state object.");
			return false;
		}

		// デフォルトPSOのキャッシュ追加
		m_fixedPsoArray[static_cast<size_t>(PSOType::TYPE_DEFAULT)] = pso;
	}

	// テクスチャ用PSOの作成
	{
		//desc.pRootSignature = m_fixedrootSignatureArray[static_cast<size_t>(PSOType::TYPE_TEXTURE)].Get();
		desc.VS = CD3DX12_SHADER_BYTECODE(m_shaderManager.GetShaderBlobs(m_shaderManager.TYPE_TEXTURE).vs.Get());
		desc.PS = CD3DX12_SHADER_BYTECODE(m_shaderManager.GetShaderBlobs(m_shaderManager.TYPE_TEXTURE).ps.Get());
		desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		ComPtr<ID3D12PipelineState> pso = nullptr;
		if (FAILED(m_graphicsDevice.GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pso.GetAddressOf()))))
		{
			DebugHelper::Print(L"Error: Failed to create texture pipeline state object.");
			return false;
		}

		// デフォルトPSOのキャッシュ追加
		m_fixedPsoArray[static_cast<size_t>(PSOType::TYPE_TEXTURE)] = pso;
	}

	return true;
}
