#include "stdafx.h"
#include "Window.h"

Window::Window(const unsigned int width, const unsigned int height, const std::wstring& windowName): m_width(width), m_height(height), m_windowName(windowName), m_hwnd(nullptr), m_nCmdShow(0)
{
}

Window::~Window()
{
	// �E�C���h�E�̔j��
	DestroyWindow(m_hwnd);
}

bool Window::OnInit(HINSTANCE hIns, int nCmdShow)
{
	// �E�C���h�E�N���X�̍쐬
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hIns;
	wc.lpszClassName = m_windowName.c_str();
	wc.lpfnWndProc = WndProc;

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wc);

	// �E�C���h�E�̒���
	RECT rect = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�̍쐬
	m_hwnd = CreateWindow(wc.lpszClassName, m_windowName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, wc.hInstance, nullptr);
	if (m_hwnd == nullptr)
	{
		return false;
	}

	m_nCmdShow = nCmdShow;

	return true;
}

void Window::Show()
{
	ShowWindow(m_hwnd, m_nCmdShow);
}

bool Window::MainProcess()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
