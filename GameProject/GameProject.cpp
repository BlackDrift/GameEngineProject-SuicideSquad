#include "../GameEngine/Window.h"
#include "../GameEngine/InputManager.h"
#include <iostream>

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	Window window(1280, 720, "Window");

	while (true) {
		if ( Window::ProcessMessages()==false)
			break;
		}










































































































































































































































































	}
	
	return 0;
}
