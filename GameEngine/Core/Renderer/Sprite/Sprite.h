#pragma once

#include "../../Renderer/Shape/Square/Square.h"

class GraphicsDevice;
class TextureManager;
class CommandListManager;
class PipelineStateManager;

class Sprite
{
public:
	Sprite(GraphicsDevice& graphicsDevice, TextureManager& texManager, CommandListManager& cmdListManager, PipelineStateManager& pipelineStateManager);
	void Draw(const std::wstring& filePath);

private:
	GraphicsDevice& m_graphicsDevice;
	TextureManager& m_texManager;
	CommandListManager& m_cmdListManager;
	PipelineStateManager& m_pipelineStateManager;


	std::unique_ptr<Square> m_square;
};