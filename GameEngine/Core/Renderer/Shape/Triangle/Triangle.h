#pragma once

#include "../Base/Shape.h"
#include "../../../Graphics/GraphicsDevice.h"
#include "../../VertexBuffer/VertexBuffer.h"

class Triangle : public Shape
{
public:
	Triangle(GraphicsDevice& graphicsDevice);

	void Draw(ID3D12GraphicsCommandList* cmdList) override;

private:
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	UINT m_vertexCount;
};