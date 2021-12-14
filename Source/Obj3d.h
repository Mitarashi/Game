#pragma once

#include "../Library/Vector.h"
#include "../Library/Shader/Shader.h"

// Obj3d
class Obj3d
{
public:
	// コンストラクタ
	Obj3d() = default;

	// デストラクタ
	virtual ~Obj3d() = default;

	// 行列更新処理
	void UpdateTransform();

	// getter
	const Vec3& GetPosition()	const { return position; }
	const Vec3& GetAngle()		const { return angle; }
	const Vec3& GetScale()		const { return scale; }

	// setter
	void SetPosition(const Vec3& position_) { position = position_; }
	void SetAngle(const Vec3& angle_)		{ angle = angle_; }
	void SetScale(const Vec3& scale_)		{ scale = scale_; }

protected:
	DirectX::XMFLOAT4X4		transform	= { 1, 0, 0, 0,
											0, 1, 0, 0,
											0, 0, 1, 0,
											0, 0, 0, 1 };
	Vec3					position	= { 0, 0, 0 };
	Vec3					angle		= { 0, 0, 0 };
	Vec3					scale		= { 1, 1, 1 };
	Vec3					velocity	= { 0, 0, 0 };
	float					gravity		= -1.0f;

	std::unique_ptr<Model>	model		= nullptr;
	std::unique_ptr<Shader> shader		= nullptr;
};