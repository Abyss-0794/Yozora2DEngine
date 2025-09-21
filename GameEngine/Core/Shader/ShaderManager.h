#pragma once

class GraphicsDevice;

class ShaderManager
{
public:
	ShaderManager();
	ID3DBlob* LoadShader(const std::wstring& filePath, const std::string& entryPoint, const std::string& target);

private:
	std::unordered_map<std::wstring, ComPtr<ID3DBlob>> m_shaderBlobs;
};