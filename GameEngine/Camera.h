#pragma once
#include "Component.h"
#include "Window.h"
class CameraComponent : public Component {
private:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMMATRIX viewMatrix;
    DirectX::XMMATRIX projectionMatrix;
    Window oWindow;

public:

    // Initialize camera properties
    void Initialize();
    void SetPosition(float x, float y, float z);

    void SetRotation(float pitch, float yaw, float roll);
};
