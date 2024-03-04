#include "GameObject.h"
GameObject::GameObject(DirectX::XMVECTOR pos, DirectX::XMVECTOR forward = { 0, 0, -1 }, float width, float height) {
	this->pos = pos;
	this->forward = forward;
	this->width = width;
	this->height = height;
}