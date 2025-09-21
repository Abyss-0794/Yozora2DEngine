#pragma once

class Shape
{
public:
	virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;
	virtual ~Shape() = default;
};