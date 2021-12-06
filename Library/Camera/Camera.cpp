#include "../Input/InputManager.h"

#include "Camera.h"

// �w�����������
void Camera::SetLookAt(
	const Vec3& eye_,
	const Vec3& focus_,
	const Vec3& up_)
{
	// ���_�A�����_�A���������r���[�s����쐬
	DirectX::XMVECTOR Eye = eye_.GetXMVector();
	DirectX::XMVECTOR Focus = focus_.GetXMVector();
	DirectX::XMVECTOR Up = up_.GetXMVector();
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	// �r���[���t�s�񉻂��A���[���h�s��ɖ߂�
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world = {};
	DirectX::XMStoreFloat4x4(&world, World);

	// �J�����̕��������o��
	this->right = { world._11, world._12, world._13 };
	this->up = { world._21, world._22, world._23 };
	this->front = { world._31, world._32, world._33 };

	// ���_�A�����_��ۑ�
	eye = eye_;
	focus = focus_;
}

// �������e�ݒ�
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	// ��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s��쐬
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
}

// ���s���e�ݒ�
void Camera::SetOrthographic(float width, float height, float nearZ, float farZ)
{
	DirectX::XMMATRIX Projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
}