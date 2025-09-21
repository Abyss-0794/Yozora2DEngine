#include "stdafx.h"
#include "Engine.h"

bool Engine::Initialize(HINSTANCE hIns, int nCmdShow)
{
	// ウインドウの初期化
	m_window = std::make_unique<Window>(1280, 720, L"Yozora2D Engine");
	if (!m_window->OnInit(hIns, nCmdShow))
	{
		DebugHelper::Print(L"Error: Failed to initialize window.");
		return false;
	}

	// グラフィックデバイスの初期化
	m_graphicsDevice = std::make_unique<GraphicsDevice>();
	if (!m_graphicsDevice->Initialize(m_window->GetWindowHandle(), m_window->GetWindowWidth(), m_window->GetWindowHeight()))
	{
		DebugHelper::Print(L"Error: Failed to initialize graphics device.");
		return false;
	}

	// コマンド関連の初期化
	m_commandListManager = std::make_unique<CommandListManager>(*m_graphicsDevice);
	if (!m_commandListManager->Initialize())
	{
		DebugHelper::Print(L"Error: Failed to initialize command list manager.");
		return false;
	}

	// シェーダーマネージャー
	m_shaderManager = std::make_unique<ShaderManager>();

	// パイプラインステート関連初期化
	m_pipelineStateManager = std::make_unique<PipelineStateManager>(*m_graphicsDevice, *m_shaderManager);
	if (!m_pipelineStateManager->Initialize())
	{
		DebugHelper::Print(L"Error: Failed to initialize pipeline state manager.");
		return false;
	}

	// テクスチャマネージャー初期化
	m_texManager = std::make_unique<TextureManager>(*m_graphicsDevice);
	if (!m_texManager->LoadShader(L"E:\\VisualStudioProject\\Yozora2DEngine\\GameEngine\\Assets\\girl.png"))
	{
		DebugHelper::Print(L"Error: Failed to load texture.");
		return false;
	}

	// レンダラーの初期化
	m_renderer = std::make_unique<Renderer>(*m_graphicsDevice, *m_commandListManager, *m_pipelineStateManager, *m_texManager , *m_window);

	return true;
}

void Engine::Run()
{
	// ウインドウの表示
	m_window->Show();
	while (m_window->MainProcess())
	{
		m_renderer->BeginFrame();

		m_renderer->DrawScene();

		m_renderer->EndFrame();
	}
}

void Engine::OnDestroy()
{
	m_commandListManager->OnDestroy();
}
