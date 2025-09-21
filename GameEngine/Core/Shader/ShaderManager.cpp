#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ID3DBlob* ShaderManager::LoadShader(const std::wstring& filePath, const std::string& entryPoint, const std::string& target)
{
	auto foundShader = m_shaderBlobs.find(filePath);
	if (foundShader != m_shaderBlobs.end())
	{
		return foundShader->second.Get();
	}

	ComPtr<ID3DBlob> shader, error;
	if (FAILED(D3DCompileFromFile(filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), 0, 0, shader.GetAddressOf(), error.GetAddressOf())))
	{
		DebugHelper::Print(L"Error: Failed to compile shader.");
		return nullptr;
	}

	m_shaderBlobs.try_emplace(filePath, shader);

	return shader.Get();
}
