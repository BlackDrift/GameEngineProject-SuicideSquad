#pragma once
#include "Transform.h"
#include "Component.h"
#include "Texture.h"
#include <iostream>
#include <vector>
class GameObject
{
	GameObject(DirectX::XMVECTOR pos, DirectX::XMVECTOR forward, float width, float height, std::vector<Component> listComponents);
	Transform transform;
	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR forward = { 0, 0, -1 };
	float width;
	float height;
	std::vector<Component> listComponents;
};