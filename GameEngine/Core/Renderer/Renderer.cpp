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
	// 現在のバックバッファインデックスを取得
	auto backBufferCount = m_graphicsDevice.GetCurrentBackBufferCount();

	// コマンドリストの取得
	auto* cmdList = m_commandListManager.GetCommandList();

	// コマンドリストとアロケーターのリセット
	m_commandListManager.ResetCommandListAndAllocator(backBufferCount, nullptr);

	// リソースバリアの設定 (PRESENT -> TARGET)
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_graphicsDevice.GetRTV(backBufferCount), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdList->ResourceBarrier(1, &barrier);

	// ビューポートの設定
	cmdList->RSSetViewports(1, &m_viewPort);

	// シザー矩形の設定
	cmdList->RSSetScissorRects(1, &m_scissorRect);

	// レンダーターゲットビューの設定
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_graphicsDevice.GetRTVHandle(backBufferCount);
	cmdList->OMSetRenderTargets(1, &rtvHandle, TRUE, nullptr);

	// 画面クリア
	FLOAT clearColor[4] = { 0,0,0,1 };
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void Renderer::DrawScene()
{
	// コマンドリストの取得
	auto* cmdList = m_commandListManager.GetCommandList();

	// ポリゴンを描画
	{
		// PSOのセット
		cmdList->SetPipelineState(m_pipelineStateManager.GetFixedPSO(m_pipelineStateManager.TYPE_DEFAULT));

		// ルートシグネチャのセット
		cmdList->SetGraphicsRootSignature(m_pipelineStateManager.GetFixedRootSignature(m_pipelineStateManager.TYPE_DEFAULT));

		// 描画
		for (auto& shape : m_shapes)
		{
			shape->Draw(cmdList);
		}
	}
}

void Renderer::EndFrame()
{
	// 現在のバックバッファインデックスを取得
	auto backBufferCount = m_graphicsDevice.GetCurrentBackBufferCount();

	// コマンドリストの取得
	auto* cmdList = m_commandListManager.GetCommandList();

	// リソースバリアの設定 (TARGET -> PRESENT)
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_graphicsDevice.GetRTV(backBufferCount), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	cmdList->ResourceBarrier(1, &barrier);

	// コマンドの実行
	m_commandListManager.ExecuteCommand();

	// 画面のスワップ
	m_graphicsDevice.GetSwapChain()->Present(1, 0);

	// GPU処理待機
	m_commandListManager.WaitForGPUProcess();
}
