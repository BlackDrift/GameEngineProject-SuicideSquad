#include "GeometryGenerator.h"
#include <cmath> // Pour les fonctions trigonométriques cosf et sinf
#include <DirectXMath.h> // Pour les types et fonctions DirectX Math

void GeometryGenerator::CreateCylinder(
	float bottomRadius, float topRadius,
	float height, uint32 sliceCount, uint32 stackCount)
{
	MeshData meshData;
	//
	// Build Stacks.
	//
	float stackHeight = height / stackCount;
	// Amount to increment radius as we move up each stack level from
	// bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;
	uint32 ringCount = stackCount + 1;
		// Compute vertices for each stack ring starting at the bottom and
		// moving up.
		for (uint32 i = 0; i < ringCount; ++i)
		{
			float y = -0.5f * height + i * stackHeight;
			float r = bottomRadius + i * radiusStep;
			// vertices of ring
			float dTheta = 2.0f * DirectX::XM_PI / sliceCount;
			for (uint32 j = 0; j <= sliceCount; ++j)
			{
				Vertex vertex;
				float c = cosf(j * dTheta);
				float s = sinf(j * dTheta);
				vertex.Position = DirectX::XMFLOAT3(r * c, y, r * s);
				vertex.TexC.x = (float)j / sliceCount;
				vertex.TexC.y = 1.0f - (float)i / stackCount;
				// Cylinder can be parameterized as follows, where we introduce v
				// parameter that goes in the same direction as the v tex-coord
				// so that the bitangent goes in the same direction as the
				// v tex-coord.
				// Let r0 be the bottom radius and let r1 be the top radius.
				// y(v) = h - hv for v in [0,1].
				// r(v) = r1 + (r0-r1)v
				//
				// x(t, v) = r(v)*cos(t)
				// y(t, v) = h - hv
				// z(t, v) = r(v)*sin(t)
				//
				// dx/dt = -r(v)*sin(t)
				// dy/dt = 0
				// dz/dt = +r(v)*cos(t)
				//
				// dx/dv = (r0-r1)*cos(t)
				// dy/dv = -h
				// dz/dv = (r0-r1)*sin(t)
				// This is unit length.
				vertex.TangentU = DirectX::XMFLOAT3(-s, 0.0f, c);
				float dr = bottomRadius - topRadius;
				DirectX::XMFLOAT3 bitangent(dr * c, -height, dr * s);
				DirectX::XMVECTOR T = XMLoadFloat3(&vertex.TangentU);
				DirectX::XMVECTOR B = XMLoadFloat3(&bitangent);
				DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(T, B));
				XMStoreFloat3(&vertex.Normal, N);
				meshData.Vertices.push_back(vertex);
			}
		}
		uint32 ringVertexCount = sliceCount + 1;

		// Compute indices for each stack.
		for (uint32 i = 0; i < stackCount; ++i)
		{
			for (uint32 j = 0; j < sliceCount; ++j)
			{
				meshData.Indices32.push_back(i * ringVertexCount + j);
				meshData.Indices32.push_back((i + 1) * ringVertexCount + j);
				meshData.Indices32.push_back((i + 1) * ringVertexCount + j + 1);
				meshData.Indices32.push_back(i * ringVertexCount + j);
				meshData.Indices32.push_back((i + 1) * ringVertexCount + j + 1);
				meshData.Indices32.push_back(i * ringVertexCount + j + 1);
			}
		}



		BuildCylinderTopCap(bottomRadius, topRadius, height,
			sliceCount, stackCount, meshData);
		//BuildCylinderBottomCap(bottomRadius, topRadius, height,
			//sliceCount, stackCount, meshData);
}