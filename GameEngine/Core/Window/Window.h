#pragma once

class Window
{
public:
	Window(const unsigned int width, const unsigned int height, const std::wstring& windowName);
	~Window();

	bool OnInit(HINSTANCE hIns, int nCmdShow);
	void Show();
	bool MainProcess();

private:
	const unsigned int m_width, m_height;
	std::wstring m_windowName;
	HWND m_hwnd;
	int m_nCmdShow;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

public:
	FORCEINLINE unsigned int GetWindowWidth() const { return m_width; }
	FORCEINLINE unsigned int GetWindowHeight() const { return m_height; }
	FORCEINLINE HWND GetWindowHandle() { return m_hwnd; }
};