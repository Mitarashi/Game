#include <cstdlib>

#include "../Library/Misc.h"

#include "Math.h"

#undef max
#undef min

// 線形補完
float Math::Lerp(float min, float max, float time)
{
	return (1.0f - time) * min + time * max;
}

// 指定範囲のランダム値を計算する
float Math::RandomRange(float min, float max)
{
	_ASSERT_EXPR(max >= min,
		L"The Minimum exceed it than the Maximum!\n"
		L"(最小値が最大値の範囲を超えています)");

	// 0.0～1.0の間までのランダム値
	float value = static_cast<float>(rand()) / RAND_MAX;

	// min～maxまでのランダム値に変換
	return min + (max - min) * value;
}

// 角度を範囲内に収める
float Math::AngleNormalize(float radius)
{
	if (radius > PI) radius -= PI * 2;
	if (radius < -PI) radius += PI * 2;

	return radius;
}

// 角度を範囲内に収める
Vec2 Math::AngleNormalize(Vec2& radius)
{
	return Vec2(AngleNormalize(radius.x), AngleNormalize(radius.y));
}

// 角度を範囲内に収める
Vec3 Math::AngleNormalize(Vec3& radius)
{
	return Vec3(AngleNormalize(radius.x), AngleNormalize(radius.y), AngleNormalize(radius.z));
}

// 拡大縮小行列算出
Math::DxMATRIX Math::MatrixScaling(Vec3& scale)
{
	return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
}

// 回転行列算出
Math::DxMATRIX Math::MatrixRotation(Vec3& angle)
{
	DxMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DxMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DxMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	return Y * X * Z;
}

// 回転行列算出
Math::DxMATRIX Math::MatrixRotationQuaternion(Vec4& rotate)
{
	return DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotate));
}

// 平行移動行列算出
Math::DxMATRIX Math::MatrixTranslation(Vec3& position)
{
	return DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

// ベクトルの長さ算出
float Math::Vector2Length(DxVECTOR& vec)
{
	DxVECTOR Length = DirectX::XMVector2Length(vec);
	float length = 0.0f;
	DirectX::XMStoreFloat(&length, Length);
	return length;
}

// ベクトルの長さ算出
float Math::Vector3Length(DxVECTOR& vec)
{
	DxVECTOR Length = DirectX::XMVector3Length(vec);
	float length = 0.0f;
	DirectX::XMStoreFloat(&length, Length);
	return length;
}

// ベクトルの長さ算出
float Math::Vector4Length(DxVECTOR& vec)
{
	DxVECTOR Length = DirectX::XMVector4Length(vec);
	float length = 0.0f;
	DirectX::XMStoreFloat(&length, Length);
	return length;
}

// 内積を算出
float Math::Vector2Dot(Vec2& v1, Vec2& v2)
{
	float dot = 0.0f;
	DxVECTOR Dot = DirectX::XMVector2Dot(v1.GetXMVector(), v2.GetXMVector());
	DirectX::XMStoreFloat(&dot, Dot);
	return dot;
}

// 内積を算出
float Math::Vector3Dot(Vec3& v1, Vec3& v2)
{
	float dot = 0.0f;
	DxVECTOR Dot = DirectX::XMVector3Dot(v1.GetXMVector(), v2.GetXMVector());
	DirectX::XMStoreFloat(&dot, Dot);
	return dot;
}

// 内積を算出
float Math::Vector4Dot(Vec4& v1, Vec4& v2)
{
	float dot = 0.0f;
	DxVECTOR Dot = DirectX::XMVector4Dot(v1.GetXMVector(), v2.GetXMVector());
	DirectX::XMStoreFloat(&dot, Dot);
	return dot;
}
