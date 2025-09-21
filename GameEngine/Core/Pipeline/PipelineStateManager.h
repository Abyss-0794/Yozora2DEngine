#pragma once

class GraphicsDevice;
class ShaderManager;

using PSOKey = size_t;

class PipelineStateManager
{
public:
	enum PSOType
	{
		TYPE_DEFAULT,
		TYPE_TEXTURE,
		COUNT_PSO
	};
	enum ROOTSIGType
	{
		TYPE_DRAW,
		COUNT_ROOTSIG
	};

	PipelineStateManager(GraphicsDevice& graphicsDevice, ShaderManager& shaderManager);
	bool Initialize();
	ID3D12PipelineState* GetDynamicPSO(PSOKey key);
	ID3D12PipelineState* GetFixedPSO(PSOType type);
	ID3D12RootSignature* GetDynamicRootSignature(PSOKey key);
	ID3D12RootSignature* GetFixedRootSignature(ROOTSIGType type);

private:
	PSOKey HashKey(const std::string& name)
	{
		return std::hash<std::string>{}(name);
	}

	GraphicsDevice& m_graphicsDevice;
	ShaderManager& m_shaderManager;

	std::array<ComPtr<ID3D12PipelineState>, static_cast<size_t>(PSOType::COUNT_PSO)> m_fixedPsoArray;			// �Œ��ޗpPSO
	std::array<ComPtr<ID3D12RootSignature>, static_cast<size_t>(ROOTSIGType::COUNT_ROOTSIG)> m_fixedrootSignatureArray;	// �Œ��ޗpRS
	std::unordered_map<PSOKey, ComPtr<ID3D12PipelineState>> m_dynamicPsoMap;
	std::unordered_map<PSOKey, ComPtr<ID3D12RootSignature>> m_rootSignatures;

	bool CreateRootSignature();
	bool CreatePipelineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);
};