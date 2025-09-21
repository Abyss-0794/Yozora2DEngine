#pragma once

class GraphicsDevice;

class VertexBuffer
{
public:
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	VertexBuffer(GraphicsDevice& graphicsDevice, const std::vector<Vertex>& vertices);
	VertexBuffer(GraphicsDevice& graphicsDevice, const std::vector<Vertex>& vertices, const std::vector<WORD>& indices);

	void Bind(ID3D12GraphicsCommandList* cmdList);

private:
	ComPtr<ID3D12Resource> m_vertexBuffer = nullptr;
	ComPtr<ID3D12Resource> m_indexBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView = {};
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView = {};

	UINT m_vertexCount = 0;
	UINT m_indexCount = 0;

	ComPtr<ID3D12Resource> CreateVertexBuffer(GraphicsDevice& device, const std::vector<Vertex>& vertices);
	ComPtr<ID3D12Resource> CreateIndexBuffer(GraphicsDevice& device, const std::vector<WORD>& indices);
};