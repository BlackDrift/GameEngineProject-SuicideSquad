#pragma once
#include <Windows.h>
#include <optional>

class Window
{
public:
	Window(int width, int height, const char* name);
	~Window();
	static std::optional<int> ProcessMessages();
private:
	static LRESULT _stdcall WindowProc(HWND hWnd, UINT msg, WPARAM wPraram, LPARAM lParam);
private:
	int width;
	int height;
	HWND hWnd;
	const char* windowName = "Triangle Moteur";
	HINSTANCE hInstance;
}; 