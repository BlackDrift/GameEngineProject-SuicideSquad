#pragma once
#include <d3d12.h>
class InputManager {
public:
	InputManager(int key, const char* type);
	void Update();
	bool input;
	bool lastInput;
private:
	int key;
	const char* type;
};