#pragma once

#include "Component.h"
#include <iostream>
#include <d3d12.h>

enum EShapeType
{
	SHAPE_BOX,
	SHAPE_SPHERE,

	//SHAPE_CAPSULE,
	//SHAPE_CYLINDER,
	//SHAPE_CONVEX,
};



class PhysicsShape
{
public:

	PhysicsShape(EShapeType t);

	inline EShapeType getType() const { return m_type; }

	virtual void getAabb(AABB& out) const;


private:

	EShapeType m_type;
};

class PhysicsShapeSphere : public PhysicsShape
{
public:

	PhysicsShapeSphere(float radius)
		: PhysicsShape(SHAPE_SPHERE)
		, m_radius(radius)
	{

	}

	inline float getRadius() const { return m_radius; }

	virtual void getAabb(AABB& out) const override;


private:

	float m_radius;
};

enum EMotionType
{
	MOTION_STATIC,
	MOTION_KEYFRAMED,
	MOTION_DYNAMIC,
};

enum ECollisionQuality
{
	QUALITY_DEFAULT,
	QUALITY_BULLET,    // More accurate collision detection, for small and/or fast moving objects.
};



struct RigidBodyCinfo
{
	PhysicsShape* m_shape;

	EMotionType m_motionType;
	ECollisionQuality m_collisionQuality;
	bool m_isTrigger;

	/// Initial transform.
	DirectX::XMFLOAT4 m_position;
	DirectX::XMFLOAT4 m_rotation;

	/// Initial velocity.
	DirectX::XMFLOAT4 m_linearVelocity;
	DirectX::XMFLOAT4 m_angularVelocity;

	RigidBodyCinfo();
};



class RigidBody
{
	friend class PhysicsWorld;

public:

	RigidBody(const RigidBodyCinfo& info);

	inline const DirectX::XMFLOAT4& getPosition() const { return m_position; }
	inline const DirectX::XMFLOAT4& getRotation() const { return m_rotation; }

	inline const DirectX::XMFLOAT4& getLinearVelocity() const { return m_linearVelocity; }

	inline EMotionType getMotionType() const { return m_motionType; }
	inline bool isTrigger() const { return m_trigger; }

	inline void* getUserData() const { return m_userData; }


	void setPosition(DirectX::FXMVECTOR p);
	void setRotation(DirectX::FXMVECTOR p);

	void setLinearVelocity(DirectX::FXMVECTOR p);

	/// Apply a force to this rigid body.
	void applyImpulse(DirectX::FXMVECTOR force);

	/// Change the rigid body's motion type during runtime.
	/// This is only possible for objects initially created with a dynamic motion type.
	void setMotionType(EMotionType m);


	inline void setUserData(void* d) { m_userData = d; }

	inline PhysicsWorld* getWorld() const { return m_world; }

	inline PhysicsShape* getShape() const { return m_shape; }



private:

	PhysicsWorld* m_world;

	PhysicsShape* m_shape;

	DirectX::XMFLOAT4 m_position;
	DirectX::XMFLOAT4 m_rotation;

	DirectX::XMFLOAT4 m_linearVelocity;
	DirectX::XMFLOAT4 m_angularVelocity;


	void* m_userData;

	EMotionType m_motionType;

	bool m_trigger : 1;
	bool m_initiallyStatic : 1;
};

class Collider : public Component
{
public:

	Collider(float radius, EMotionType motionType);

	void applyImpulse(DirectX::FXMVECTOR f);

	void setPosition(DirectX::FXMVECTOR f);
	void setRotation(DirectX::FXMVECTOR f);

	virtual void onCollide(Component* e) { }


	DirectX::XMVECTOR GetLinearVelocity();
	void setLinearVelocity(DirectX::FXMVECTOR p);

	virtual void OnSpawn(World* world);
	virtual void OnUpdate(const Time& timer);
	virtual void OnDestroy(World* world);


private:

	RigidBody* m_rigidBody;
	PhysicsShape* m_shape;
	class MyCollideListener* m_listener;

	DirectX::XMFLOAT4 m_linearVelocity;

	float m_radius;
	EMotionType m_motionType;
};