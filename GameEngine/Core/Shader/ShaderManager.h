#pragma once

class GraphicsDevice;

class ShaderManager
{
public:
	enum FixedShaderType
	{
		TYPE_DEFAULT,
		TYPE_TEXTURE,
		COUNT
	};

	struct YOZORAENGINE_SHADER_DESC
	{
		LPCWSTR filePath = L"";
		LPCSTR entoryPoint = "";
		LPCSTR shaderVersion = "";
		D3D_SHADER_MACRO* pDefines = nullptr;
		ID3DInclude* pInclude = nullptr;
		UINT flag1 = 0;
		UINT flag2 = 0;
	};

	struct YOZORAENGINE_SHADER_INFO
	{
		ComPtr<ID3DBlob> vs;
		ComPtr<ID3DBlob> ps;
	};

	ShaderManager();
	ID3DBlob* LoadShader(const std::wstring& filePath, const std::string& entryPoint, const std::string& target);
	auto& GetShaderBlobs(FixedShaderType type)
	{
		// TODO: return ステートメントをここに挿入します
		return m_fixedShaderBlobs[static_cast<size_t>(type)];
	}

private:
	std::array<YOZORAENGINE_SHADER_INFO, static_cast<size_t>(FixedShaderType::COUNT)> m_fixedShaderBlobs;
	std::unordered_map<std::wstring, ComPtr<ID3DBlob>> m_shaderBlobs;

	void CompileShader(const YOZORAENGINE_SHADER_DESC* pShaderDesc, ID3DBlob** ppShaderBlob);
};