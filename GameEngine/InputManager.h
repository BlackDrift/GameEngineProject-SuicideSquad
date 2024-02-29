#pragma once
#include <d3d12.h>
class InputManager {
	InputManager(int key, char* type);
	bool Update();
	bool input;
	bool lastInput;
	int key;
	char* type;
};