/*#include "World.h"
#include <d3d12.h>
#include <DirectXMath.h>


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


void World::markForDeletion(Component* e)
{
	m_markedForDelete.push_back(e);
}


void World::deletePendingEntities()
{
	for (Component* e : m_markedForDelete) {
		e->OnDestroy(this);

		// Move children up.
		for (Component* c = e->m_FirstChild; c; c = c->m_Sibling) {
			c->AttachToParent(e->m_Parent);
		}

		e->removeAllAttachments();

		delete e;
	}

	m_markedForDelete.clear();
}*/
