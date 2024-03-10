#pragma once
#include <DirectXMath.h>
#include <stdlib.h>
#include <DirectXCollision.h>


class AABB
{
public:

	AABB(DirectX::FXMVECTOR minPoint, DirectX::FXMVECTOR maxPoint);
	AABB(const AABB& other);

	void set(const AABB& other);

	void translate(DirectX::FXMVECTOR t);

	/// Rotates this AABB's points around the origin, and constructs another AABB
	/// containing the transformed points.
	void rotate(DirectX::FXMVECTOR quat);

	bool overlaps(const AABB& other) const;


	inline const DirectX::XMFLOAT3& getMin() const { return m_min; }
	inline const DirectX::XMFLOAT3& getMax() const { return m_max; }


	void operator=(const AABB& other);


private:

	DirectX::XMFLOAT3 m_min;
	DirectX::XMFLOAT3 m_max;
};


namespace Math
{
	float clamp(float x, float min, float max);

	int randomInt(int min, int max);

	float randomFloat(float min = 0.0F, float max = 1.0F);
}

class Transform
	{
	public:

		/// Default constructor, initializes this transform to identity.
		Transform();

		/// Set this transform as identity.
		void setIdentity();

		DirectX::XMVECTOR getForwardVector() const;
		DirectX::XMVECTOR getRightVector() const;
		DirectX::XMVECTOR getUpVector() const;

		inline const DirectX::XMFLOAT3& getPosition() const { return m_position; }
		inline const DirectX::XMFLOAT4& getRotation() const { return m_rotation; }
		inline const DirectX::XMFLOAT3& getScale()    const { return m_scale; }

		inline bool isDirty() const { return m_dirty; }

		inline const void setPosition(DirectX::XMFLOAT3 position) { m_position = position; m_dirty = true; }
		inline const void setRotation(DirectX::XMFLOAT4 rotation) { m_rotation = rotation; m_dirty = true; }
		inline const void setScale(DirectX::XMFLOAT3 scale) { m_scale = scale; m_dirty = true; }


		const DirectX::XMFLOAT4X4& toMatrix();

		void ApplyRotation(DirectX::XMVECTOR Q);


	private:

		/// Cached matrix representation of this transform.
		DirectX::XMFLOAT4X4 m_cachedLocalTransform;

		/// Decomposed transform.
		DirectX::XMFLOAT4 m_rotation;
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT3 m_scale;

		/// Dirty flag (should the cached matrix be rebuilt?)
		bool m_dirty = true;
};