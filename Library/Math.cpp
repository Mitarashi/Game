#include <cstdlib>

#include "../Library/Misc.h"

#include "Math.h"

#undef max
#undef min

// ���`�⊮
float Math::Lerp(float min, float max, float time)
{
	return (1.0f - time) * min + time * max;
}

// �w��͈͂̃����_���l���v�Z����
float Math::RandomRange(float min, float max)
{
	_ASSERT_EXPR(max >= min,
		L"The Minimum exceed it than the Maximum!\n"
		L"(�ŏ��l���ő�l�͈̔͂𒴂��Ă��܂�)");

	// 0.0�`1.0�̊Ԃ܂ł̃����_���l
	float value = static_cast<float>(rand()) / RAND_MAX;

	// min�`max�܂ł̃����_���l�ɕϊ�
	return min + (max - min) * value;
}

// �p�x��͈͓��Ɏ��߂�
float Math::AngleNormalize(float radius)
{
	if (radius > PI) radius -= PI * 2;
	if (radius < -PI) radius += PI * 2;

	return radius;
}

// �p�x��͈͓��Ɏ��߂�
Vec2 Math::AngleNormalize(Vec2& radius)
{
	return Vec2(AngleNormalize(radius.x), AngleNormalize(radius.y));
}

// �p�x��͈͓��Ɏ��߂�
Vec3 Math::AngleNormalize(Vec3& radius)
{
	return Vec3(AngleNormalize(radius.x), AngleNormalize(radius.y), AngleNormalize(radius.z));
}

// �g��k���s��Z�o
Math::DxMATRIX Math::MatrixScaling(Vec3& scale)
{
	return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
}

// ��]�s��Z�o
Math::DxMATRIX Math::MatrixRotationQuaternion(Vec4& rotate)
{
	return DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotate));
}

// ���s�ړ��s��Z�o
Math::DxMATRIX Math::MatrixTranslation(Vec3& translate)
{
	return DirectX::XMMatrixTranslation(translate.x, translate.y, translate.z);
}

// �x�N�g���̒����Z�o
float Math::Vector2Length(DxVECTOR& vec)
{
	DxVECTOR Length = DirectX::XMVector2Length(vec);
	float length = 0.0f;
	DirectX::XMStoreFloat(&length, Length);
	return length;
}

// �x�N�g���̒����Z�o
float Math::Vector3Length(DxVECTOR& vec)
{
	DxVECTOR Length = DirectX::XMVector3Length(vec);
	float length = 0.0f;
	DirectX::XMStoreFloat(&length, Length);
	return length;
}

// �x�N�g���̒����Z�o
float Math::Vector4Length(DxVECTOR& vec)
{
	DxVECTOR Length = DirectX::XMVector4Length(vec);
	float length = 0.0f;
	DirectX::XMStoreFloat(&length, Length);
	return length;
}

// ���ς��Z�o
float Math::Vector2Dot(Vec2& v1, Vec2& v2)
{
	float dot = 0.0f;
	DxVECTOR Dot = DirectX::XMVector2Dot(v1.GetXMVector(), v2.GetXMVector());
	DirectX::XMStoreFloat(&dot, Dot);
	return dot;
}

// ���ς��Z�o
float Math::Vector3Dot(Vec3& v1, Vec3& v2)
{
	float dot = 0.0f;
	DxVECTOR Dot = DirectX::XMVector3Dot(v1.GetXMVector(), v2.GetXMVector());
	DirectX::XMStoreFloat(&dot, Dot);
	return dot;
}

// ���ς��Z�o
float Math::Vector4Dot(Vec4& v1, Vec4& v2)
{
	float dot = 0.0f;
	DxVECTOR Dot = DirectX::XMVector4Dot(v1.GetXMVector(), v2.GetXMVector());
	DirectX::XMStoreFloat(&dot, Dot);
	return dot;
}
