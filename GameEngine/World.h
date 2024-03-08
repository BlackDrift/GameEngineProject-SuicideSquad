/*#pragma once

#include "Component.h"

#include <vector>
#include <map>

struct PhysicsWorldCinfo
{
	DirectX::XMFLOAT3 m_gravity;
	float m_broadphaseSize;
	int m_broadphaseDivisions;

	PhysicsWorldCinfo();
};



class PhysicsWorld
{
public:

	PhysicsWorld(const PhysicsWorldCinfo& info);

	void step(float deltaTime);


	void addRigidBody(RigidBody* rb);
	void removeRigidBody(RigidBody* rb);

	bool rayCast(RayHitResult& outHit, DirectX::FXMVECTOR origin, DirectX::FXMVECTOR direction, float length);
	bool rayCast(RayHitResult& outHit, DirectX::FXMVECTOR begin, DirectX::FXMVECTOR end);
	bool rayCast(RayHitResult& outHit, const Ray& ray);


	void registerCollisionAgents();

	CollisionAgent* getCollisionAgent(PhysicsShape* a, PhysicsShape* b);



private:

	bool firstStep;

	std::vector<RigidBody*> m_dynamicRigidBodies;
	std::vector<RigidBody*> m_staticRigidBodies;

	std::map<uint32_t, CollisionAgent*> m_collisionAgents;

	DirectX::XMFLOAT3 m_gravity;
};


class World : public Component
{
	friend class Game;

public:

	World();
	~World();

	//virtual void ExecuteProcedure();

	void markForDeletion(Component* e);

	inline PhysicsWorld* getPhysicsWorld() const { return m_physicsWorld; }

	inline bool isOpen() const { return m_isOpen; }
	inline void setOpen(bool b) { m_isOpen = b; }
private:

	/// Performs destroy on all entities marked for deletion.
	void deletePendingEntities();


private:

	PhysicsWorld* m_physicsWorld;

	bool m_isOpen;
	std::vector<Component*> m_markedForDelete;
};*/

