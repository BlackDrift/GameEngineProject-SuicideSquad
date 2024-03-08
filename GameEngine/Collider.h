/*#pragma once

#include "Component.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include "T"

class Collider : public Component
{
public:

	enum EMotionType
	{
		MOTION_STATIC,
		MOTION_KEYFRAMED,
		MOTION_DYNAMIC,
	};

	Collider(float radius, EMotionType motionType);

	void applyImpulse(DirectX::FXMVECTOR f);

	void setPosition(DirectX::FXMVECTOR f);
	void setRotation(DirectX::FXMVECTOR f);

	virtual void onCollide(Component* e) { }


	DirectX::XMVECTOR GetLinearVelocity();
	void setLinearVelocity(DirectX::FXMVECTOR p);

	virtual void OnSpawn(QuWorld* world) final override;
	virtual void OnUpdate(const Time& timer) final override;
	virtual void OnDestroy(QuWorld* world) final override;


private:

	RigidBody* m_rigidBody;
	PhysicsShape* m_shape;
	class MyCollideListener* m_listener;

	DirectX::XMFLOAT4 m_linearVelocity;

	float m_radius;
	EMotionType m_motionType;
};*/

