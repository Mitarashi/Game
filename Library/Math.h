#pragma once

#include "../Library/Utility/Vector.h"

// 算術
class Math
{
private:
	using DxMATRIX = DirectX::XMMATRIX;
	using DxVECTOR = DirectX::XMVECTOR;

public:
	// 度数法(角度)からに変換する関数
	inline static float (* const ToRadian)(float) = DirectX::XMConvertToRadians;

	// 弧度法(ラジアン)から度数法(角度)に変換する関数
	inline static float (* const ToDegree)(float) = DirectX::XMConvertToDegrees;

public:
	// 線形補完
	static float Lerp(float min, float max, float time);

	// 指定範囲のランダム値を計算する
	static float RandomRange(float min, float max);

	// 角度を範囲内に収める
	static float AngleNormalize(float radius);
	static Vec2	 AngleNormalize(Vec2& radius);
	static Vec3	 AngleNormalize(Vec3& radius);

	// 拡大縮小行列算出
	static DxMATRIX MatrixScaling(Vec3& scale);

	// 回転行列算出
	static DxMATRIX MatrixRotationQuaternion(Vec4& rotate);

	// 平行移動行列算出
	static DxMATRIX MatrixTranslation(Vec3& translate);

	// ベクトルの長さ算出
	static float Vector2Length(DxVECTOR& vec);
	static float Vector3Length(DxVECTOR& vec);
	static float Vector4Length(DxVECTOR& vec);

	// 内積を算出
	static float Vector2Dot(Vec2& v1, Vec2& v2);
	static float Vector3Dot(Vec3& v1, Vec3& v2);
	static float Vector4Dot(Vec4& v1, Vec4& v2);

public:
	static constexpr float PI = 3.141592654f;
};