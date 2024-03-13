#include "Collider.h"

using namespace DirectX;

void PhysicsShapeSphere::getAabb(AABB& out) const
{
	DirectX::XMVECTOR pr, nr;
	pr = DirectX::XMVectorReplicate(m_radius);
	nr = DirectX::XMVectorNegate(pr);

	out = AABB(nr, pr);
}

PhysicsShape::PhysicsShape(EShapeType t)
	: m_type(t)
{

}


void PhysicsShape::getAabb(AABB& out) const
{
	XMVECTOR zero = XMVectorZero();

	out = AABB(zero, zero);
}

RigidBodyCinfo::RigidBodyCinfo()
	: m_motionType(MOTION_STATIC)
	, m_collisionQuality(QUALITY_DEFAULT)
	, m_shape(0)
	, m_isTrigger(false)
{

}



RigidBody::RigidBody(const RigidBodyCinfo& info)
	: m_world(0)
	, m_shape(info.m_shape)
	, m_motionType(info.m_motionType)
	, m_userData(0)
{
	assert(m_shape != 0);

	m_trigger = info.m_isTrigger;
	m_initiallyStatic = (info.m_motionType != MOTION_DYNAMIC);

	assert(!m_trigger || m_initiallyStatic);

	XMVECTOR zero = XMVectorZero();
	XMStoreFloat4(&m_position, XMLoadFloat4(&info.m_position));
	XMStoreFloat4(&m_linearVelocity, zero);
}

void RigidBody::setPosition(FXMVECTOR p)
{
	XMStoreFloat4(&m_position, p);
}

void RigidBody::setRotation(FXMVECTOR p)
{
	XMStoreFloat4(&m_rotation, p);
}

void RigidBody::setLinearVelocity(FXMVECTOR p)
{
	XMStoreFloat4(&m_linearVelocity, p);
}

void RigidBody::applyImpulse(FXMVECTOR force)
{
	XMVECTOR vel = XMLoadFloat4(&m_linearVelocity);
	vel = XMVectorAdd(vel, force);
	XMStoreFloat4(&m_linearVelocity, vel);
}

void RigidBody::setMotionType(EMotionType mt)
{
	assert(!m_initiallyStatic);

	m_motionType = mt;
}



Collider::Collider(float radius, EMotionType motionType)
	: m_rigidBody(0)
	, m_shape(0)
	, m_radius(radius)
	, m_motionType(motionType)
{
	m_linearVelocity = { 0, 0, 0, 0 };
}

void Collider::applyImpulse(FXMVECTOR f)
{
	XMVECTOR newVel = XMVectorAdd(f, XMLoadFloat4(&m_linearVelocity));
	XMStoreFloat4(&m_linearVelocity, newVel);

	if (m_rigidBody) {
		m_rigidBody->applyImpulse(f);
	}
}

void Collider::setPosition(FXMVECTOR f)
{
	XMFLOAT3 pos; XMStoreFloat3(&pos, f);
	Component::setPosition(pos);

	if (m_rigidBody) {
		m_rigidBody->setPosition(f);
	}
}

void Collider::setRotation(FXMVECTOR f)
{
	XMFLOAT4 rot; XMStoreFloat4(&rot, f);
	Component::setRotation(rot);

	if (m_rigidBody) {
		m_rigidBody->setRotation(f);
	}
}


void Collider::OnSpawn(World* world)
{
	m_shape = new PhysicsShapeSphere(m_radius);

	RigidBodyCinfo info;
	info.m_motionType = m_motionType;
	info.m_shape = m_shape;
	XMStoreFloat4(&info.m_position, XMLoadFloat3(&this->GetTransform().getPosition()));
	m_rigidBody = new RigidBody(info);

	m_rigidBody->setUserData(this);

	m_rigidBody->applyImpulse(XMLoadFloat4(&m_linearVelocity));
}

void Collider::OnUpdate(const Time& timer)
{
	if (m_rigidBody) {
		XMVECTOR v = XMLoadFloat4(&m_rigidBody->getPosition());
		this->setPosition(v);
	}
}

void Collider::OnDestroy(World* world)
{
	if (m_rigidBody)
	{

		delete m_rigidBody;
		delete m_shape;

		m_rigidBody = 0;
		m_shape = 0;
	}
}

XMVECTOR Collider::GetLinearVelocity()
{
	return XMLoadFloat4(&m_rigidBody->getLinearVelocity());
}

void Collider::setLinearVelocity(DirectX::FXMVECTOR p)
{
	m_rigidBody->setLinearVelocity(p);
}
