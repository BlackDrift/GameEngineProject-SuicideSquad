#include "GameObject.h"
GameObject::GameObject(DirectX::XMVECTOR pos, DirectX::XMVECTOR forward, float width, float height, std::vector<Component> listComponents) {
	this->pos = pos;
	this->forward = forward;
	this->width = width;
	this->height = height;
	this->listComponents = listComponents;
}