#include "InputManager.h"
InputManager::InputManager(int key, const char* type) {
	this->key = key;
	this->type = type;
	this->lastInput = 0;
	this->input = 0;
}
void InputManager::Update() {
	lastInput = input;
	if (type == "keyboard") {
		input = GetAsyncKeyState(key);
	}
}