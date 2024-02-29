#include "InputManager.h"
InputManager::InputManager(int key, bool type) {
	this->key = key;
	this->type = type;
}
char* InputManager::Update() {
	lastInput = input;
	if (type == "keyboard") {
		input = GetAsyncKeyState(key)
	}
	return input;
}