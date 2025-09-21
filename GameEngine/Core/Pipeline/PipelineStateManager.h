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
		COUNT
	};

	PipelineStateManager(GraphicsDevice& graphicsDevice, ShaderManager& shaderManager);
	bool Initialize();
	ID3D12PipelineState* GetDynamicPSO(PSOKey key);
	ID3D12PipelineState* GetFixedPSO(PSOType type);
	ID3D12RootSignature* GetDynamicRootSignature(PSOKey key);
	ID3D12RootSignature* GetFixedRootSignature(PSOType type);

private:
	PSOKey HashKey(const std::string& name)
	{
		return std::hash<std::string>{}(name);
	}

	GraphicsDevice& m_graphicsDevice;
	ShaderManager& m_shaderManager;

	std::array<ComPtr<ID3D12PipelineState>, static_cast<size_t>(PSOType::COUNT)> m_fixedPsoArray;			// ŒÅ’èŽí—Þ—pPSO
	std::array<ComPtr<ID3D12RootSignature>, static_cast<size_t>(PSOType::COUNT)> m_fixedrootSignatureArray;	// ŒÅ’èŽí—Þ—pRS
	std::unordered_map<PSOKey, ComPtr<ID3D12PipelineState>> m_dynamicPsoMap;
	std::unordered_map<PSOKey, ComPtr<ID3D12RootSignature>> m_rootSignatures;

	bool CreateRootSignature();
	bool CreatePipelineStateObject(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);
};