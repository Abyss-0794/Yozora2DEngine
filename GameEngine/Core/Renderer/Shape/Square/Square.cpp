#include "stdafx.h"
#include "Square.h"

Square::Square(GraphicsDevice& graphicsDevice)
{
    // 四角形頂点 + インデックス
    std::vector<VertexBuffer::Vertex> vertices =
    {
        { {-0.4f,  0.7f, 0.0f }, {0, 0} },
        { { 0.4f,  0.7f, 0.0f }, {1, 0} },
        { { 0.4f, -0.7f, 0.0f }, {1, 1} },
        { {-0.4f, -0.7f, 0.0f }, {0, 1} }
    };
    std::vector<WORD> indices = { 0,1,2, 2,3,0 };

    m_indexCount = (UINT)indices.size();
    m_vertexBuffer = std::make_unique<VertexBuffer>(graphicsDevice, vertices, indices);
}

void Square::Draw(ID3D12GraphicsCommandList* cmdList)
{
    m_vertexBuffer->Bind(cmdList);
    cmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
