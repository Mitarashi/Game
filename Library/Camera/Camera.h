#pragma once

#include <memory>

#include "../Library/Vector.h"

// �J����
class Camera
{
public:
	// �R���X�g���N�^
	Camera() = default;

	// �f�X�g���N�^
	~Camera() = default;

	// �w�����������
	void SetLookAt(const Vec3& eye,	const Vec3& focus, const Vec3& up);

	// �������e�ݒ�
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	// ���s���e�ݒ�
	void SetOrthographic(float width, float height, float nearZ, float farZ);

	// getter
	const DirectX::XMFLOAT4X4&	GetView()		const { return view; }
	const DirectX::XMFLOAT4X4&	GetProjection()	const { return projection; }
	const Vec3&					GetEye()		const { return eye; }
	const Vec3&					GetFocus()		const { return focus; }
	const Vec3&					GetUp()			const {	return up; }
	const Vec3&					GetFront()		const { return front; }
	const Vec3&					GetRight()		const { return right; }

private:
	Vec3				eye			= {};
	Vec3				focus		= {};
	Vec3				front		= {};
	Vec3				up			= {};
	Vec3				right		= {};

	DirectX::XMFLOAT4X4 view		= {};
	DirectX::XMFLOAT4X4 projection	= {};
};

