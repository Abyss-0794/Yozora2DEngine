#pragma once

class GraphicsDevice;

class CommandListManager
{
public:
	CommandListManager(GraphicsDevice& graphicsDevice);

	bool Initialize();
	void ResetCommandListAndAllocator(int frameCount, ID3D12PipelineState* pso = nullptr);
	void ExecuteCommand();
	void WaitForGPUProcess();
	void OnDestroy();

private:
	UINT64 m_fenceValue[2] = { 0, 0 };
	HANDLE m_fenceEvent = nullptr;

	UINT m_currentFrameCount = 0;

	ComPtr<ID3D12GraphicsCommandList> m_cmdList = nullptr;
	ComPtr<ID3D12CommandAllocator> m_cmdAllocator[2];
	ComPtr<ID3D12Fence> m_fence = nullptr;

	// éQè∆ìnÇµÇ≥ÇÍÇÈïœêî
	GraphicsDevice& m_graphicsDevice;

public:
	FORCEINLINE ID3D12GraphicsCommandList* GetCommandList() { return m_cmdList.Get(); }
	
};