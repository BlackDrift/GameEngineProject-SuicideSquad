#include "Transform.h"

using namespace DirectX;

AABB::AABB(FXMVECTOR minPoint, FXMVECTOR maxPoint)
{
	XMStoreFloat3(&m_min, minPoint);
	XMStoreFloat3(&m_max, maxPoint);
}

AABB::AABB(const AABB& other)
{
	this->set(other);
}

void AABB::set(const AABB& other)
{
	XMStoreFloat3(&m_min, XMLoadFloat3(&other.m_min));
	XMStoreFloat3(&m_max, XMLoadFloat3(&other.m_max));
}

void AABB::translate(FXMVECTOR t)
{
	XMVECTOR p0, p1;

	p0 = XMLoadFloat3(&m_min); p1 = XMLoadFloat3(&m_max);
	p0 = XMVectorAdd(p0, t);   p1 = XMVectorAdd(p1, t);

	XMStoreFloat3(&m_min, p0); XMStoreFloat3(&m_max, p1);
}

void AABB::rotate(FXMVECTOR quat)
{
	XMVECTOR p0, p1, c[8];

	p0 = XMLoadFloat3(&m_min); p1 = XMLoadFloat3(&m_max);

	// Define corners.
	c[0] = p0;
	c[1] = XMVectorSelect(p0, p1, XMVectorSelectControl(0, 0, 1, 0));
	c[2] = XMVectorSelect(p0, p1, XMVectorSelectControl(1, 0, 1, 0));
	c[3] = XMVectorSelect(p0, p1, XMVectorSelectControl(1, 0, 0, 0));
	c[5] = XMVectorSelect(p0, p1, XMVectorSelectControl(0, 1, 0, 0));
	c[6] = XMVectorSelect(p0, p1, XMVectorSelectControl(0, 1, 1, 0));
	c[4] = p1;
	c[7] = XMVectorSelect(p0, p1, XMVectorSelectControl(1, 1, 0, 0));

	// Transform corners.
	for (int i = 0; i < 8; ++i) {
		c[i] = XMVector3Rotate(c[i], quat);
	}

	// Find min/max.
	p0 = c[0];
	p1 = c[0];
	for (int i = 1; i < 8; ++i) {
		p0 = XMVectorMin(p0, c[i]);
		p1 = XMVectorMax(p1, c[i]);
	}

	XMStoreFloat3(&m_min, p0);
	XMStoreFloat3(&m_max, p1);
}

bool AABB::overlaps(const AABB& o) const
{
	if (m_max.x >= o.m_min.x && m_min.x <= o.m_max.x) {
		if (m_max.y >= o.m_min.y && m_min.y <= o.m_max.y) {
			if (m_max.z >= o.m_min.z && m_min.z <= o.m_max.z) {
				return true;
			}
		}
	}
	return false;
}



void AABB::operator=(const AABB& other)
{
	this->set(other);
}

float Math::clamp(float x, float min, float max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

int Math::randomInt(int min, int max)
{
	return rand() % (max - min) + min;
}

float Math::randomFloat(float min, float max)
{
	return (rand() / (float)RAND_MAX) * (max - min) + min;
}

Transform::Transform()
{
	this->setIdentity();
}

void Transform::setIdentity()
{
	m_position = { 0.0F, 0.0F, 0.0F };
	m_rotation = { 0.0F, 0.0F, 0.0F, 1.0F };
	m_scale = { 1.0F, 1.0F, 1.0F };
}

XMVECTOR Transform::getForwardVector() const
{
	return XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&m_rotation));
}

XMVECTOR Transform::getRightVector() const
{
	return XMVector3Rotate(XMVectorSet(1, 0, 0, 0), XMLoadFloat4(&m_rotation));
}

XMVECTOR Transform::getUpVector() const
{
	return XMVector3Rotate(XMVectorSet(0, 1, 0, 0), XMLoadFloat4(&m_rotation));
}

const XMFLOAT4X4& Transform::toMatrix()
{
	static XMVECTOR zero = XMVectorZero();
	static XMVECTOR idqu = XMQuaternionIdentity();

	if (m_dirty) {
		XMVECTOR translate = XMLoadFloat3(&m_position);
		XMVECTOR rotate = XMLoadFloat4(&m_rotation);
		XMVECTOR scale = XMLoadFloat3(&m_scale);
		XMMATRIX result;

		m_dirty = false;
		result = XMMatrixTransformation(zero, idqu, scale, zero, rotate, translate);

		XMStoreFloat4x4(&m_cachedLocalTransform, result);
	}

	return m_cachedLocalTransform;
}

void Transform::ApplyRotation(XMVECTOR Q)
{
	Q = XMQuaternionMultiply(XMLoadFloat4(&m_rotation), Q);
	XMStoreFloat4(&m_rotation, Q);

	m_dirty = true;
}