#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

struct Transform
{
    DirectX::XMFLOAT3 vSca;
    DirectX::XMMATRIX mSca;

    DirectX::XMFLOAT3 vDir;
    DirectX::XMFLOAT3 vRight;
    DirectX::XMFLOAT3 vUp;
    DirectX::XMVECTOR qRot;
    DirectX::XMMATRIX mRot;

    DirectX::XMFLOAT3 vPos;
    DirectX::XMMATRIX mPos;

    DirectX::XMMATRIX matrix;
    void FromMatrix(DirectX::XMMATRIX* pMat);
    void UpdateRotationFromVectors();
    void UpdateRotationFromQuaternion();
    void UpdateRotationFromMatrix();
    void UpdateMatrix();
    void Rotate();
    void RotateWorld(DirectX::XMMATRIX* pMatrix);
};
