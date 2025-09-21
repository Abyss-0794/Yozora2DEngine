#include "stdafx.h"
#include "GraphicsDevice.h"

bool GraphicsDevice::Initialize(HWND hWnd, int width, int height)
{
#if _DEBUG
	EnableDebugLayer();
#endif

	// ファクトリ作成
	if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_dxgiFactory.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to create dxgi factory.");
		return false;
	}

	// デバイスの作成
	ComPtr<IDXGIAdapter1> targetAdapter = GetHardWareAdapter(m_dxgiFactory.Get());
	if (FAILED(D3D12CreateDevice(targetAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(m_d3dDevice.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to create device.");
		return false;
	}

	// コマンドキューの作成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.NodeMask = 0;
	if (FAILED(m_d3dDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_cmdQueue.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to create command queue.");
		return false;
	}

	// スワップチェインの作成
	ComPtr<IDXGISwapChain1> swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = static_cast<UINT>(width);
	swapChainDesc.Height = static_cast<UINT>(height);
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	if (FAILED(m_dxgiFactory->CreateSwapChainForHwnd(m_cmdQueue.Get(), hWnd, &swapChainDesc, nullptr, nullptr, swapChain.GetAddressOf())))
	{
		DebugHelper::Print(L"Error: Failed to create swap chain.");
		return false;
	}
	if (FAILED(swapChain.As(&m_swapChain)))
	{
		DebugHelper::Print(L"Error: Failed to convert swap chain4.");
		return false;
	}

	// ディスクリプタヒープの作成 (RTV)
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = FrameCount;
	rtvHeapDesc.NodeMask = 0;
	if (FAILED(m_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_rtvHeap.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to create descriptor heap (RTV).");
		return false;
	}

	// レンダーターゲットビューの作成
	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT idx = 0; idx < FrameCount; idx++)
	{
		if (FAILED(m_swapChain->GetBuffer(idx, IID_PPV_ARGS(m_rtv[idx].GetAddressOf()))))
		{
			DebugHelper::Print(L"Error: Failed to get back buffer (RTV).");
			return false;
		}
		m_d3dDevice->CreateRenderTargetView(m_rtv[idx].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}

	return true;
}

void GraphicsDevice::EnableDebugLayer()
{
	ComPtr<ID3D12Debug> debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
	{
		debugController->EnableDebugLayer();
	}
}

ComPtr<IDXGIAdapter1> GraphicsDevice::GetHardWareAdapter(IDXGIFactory7* pdxgiFactory)
{
	ComPtr<IDXGIAdapter1> targetAdapter = nullptr;
	for (UINT idx = 0; pdxgiFactory->EnumAdapters1(idx, targetAdapter.ReleaseAndGetAddressOf()) != DXGI_ERROR_NOT_FOUND; idx++)
	{
		DXGI_ADAPTER_DESC1 adpDesc = {};
		targetAdapter->GetDesc1(&adpDesc);

		if (adpDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

		if (SUCCEEDED(D3D12CreateDevice(targetAdapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), nullptr)))
		{
			return targetAdapter;
		}
	}
	return nullptr;
}
