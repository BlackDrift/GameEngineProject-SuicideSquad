#include "../GameEngine/Window.h"
#include "../GameEngine/Rendering.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Rendering renderer;

	Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Window");

	if(renderer.InitializeD3D12(window.hWnd, WINDOW_WIDTH, WINDOW_HEIGHT) == false)
	{
		MessageBox(window.hWnd, renderer.ErrorMessage, L"Error - Initialize", 0);
		return 0;
	}

	while (true) 
	{
		if (Window::ProcessMessages() == false)
			return 0;
	}

	//MessageBox(window.hWnd, message, nom de la box, MB_OK);
	//Le message et le nom de la box doivent être de type LPCSTR, syntaxe : L"blabla"
}