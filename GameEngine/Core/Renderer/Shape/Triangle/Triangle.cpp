#include "stdafx.h"
#include "Triangle.h"

Triangle::Triangle(GraphicsDevice& graphicsDevice)
{
    // 頂点データ
    std::vector<VertexBuffer::Vertex> vertices =
    {
        { { 0.0f,  0.5f, 0.0f }, {0, 0} },
        { { 0.5f, -0.5f, 0.0f }, {1, 1} },
        { {-0.5f, -0.5f, 0.0f }, {1, 0} }
    };
    m_vertexCount = (UINT)vertices.size();

    // 頂点バッファーの作成
    m_vertexBuffer = std::make_unique<VertexBuffer>(graphicsDevice, vertices);
}

void Triangle::Draw(ID3D12GraphicsCommandList* cmdList)
{
    m_vertexBuffer->Bind(cmdList);
    cmdList->DrawInstanced(m_vertexCount, 1, 0, 0);
}
