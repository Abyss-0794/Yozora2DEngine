#include "stdafx.h"
#include "Sprite.h"

#include "../../Texture/TextureManager.h"
#include "../../Pipeline/PipelineStateManager.h"
#include "../../Command/CommandListManager.h"

Sprite::Sprite(GraphicsDevice& graphicsDevice, TextureManager& texManager, CommandListManager& cmdListManager, PipelineStateManager& pipelineStateManager) : 
	m_texManager(texManager), m_cmdListManager(cmdListManager),m_pipelineStateManager(pipelineStateManager), m_graphicsDevice(graphicsDevice)
{
	m_square = std::make_unique<Square>(graphicsDevice);
}

void Sprite::Draw(const std::wstring& filePath)
{
	// �f�B�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* descHeaps[] = { m_graphicsDevice.GetSRVHeap() };
	m_cmdListManager.GetCommandList()->SetDescriptorHeaps(_countof(descHeaps), descHeaps);

	// �f�B�X�N���v�^�e�[�u��
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandle = m_texManager.GetTextureInfo(filePath).srvHandle;
	m_cmdListManager.GetCommandList()->SetGraphicsRootDescriptorTable(0, srvHandle);

	// �`��
	m_square->Draw(m_cmdListManager.GetCommandList());
}
