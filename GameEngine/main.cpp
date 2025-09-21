#include "stdafx.h"
#include "Core/Engine/Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	// エンジンの初期化
	Engine engine;
	if (!engine.Initialize(hInstance, nCmdShow))
	{
		DebugHelper::Print(L"Failed to initialize game engine.");
		return -1;
	}

	// エンジンのメイン処理
	engine.Run();

	// エンジンの破棄処理
	engine.OnDestroy();

	return 0;
}