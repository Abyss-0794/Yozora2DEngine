#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	//ベーシックシェーダーのコンパイル
	{
		// 頂点シェーダーのコンパイル
		ComPtr<ID3DBlob> vertexShader = nullptr;
		YOZORAENGINE_SHADER_DESC basicVertexShaderDesc = {};
		basicVertexShaderDesc.filePath = L"GameEngine\\Shaders\\BasicShader\\BasicVertexShader.hlsl";
		basicVertexShaderDesc.entoryPoint = "BasicVS";
		basicVertexShaderDesc.shaderVersion = "vs_5_0";
		basicVertexShaderDesc.pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE;
		CompileShader(&basicVertexShaderDesc, vertexShader.GetAddressOf());

		// ピクセルシェーダーのコンパイル
		ComPtr<ID3DBlob> pixelShader = nullptr;
		YOZORAENGINE_SHADER_DESC basicPixelShaderDesc = {};
		basicPixelShaderDesc.filePath = L"GameEngine\\Shaders\\BasicShader\\BasicPixelShader.hlsl";
		basicPixelShaderDesc.entoryPoint = "BasicPS";
		basicPixelShaderDesc.shaderVersion = "ps_5_0";
		basicPixelShaderDesc.pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE;
		CompileShader(&basicPixelShaderDesc, pixelShader.GetAddressOf());

		// キャッシュ追加
		GetShaderBlobs(FixedShaderType::TYPE_DEFAULT).vs = vertexShader;
		GetShaderBlobs(FixedShaderType::TYPE_DEFAULT).ps = pixelShader;
	}

	// テクスチャ用シェーダーのコンパイル
	{
		// 頂点シェーダーのコンパイル
		ComPtr<ID3DBlob> vertexShader = nullptr;
		YOZORAENGINE_SHADER_DESC basicVertexShaderDesc = {};
		basicVertexShaderDesc.filePath = L"GameEngine\\Shaders\\TextureShader\\TextureVertexShader.hlsl";
		basicVertexShaderDesc.entoryPoint = "TexVS";
		basicVertexShaderDesc.shaderVersion = "vs_5_0";
		basicVertexShaderDesc.pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE;
		CompileShader(&basicVertexShaderDesc, vertexShader.GetAddressOf());

		// ピクセルシェーダーのコンパイル
		ComPtr<ID3DBlob> pixelShader = nullptr;
		YOZORAENGINE_SHADER_DESC basicPixelShaderDesc = {};
		basicPixelShaderDesc.filePath = L"GameEngine\\Shaders\\TextureShader\\TexturePixelShader.hlsl";
		basicPixelShaderDesc.entoryPoint = "TexPS";
		basicPixelShaderDesc.shaderVersion = "ps_5_0";
		basicPixelShaderDesc.pInclude = D3D_COMPILE_STANDARD_FILE_INCLUDE;
		CompileShader(&basicPixelShaderDesc, pixelShader.GetAddressOf());

		// キャッシュ追加
		GetShaderBlobs(FixedShaderType::TYPE_TEXTURE).vs = vertexShader;
		GetShaderBlobs(FixedShaderType::TYPE_TEXTURE).ps = pixelShader;
	}
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

void ShaderManager::CompileShader(const YOZORAENGINE_SHADER_DESC* pShaderDesc, ID3DBlob** ppShaderBlob)
{
	ComPtr<ID3DBlob> shader, error;
	if (FAILED(D3DCompileFromFile(pShaderDesc->filePath, pShaderDesc->pDefines, 
		pShaderDesc->pInclude, pShaderDesc->entoryPoint, pShaderDesc->shaderVersion, pShaderDesc->flag1, pShaderDesc->flag2, shader.GetAddressOf(), error.GetAddressOf())))
	{
		DebugHelper::Print(L"Error: Failed to compile shader.");
	}

	*ppShaderBlob = shader.Detach();
}
