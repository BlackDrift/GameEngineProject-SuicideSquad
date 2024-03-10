/*#include "Component.h"


using namespace DirectX;


Component::Component()
	: m_Parent(0), m_FirstChild(0), m_Sibling(0)
	, m_dirtyWorldMatrix(true)
	, m_markedForDeletion(false)
{
	//init to null
}

Component::~Component()
{
}

void Component::attachChild(Component* child)
{
	// Undo previous attachments.
	if (child->m_Parent) {
		child->DetachFromParent();
	}

	// Prepend child.
	child->m_Parent = this;
	child->m_Sibling = this->m_FirstChild;
	this->m_FirstChild = child;

	World* w = (World*)child->getWorld();
	if (w && w->isOpen()) {
		child->OnSpawn(w);
	}
}

void Component::AttachToParent(Component* Parent)
{
	if (m_Parent != NULL)
	{
		DetachFromParent();
	}

	m_Parent = Parent;
	this->m_Sibling = m_Parent->m_FirstChild;
	Parent->m_FirstChild = this;

	World* w = (World*)this->getWorld();
	if (w->isOpen()) {
		this->OnSpawn(w);
	}
}

void Component::DetachFromParent()
{
	World* w = this->getWorld();

	this->removeAllAttachments();
	w->attachChild(this);
}

void Component::Destroy(bool children)
{
	World* world = (World*)this->getWorld();
	if (!world || m_markedForDeletion) return;

	// Destroy children recursively.
	if (children) {
		for (Component* c = this->m_FirstChild; c; c = c->m_Sibling) {
			c->Destroy(children);
		}
	}

	// Mark this entity for deletion.
	m_markedForDeletion = true;
	world->markForDeletion(this);
}



XMVECTOR Component::getWorldPosition()
{
	// Extract translate component from the world matrix.
	const XMFLOAT4X4& mat = this->GetWorldTransformMatrix();
	const XMFLOAT4& pos = *((const XMFLOAT4*)mat.m[3]);

	return XMLoadFloat4(&pos);
}

XMVECTOR Component::getWorldRotation()
{
	XMVECTOR quat = XMLoadFloat4(&m_Transform.getRotation());
	XMVECTOR pqua;

	for (Component* parent = this->m_Parent; parent; parent = parent->m_Parent) {
		pqua = XMLoadFloat4(&parent->m_Transform.getRotation());
		quat = XMQuaternionMultiply(quat, pqua);
	}

	return quat;
}

XMVECTOR Component::getForwardVector()
{
	XMVECTOR v = XMVectorSet(0, 0, 1, 0);
	XMMATRIX worldMat = XMLoadFloat4x4(&this->GetWorldTransformMatrix());

	return XMVector3Normalize(XMVector4Transform(v, worldMat));
}

XMVECTOR Component::getRightVector()
{
	XMVECTOR v = XMVectorSet(1, 0, 0, 0);
	XMMATRIX worldMat = XMLoadFloat4x4(&this->GetWorldTransformMatrix());

	return XMVector3Normalize(XMVector4Transform(v, worldMat));
}

XMVECTOR Component::getUpVector()
{
	XMVECTOR v = XMVectorSet(0, 1, 0, 0);
	XMMATRIX worldMat = XMLoadFloat4x4(&this->GetWorldTransformMatrix());

	return XMVector3Normalize(XMVector4Transform(v, worldMat));
}


void Component::setPosition(DirectX::XMFLOAT3 positon)
{
	m_Transform.setPosition(positon);
	this->setDirtyWorldMatrix();
}

void Component::setRotation(DirectX::XMFLOAT4 quat)
{
	m_Transform.setRotation(quat);
	this->setDirtyWorldMatrix();
}

void Component::setScale(DirectX::XMFLOAT3 scale)
{
	m_Transform.setScale(scale);
	this->setDirtyWorldMatrix();
}

void Component::applyRotation(DirectX::XMVECTOR quat)
{
	m_Transform.ApplyRotation(quat);
	this->setDirtyWorldMatrix();
}


void Component::updateWorldMatrix()
{
	if (!m_dirtyWorldMatrix && !m_Transform.isDirty()) return;

	m_dirtyWorldMatrix = false;

	DirectX::XMMATRIX res = DirectX::XMLoadFloat4x4(&m_Transform.toMatrix());
	DirectX::XMMATRIX tmp;

	// Compose parent transforms.
	for (Component* parent = m_Parent; parent; parent = parent->m_Parent)
	{
		tmp = DirectX::XMLoadFloat4x4(&parent->m_Transform.toMatrix());
		res = DirectX::XMMatrixMultiply(res, tmp);
	}

	DirectX::XMStoreFloat4x4(&m_cachedWorldMatrix, res);
}

World* Component::getWorld() const {
	Component* parent = (Component*)this;

	while (parent->m_Parent != NULL)
	{
		parent = parent->m_Parent;
	}
	return dynamic_cast<World*>(parent);
}

void Component::setDirtyWorldMatrix()
{
	m_dirtyWorldMatrix = true;

	for (Component* c = m_FirstChild; c; c = c->m_Sibling) {
		c->setDirtyWorldMatrix();
	}
}

void Component::removeAllAttachments()
{
	Component* PreviousChild = m_Parent->m_FirstChild;

	if (m_Parent->m_FirstChild == this)
	{
		m_Parent->m_FirstChild = this->m_Sibling;
		this->m_Sibling = NULL;
	}
	else
	{
		while (this != PreviousChild->m_Sibling)
		{
			PreviousChild = PreviousChild->m_Sibling;
		}
		PreviousChild->m_Sibling = this->m_Sibling;
		this->m_Sibling = NULL;
	}

	this->m_Parent = NULL;
}*/