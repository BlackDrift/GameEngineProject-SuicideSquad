//#include "Transform.h"
//// Créer un quaternion pour chaque rotation (delta)
//DirectX::XMFLOAT4 quat;
//D3DXQuaternionRotationAxis(&quat, &m_vDir, roll);
//quatRot = quat;
//D3DXQuaternionRotationAxis(&quat, &m_vRight, pitch);
//quatRot *= quat;
//D3DXQuaternionRotationAxis(&quat, &m_vUp, yaw);
//quatRot *= quat;
//
//// Ajouter la rotation delta à la rotation actuelle de l’objet
//m_quatRot *= quatRot;
//
//// Convertir le quaternion en une matrice de rotation (magique)
//D3DXMATRIX matRot;
//D3DXMatrixRotationQuaternion(&matRot, &m_quatRot);
//
//// Mettre à jour les axes de notre objet (3 vecteurs)
//m_vRight.x = matRot._11;
//m_vRight.y = matRot._12;
//m_vRight.z = matRot._13;
//m_vUp.x = matRot._21;
//m_vUp.y = matRot._22;
//m_vUp.z = matRot._23;
//m_vDir.x = matRot._31;
//m_vDir.y = matRot._32;
//m_vDir.z = matRot._33;
//
