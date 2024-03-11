#include "Component.h"


using namespace DirectX;


Component::Component()
{
	//init to null
}

Component::~Component()
{
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

	DirectX::XMStoreFloat4x4(&m_cachedWorldMatrix, res);
}

void Component::setDirtyWorldMatrix()
{
	m_dirtyWorldMatrix = true;
}