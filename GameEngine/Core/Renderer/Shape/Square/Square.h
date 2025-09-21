#pragma once

#include "../Base/Shape.h"
#include "../../../Graphics/GraphicsDevice.h"
#include "../../VertexBuffer/VertexBuffer.h"

class Square : public Shape
{
public:
	Square(GraphicsDevice& graphicsDevice);

	void Draw(ID3D12GraphicsCommandList* cmdList) override;

private:
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	UINT m_indexCount;
};