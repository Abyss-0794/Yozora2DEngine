#include "stdafx.h"
#include "CommandListManager.h"
#include "../Graphics/GraphicsDevice.h"

CommandListManager::CommandListManager(GraphicsDevice& graphicsDevice): m_graphicsDevice(graphicsDevice)
{
}

bool CommandListManager::Initialize()
{
	// �f�o�C�X�̎擾
	auto* d3dDevice = m_graphicsDevice.GetDevice();

	// �R�}���h�A���P�[�^�[�̍쐬
	for (int idx = 0; idx < 2; idx++)
	{
		if (FAILED(d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmdAllocator[idx].GetAddressOf()))))
		{
			DebugHelper::Print(L"Error: Failed to create command allocator.");
			return false;
		}
	}

	// �R�}���h���X�g�̍쐬
	if (FAILED(d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator[0].Get(), nullptr, IID_PPV_ARGS(m_cmdList.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to create command list.");
		return false;
	}
	m_cmdList->Close();

	// �t�F���X�̍쐬
	if (FAILED(d3dDevice->CreateFence(m_fenceValue[0], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_fence.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to create fence.");
		return false;
	}
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return true;
}

void CommandListManager::ResetCommandListAndAllocator(int frameCount, ID3D12PipelineState* pso)
{
	// �R�}���h�A���P�[�^�[�̃��Z�b�g
	if (FAILED(m_cmdAllocator[frameCount]->Reset()))
	{
		DebugHelper::Print(L"Error: Failed to reset command allocator.");
		throw;
	}

	// �R�}���h���X�g�̃��Z�b�g
	if (FAILED(m_cmdList->Reset(m_cmdAllocator[frameCount].Get(), pso)))
	{
		DebugHelper::Print(L"Error: Failed to reset command list.");
		throw;
	}

	// ���݂̃t���[���J�E���g��ێ�
	m_currentFrameCount = frameCount;
}

void CommandListManager::ExecuteCommand()
{
	// �R�}���h�L���[�̎擾
	auto* commandQueue = m_graphicsDevice.GetCommandQueue();

	// �R�}���h�̃N���[�Y
	m_cmdList->Close();

	// �R�}���h���s
	ID3D12CommandList* cmdLists[] = {m_cmdList.Get()};
	commandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
}

void CommandListManager::WaitForGPUProcess()
{
	// �R�}���h�L���[�̎擾
	auto* commandQueue = m_graphicsDevice.GetCommandQueue();

	commandQueue->Signal(m_fence.Get(), ++m_fenceValue[m_currentFrameCount]);
	if (m_fence->GetCompletedValue() < m_fenceValue[m_currentFrameCount])
	{
		m_fence->SetEventOnCompletion(m_fenceValue[m_currentFrameCount], m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

void CommandListManager::OnDestroy()
{
	// GPU�����ҋ@
	WaitForGPUProcess();

	// �t�F���X�̃C�x���g�j��
	CloseHandle(m_fenceEvent);
}
