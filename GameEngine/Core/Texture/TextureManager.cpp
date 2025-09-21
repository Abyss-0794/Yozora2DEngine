#include "stdafx.h"
#include "TextureManager.h"

#include "../Graphics/GraphicsDevice.h"

TextureManager::TextureManager(GraphicsDevice& graphicsDevice) : m_graphicsDevice(graphicsDevice)
{
}

bool TextureManager::LoadShader(const std::wstring& filePath)
{
	auto* d3dDevice = m_graphicsDevice.GetDevice();
	auto* cmdQueue = m_graphicsDevice.GetCommandQueue();

	ComPtr<ID3D12Resource> textureBuffer = nullptr;

	// ���\�[�X�̃A�b�v���[�h���J�n
	ResourceUploadBatch resourceUpload(d3dDevice);
	resourceUpload.Begin();

	if (!std::filesystem::exists(filePath)) {
		std::wstring str = L"Error: Texture file not found: " + filePath;
		DebugHelper::Print(str);
		return false;
	}

	// �e�N�X�`�����[�h
	if (FAILED(CreateWICTextureFromFile(d3dDevice, resourceUpload, filePath.c_str(), textureBuffer.GetAddressOf())))
	{
		DebugHelper::Print(L"Error: Failed to laod texture.");
		return false;
	}

	// ���\�[�X�A�b�v���[�h���I��
	auto uploadResourceFinished = resourceUpload.End(cmdQueue);
	uploadResourceFinished.wait();

	// SRV�̍쐬
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle(m_graphicsDevice.GetSRVHeap()->GetCPUDescriptorHandleForHeapStart());
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = textureBuffer->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = textureBuffer->GetDesc().MipLevels;
	d3dDevice->CreateShaderResourceView(textureBuffer.Get(), &srvDesc, srvHandle);

	// �e�N�X�`�������쐬
	Texture textureInfo = {};
	textureInfo.textureBuffer = textureBuffer;
	textureInfo.srvHandle = D3D12_GPU_DESCRIPTOR_HANDLE(m_graphicsDevice.GetSRVHeap()->GetGPUDescriptorHandleForHeapStart());

	// �L���b�V���ɒǉ�
	m_dynamicTextureMap.try_emplace(filePath, textureInfo);

	return true;
}

TextureManager::Texture TextureManager::GetTextureInfo(const std::wstring& filePath)
{
	auto texInfo = m_dynamicTextureMap.find(filePath);
	if (texInfo != m_dynamicTextureMap.end())
	{
		return texInfo->second;
	}
	return Texture();
}
