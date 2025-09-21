#pragma once

class GraphicsDevice
{
public:
	bool Initialize(HWND hWnd, int width, int height);

private:

	static const int FrameCount = 2;

	UINT m_rtvDescriptorSize = 0;

	ComPtr<IDXGIFactory7> m_dxgiFactory = nullptr;
	ComPtr<ID3D12Device> m_d3dDevice = nullptr;
	ComPtr<ID3D12CommandQueue> m_cmdQueue = nullptr;
	ComPtr<IDXGISwapChain4> m_swapChain = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap = nullptr;
	ComPtr<ID3D12Resource> m_rtv[FrameCount] = { nullptr, nullptr };

	void EnableDebugLayer();
	ComPtr<IDXGIAdapter1> GetHardWareAdapter(IDXGIFactory7* pdxgiFactory);

public:
	FORCEINLINE ID3D12Device* GetDevice() { return m_d3dDevice.Get(); }
	FORCEINLINE ID3D12CommandQueue* GetCommandQueue() { return m_cmdQueue.Get(); }
	FORCEINLINE ID3D12DescriptorHeap* GetRTVHeap() { return m_rtvHeap.Get(); }
	FORCEINLINE ID3D12Resource* GetRTV(int index) { return m_rtv[index].Get(); }
	FORCEINLINE IDXGISwapChain4* GetSwapChain() { return m_swapChain.Get(); }
	FORCEINLINE UINT GetCurrentBackBufferCount() { return m_swapChain->GetCurrentBackBufferIndex(); }
	FORCEINLINE D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle(int index) { return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), index, m_rtvDescriptorSize); }
};