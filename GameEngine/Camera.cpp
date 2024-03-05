#include "Component.h"
#include "Camera.h"
#include <DirectXMath.h>
#include <windows.h> // Inclure pour HWND
    // Initialize camera properties
    void CameraComponent::Initialize() {
        position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
        rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
        viewMatrix = DirectX::XMMatrixIdentity();
        // Supposons que l'aspect ratio est 16:9, on peuxx ajuster ceci en fonction de notre fenêtre
        projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, 16.0f / 9.0f, 0.01f, 1000.0f);
    }

    void CameraComponent::SetPosition(float x, float y, float z) {
        position = DirectX::XMFLOAT3(x, y, z);
    }

    void CameraComponent::SetRotation(float pitch, float yaw, float roll) {
        rotation = DirectX::XMFLOAT3(pitch, yaw, roll);
    }
