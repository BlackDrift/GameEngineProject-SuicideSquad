#pragma once
#include "Transform.h"
class GameObject
{
	GameObject(DirectX::XMVECTOR pos, DirectX::XMVECTOR forward = { 0, 0, -1 }, float width, float height);
	Transform transform;
	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR forward;
	float width;
	float height;
};

