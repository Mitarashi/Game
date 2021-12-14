#pragma once

#include <memory>

#include "../Library/Vector.h"

// カメラ
class Camera
{
public:
	// コンストラクタ
	Camera() = default;

	// デストラクタ
	~Camera() = default;

	// 指定方向を向く
	void SetLookAt(const Vec3& eye,	const Vec3& focus, const Vec3& up);

	// 透視投影設定
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	// 平行投影設定
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

