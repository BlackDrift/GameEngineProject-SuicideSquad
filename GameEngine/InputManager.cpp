#include "InputManager.h"
InputManager::InputManager(int key, char* type) {
	this->key = key;
	this->type = type;
}
bool InputManager::Update() {
	lastInput = input;
	if (type == "keyboard") {
		input = GetAsyncKeyState(key);
	}
	return input;
}