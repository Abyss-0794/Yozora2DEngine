#pragma once

#include "../Window/Window.h"
#include "../Graphics/GraphicsDevice.h"
#include "../Command/CommandListManager.h"
#include "../Renderer/Renderer.h"
#include "../Shader/ShaderManager.h"
#include "../Pipeline/PipelineStateManager.h"
#include "../Texture/TextureManager.h"

class Engine
{
public:
	bool Initialize(HINSTANCE hIns, int nCmdShow);
	void Run();
	void OnDestroy();

private:
	std::unique_ptr<Window> m_window;
	std::unique_ptr<GraphicsDevice> m_graphicsDevice;
	std::unique_ptr<CommandListManager> m_commandListManager;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<ShaderManager> m_shaderManager;
	std::unique_ptr<PipelineStateManager> m_pipelineStateManager;
	std::unique_ptr<TextureManager> m_texManager;
};