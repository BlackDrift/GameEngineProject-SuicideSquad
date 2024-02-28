#include "../GameEngine/Window.h"

int _stdcall WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Window window(1280, 720, "Window");

	while (true) {
		if (const  auto msgCode = Window::ProcessMessages())
			return *msgCode;
	}
}