#pragma once

#include "../Library/Utility/Vector.h"

// �Z�p
class Math
{
private:
	using DxMATRIX = DirectX::XMMATRIX;
	using DxVECTOR = DirectX::XMVECTOR;

public:
	// �x���@(�p�x)����ɕϊ�����֐�
	inline static float (* const ToRadian)(float) = DirectX::XMConvertToRadians;

	// �ʓx�@(���W�A��)����x���@(�p�x)�ɕϊ�����֐�
	inline static float (* const ToDegree)(float) = DirectX::XMConvertToDegrees;

public:
	// ���`�⊮
	static float Lerp(float min, float max, float time);

	// �w��͈͂̃����_���l���v�Z����
	static float RandomRange(float min, float max);

	// �p�x��͈͓��Ɏ��߂�
	static float AngleNormalize(float radius);
	static Vec2	 AngleNormalize(Vec2& radius);
	static Vec3	 AngleNormalize(Vec3& radius);

	// �g��k���s��Z�o
	static DxMATRIX MatrixScaling(Vec3& scale);

	// ��]�s��Z�o
	static DxMATRIX MatrixRotationQuaternion(Vec4& rotate);

	// ���s�ړ��s��Z�o
	static DxMATRIX MatrixTranslation(Vec3& translate);

	// �x�N�g���̒����Z�o
	static float Vector2Length(DxVECTOR& vec);
	static float Vector3Length(DxVECTOR& vec);
	static float Vector4Length(DxVECTOR& vec);

	// ���ς��Z�o
	static float Vector2Dot(Vec2& v1, Vec2& v2);
	static float Vector3Dot(Vec3& v1, Vec3& v2);
	static float Vector4Dot(Vec4& v1, Vec4& v2);

public:
	static constexpr float PI = 3.141592654f;
};