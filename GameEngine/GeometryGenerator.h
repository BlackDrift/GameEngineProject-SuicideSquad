/*#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include ""



class GeometryGenerator
{
public:
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;


	void CreateCylinder(
		float bottomRadius, float topRadius,
		float height, uint32 sliceCount, uint32 stackCount);

	void BuildCylinderTopCap(
		float bottomRadius, float topRadius, float height,
		uint32 sliceCount, uint32 stackCount, MeshData& meshData)
	{
		uint32 baseIndex = (uint32)meshData.Vertices.size();
		float y = 0.5f * height;
		float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
		// Duplicate cap ring vertices because the texture coordinates and
		// normals differ.
		for (uint32 i = 0; i <= sliceCount; ++i)
		{
			float x = topRadius * cosf(i * dTheta);
			float z = topRadius * sinf(i * dTheta);
			// Scale down by the height to try and make top cap texture coord
			// area proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;
			meshData.Vertices.push_back(
				Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
		}
		// Cap center vertex.
		meshData.Vertices.push_back(
			Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
				0.5f));
		// Index of center vertex.
		uint32 centerIndex = (uint32)meshData.Vertices.size() - 1;
		for (uint32 i = 0; i < sliceCount; ++i)
		{
			meshData.Indices32.push_back(centerIndex);
			meshData.Indices32.push_back(baseIndex + i + 1);
			DRAWING IN DIRECT3D PART II 279
				meshData.Indices32.push_back(baseIndex + i);
		}
	}


	struct Vertex {
		Vertex() {}
		Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t, const DirectX::XMFLOAT2& uv) :
			Position(p),
			Normal(n),
			TangentU(t),
			TexC(uv) {}
		Vertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v) :
			Position(px, py, pz),
			Normal(nx, ny, nz),
			TangentU(tx, ty, tz),
			TexC(u, v) {}
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 TangentU;
		DirectX::XMFLOAT2 TexC;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32> Indices32;
		std::vector<uint16>& GetIndices16();
	};

	


private:
	std::vector<uint16> mIndices16;
};*/


