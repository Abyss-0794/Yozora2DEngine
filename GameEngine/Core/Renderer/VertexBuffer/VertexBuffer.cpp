#include "stdafx.h"
#include "VertexBuffer.h"

#include "../../Graphics/GraphicsDevice.h"

VertexBuffer::VertexBuffer(GraphicsDevice& graphicsDevice, const std::vector<Vertex>& vertices)
{
	m_vertexCount = static_cast<UINT>(vertices.size());

	m_vertexBuffer = CreateVertexBuffer(graphicsDevice, vertices);

	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	m_vertexBufferView.SizeInBytes = sizeof(Vertex) * m_vertexCount;
}

VertexBuffer::VertexBuffer(GraphicsDevice& graphicsDevice, const std::vector<Vertex>& vertices, const std::vector<WORD>& indices)
{
	m_vertexCount = static_cast<UINT>(vertices.size());
	m_indexCount = static_cast<UINT>(indices.size());

	m_vertexBuffer = CreateVertexBuffer(graphicsDevice, vertices);
	m_indexBuffer = CreateIndexBuffer(graphicsDevice, indices);

	m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
	m_vertexBufferView.SizeInBytes = sizeof(Vertex) * m_vertexCount;

	m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = sizeof(WORD) * m_indexCount;
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
}

void VertexBuffer::Bind(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	if (m_indexCount > 0)
	{
		cmdList->IASetIndexBuffer(&m_indexBufferView);
	}
}

ComPtr<ID3D12Resource> VertexBuffer::CreateVertexBuffer(GraphicsDevice& device, const std::vector<Vertex>& vertices)
{
	// 頂点バッファーの作成
	CD3DX12_HEAP_PROPERTIES uploadHeapProp(D3D12_HEAP_TYPE_UPLOAD);
	auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * vertices.size());

	ComPtr<ID3D12Resource> vertexBuffer;
	if (FAILED(device.GetDevice()->CreateCommittedResource(&uploadHeapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(vertexBuffer.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to committed resource vertex buffer.");
		return nullptr;
	}

	Vertex* vertMap = nullptr;
	vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&vertMap));
	std::copy(vertices.begin(), vertices.end(), vertMap);
	vertexBuffer->Unmap(0, nullptr);

	return vertexBuffer;
}

ComPtr<ID3D12Resource> VertexBuffer::CreateIndexBuffer(GraphicsDevice& device, const std::vector<WORD>& indices)
{
	// インデックスをバッファーの作成
	CD3DX12_HEAP_PROPERTIES uploadHeapProp(D3D12_HEAP_TYPE_UPLOAD);
	auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(WORD) * indices.size());

	ComPtr<ID3D12Resource> indexBuffer;
	if (FAILED(device.GetDevice()->CreateCommittedResource(&uploadHeapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(indexBuffer.GetAddressOf()))))
	{
		DebugHelper::Print(L"Error: Failed to committed resource vertex buffer.");
		return nullptr;
	}

	WORD* indexMap = nullptr;
	indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&indexMap));
	std::copy(indices.begin(), indices.end(), indexMap);
	indexBuffer->Unmap(0, nullptr);

	return indexBuffer;
}
