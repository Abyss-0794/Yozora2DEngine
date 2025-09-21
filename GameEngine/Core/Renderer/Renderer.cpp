#include "stdafx.h"
#include "Renderer.h"

#include "../Graphics/GraphicsDevice.h"
#include "../Command/CommandListManager.h"
#include "../Pipeline/PipelineStateManager.h"
#include "../Window/Window.h"

Renderer::Renderer(GraphicsDevice& graphicsDevice, CommandListManager& commandListManager, PipelineStateManager& pipelineStateManager, Window& window) :
	m_graphicsDevice(graphicsDevice),
m_commandListManager(commandListManager),
m_pipelineStateManager(pipelineStateManager),
m_window(window),
m_viewPort({ 0,0,static_cast<FLOAT>(m_window.GetWindowWidth()), static_cast<FLOAT>(m_window.GetWindowHeight()) }), m_scissorRect(0, 0, static_cast<LONG>(m_window.GetWindowWidth()), static_cast<LONG>(m_window.GetWindowHeight()))
{
	m_shapes.push_back(std::make_unique<Square>(graphicsDevice));
}

void Renderer::BeginFrame()
{
	// ���݂̃o�b�N�o�b�t�@�C���f�b�N�X���擾
	auto backBufferCount = m_graphicsDevice.GetCurrentBackBufferCount();

	// �R�}���h���X�g�̎擾
	auto* cmdList = m_commandListManager.GetCommandList();

	// �R�}���h���X�g�ƃA���P�[�^�[�̃��Z�b�g
	m_commandListManager.ResetCommandListAndAllocator(backBufferCount, nullptr);

	// ���\�[�X�o���A�̐ݒ� (PRESENT -> TARGET)
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_graphicsDevice.GetRTV(backBufferCount), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdList->ResourceBarrier(1, &barrier);

	// �r���[�|�[�g�̐ݒ�
	cmdList->RSSetViewports(1, &m_viewPort);

	// �V�U�[��`�̐ݒ�
	cmdList->RSSetScissorRects(1, &m_scissorRect);

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_graphicsDevice.GetRTVHandle(backBufferCount);
	cmdList->OMSetRenderTargets(1, &rtvHandle, TRUE, nullptr);

	// ��ʃN���A
	FLOAT clearColor[4] = { 0,0,0,1 };
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void Renderer::DrawScene()
{
	// �R�}���h���X�g�̎擾
	auto* cmdList = m_commandListManager.GetCommandList();

	// �|���S����`��
	{
		// PSO�̃Z�b�g
		cmdList->SetPipelineState(m_pipelineStateManager.GetFixedPSO(m_pipelineStateManager.TYPE_DEFAULT));

		// ���[�g�V�O�l�`���̃Z�b�g
		cmdList->SetGraphicsRootSignature(m_pipelineStateManager.GetFixedRootSignature(m_pipelineStateManager.TYPE_DEFAULT));

		// �`��
		for (auto& shape : m_shapes)
		{
			shape->Draw(cmdList);
		}
	}
}

void Renderer::EndFrame()
{
	// ���݂̃o�b�N�o�b�t�@�C���f�b�N�X���擾
	auto backBufferCount = m_graphicsDevice.GetCurrentBackBufferCount();

	// �R�}���h���X�g�̎擾
	auto* cmdList = m_commandListManager.GetCommandList();

	// ���\�[�X�o���A�̐ݒ� (TARGET -> PRESENT)
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_graphicsDevice.GetRTV(backBufferCount), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	cmdList->ResourceBarrier(1, &barrier);

	// �R�}���h�̎��s
	m_commandListManager.ExecuteCommand();

	// ��ʂ̃X���b�v
	m_graphicsDevice.GetSwapChain()->Present(1, 0);

	// GPU�����ҋ@
	m_commandListManager.WaitForGPUProcess();
}
