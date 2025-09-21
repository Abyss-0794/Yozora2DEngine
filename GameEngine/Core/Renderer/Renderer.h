#pragma once

#include "../Renderer/Shape/Base/Shape.h"
#include "../Renderer/Shape/Triangle/Triangle.h"
#include "../Renderer/Shape/Square/Square.h"

class GraphicsDevice;
class CommandListManager;
class PipelineStateManager;
class Window;

class Renderer
{
public:
	Renderer(GraphicsDevice& graphicsDevice, CommandListManager& commandListManager, PipelineStateManager& pipelineStateManager, Window& window);

	void BeginFrame();
	void DrawScene();
	void EndFrame();

private:
	GraphicsDevice& m_graphicsDevice;
	CommandListManager& m_commandListManager;
	PipelineStateManager& m_pipelineStateManager;
	Window& m_window;

	CD3DX12_VIEWPORT m_viewPort;
	CD3DX12_RECT m_scissorRect;

	std::vector<std::unique_ptr<Shape>> m_shapes;
};