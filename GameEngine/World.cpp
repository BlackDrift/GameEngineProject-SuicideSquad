#include "World.h"
#include <d3d12.h>
#include <DirectXMath.h>
using namespace DirectX;
PhysicsWorldCinfo::PhysicsWorldCinfo()
	: m_gravity(0.0F, -9.8F, 0.0F)
	, m_broadphaseSize(128.0F)
	, m_broadphaseDivisions(4)
{

}



PhysicsWorld::PhysicsWorld(const PhysicsWorldCinfo& info)
{
	m_gravity = info.m_gravity;

	firstStep = true;
}

void PhysicsWorld::step(float deltaTime)
{
	XMVECTOR deltaVec = XMVectorReplicate(deltaTime);
	XMVECTOR gravity = XMVectorMultiply(XMLoadFloat3(&m_gravity), deltaVec);
}


#define KEY(shapeTypeA, shapeTypeB) (uint32_t) ((shapeTypeA) | ((shapeTypeB) << 16))

World::World()
{
	PhysicsWorldCinfo info;
	info.m_gravity = DirectX::XMFLOAT3(0.0F, -0.0F, 0.0F);

	m_physicsWorld = new PhysicsWorld(info);
	m_physicsWorld->registerCollisionAgents();

	m_isOpen = false;
}

World::~World()
{
	delete m_physicsWorld;
}
