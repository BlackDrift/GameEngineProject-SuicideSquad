#pragma once

#include <vector>
#include <map>
#include <d3d12.h>
#include <DirectXMath.h>

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


	void registerCollisionAgents();



private:

	bool firstStep;


	DirectX::XMFLOAT3 m_gravity;
};


class World
{

public:

	World();
	~World();

	//virtual void ExecuteProcedure();

	inline PhysicsWorld* getPhysicsWorld() const { return m_physicsWorld; }

	inline bool isOpen() const { return m_isOpen; }
	inline void setOpen(bool b) { m_isOpen = b; }
private:

	/// Performs destroy on all entities marked for deletion.
	void deletePendingEntities();


private:

	PhysicsWorld* m_physicsWorld;

	bool m_isOpen;
};
