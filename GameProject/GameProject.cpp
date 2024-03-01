#include "../GameEngine/Window.h"
#include "../GameEngine/Rendering.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Rendering renderer;

	Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Window");

	//renderer.Rendering::InitializeD3D12(window.hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);

	while (true) {
		if (const  auto msgCode = Window::ProcessMessages())
			return *msgCode;
	}

	//MessageBox(window.hWnd, message, nom de la box, MB_OK);
	//Le message et le nom de la box doivent être de type LPCSTR, syntaxe : L"blabla"
}