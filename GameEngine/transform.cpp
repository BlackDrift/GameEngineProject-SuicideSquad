#include "Transform.h"
void Transform::Rotate() {
	// Créer un quaternion pour chaque rotation (delta)
	DirectX::XMVECTOR quat = {};
	const DirectX::XMFLOAT3 vDirConst = vDir;
	const DirectX::XMFLOAT3 vRightConst = vRight;
	const DirectX::XMFLOAT3 vUpConst = vUp;
	DirectX::XMVECTOR vDirVect = DirectX::XMLoadFloat3(&vDirConst);
	DirectX::XMVECTOR vRightVect = DirectX::XMLoadFloat3(&vRightConst);
	DirectX::XMVECTOR vUpVect = DirectX::XMLoadFloat3(&vUpConst);
	DirectX::XMVector3Rotate(vDirVect, quat);
	DirectX::XMVECTOR quatRot = quat;
	DirectX::XMVector3Rotate(vRightVect, quat);
	quatRot = DirectX::XMVector3Cross(quatRot, quat);
	DirectX::XMVector3Rotate(vUpVect, quat);
	quatRot = DirectX::XMVector3Cross(quatRot, quat);

	// Ajouter la rotation delta à la rotation actuelle de l’objet
	qRot = DirectX::XMVector3Cross(qRot, quatRot);

	// Convertir le quaternion en une matrice de rotation (magique)
	DirectX::XMMATRIX matRot;
	matRot = DirectX::XMMatrixRotationQuaternion(qRot);

	// Mettre à jour les axes de notre objet (3 vecteurs)
	DirectX::XMFLOAT4X4 matRotFloat; 
	DirectX::XMStoreFloat4x4(&matRotFloat, matRot);
	vRight.x = matRotFloat._11;
	vRight.y = matRotFloat._12;
	vRight.z = matRotFloat._13;
	vUp.x = matRotFloat._21;
	vUp.y = matRotFloat._22;
	vUp.z = matRotFloat._23;
	vDir.x = matRotFloat._31;
	vDir.y = matRotFloat._32;
	vDir.z = matRotFloat._33;
}

void Transform::Init(float x, float y, float z) {
	// Toujours initialiser les matrices, les quaternions, les vecteurs…
	mSca = DirectX::XMMatrixIdentity();
	mPos = DirectX::XMMatrixIdentity();
	mRot = DirectX::XMMatrixIdentity();

	// On peut effectuer des operations sur chaque matrice indépendamment
	mSca = DirectX::XMMatrixScaling(x, y, z);
	mPos = DirectX::XMMatrixTranslation(x, y, z);
	mRot = DirectX::XMMatrixRotationY(3.14f);

	// Combiner les matrices dans cet ordre
	DirectX::XMMATRIX mat = mSca;
	mat *= mRot;
	mat *= mPos;
}
