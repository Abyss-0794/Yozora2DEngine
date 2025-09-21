#include "stdafx.h"
#include "Core/Engine/Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	// �G���W���̏�����
	Engine engine;
	if (!engine.Initialize(hInstance, nCmdShow))
	{
		DebugHelper::Print(L"Failed to initialize game engine.");
		return -1;
	}

	// �G���W���̃��C������
	engine.Run();

	// �G���W���̔j������
	engine.OnDestroy();

	return 0;
}