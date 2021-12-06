#pragma once

#include <DirectXMath.h>

class Vec2 : public DirectX::XMFLOAT2
{
public:
	// コンストラクタ
	Vec2() : DirectX::XMFLOAT2(0, 0) {}
	Vec2(float x, float y) : DirectX::XMFLOAT2(x, y) {}
	Vec2(const Vec2& v) { x = v.x; y = v.y; }
	Vec2(Vec2& v) : DirectX::XMFLOAT2(v.x, v.y) {}

	// デストラクタ
	~Vec2() = default;

	// 演算子のオーバーロード
	const Vec2& operator=(const Vec2& v);

	const Vec2& operator+=(const Vec2& v);
	const Vec2& operator-=(const Vec2& v);
	const Vec2& operator*=(const Vec2& v);
	const Vec2& operator*=(float f);
	const Vec2& operator/=(const Vec2& v);
	const Vec2& operator/=(float f);

	const Vec2 operator+() const;
	const Vec2 operator-() const;

	const Vec2 operator+(const Vec2& v) const;
	const Vec2 operator-(const Vec2& v) const;
	const Vec2 operator*(const float f) const;
	const Vec2 operator*(const Vec2& v) const;
	const Vec2 operator/(const float f) const;
	const Vec2 operator/(const Vec2& v) const;

	const bool operator==(const Vec2& v) const;
	const bool operator!=(const Vec2& v) const;

	const bool operator>(const Vec2& v) const;
	const bool operator<(const Vec2& v) const;
	const bool operator>=(const Vec2& v) const;
	const bool operator<=(const Vec2& v) const;

	const float Length() const;
	const float LengthSq() const;
	const Vec2 Normalize() const;
	const DirectX::XMVECTOR GetXMVector() const { return DirectX::XMLoadFloat2(this); };
};

class Vec3 : public DirectX::XMFLOAT3
{
public:
	// コンストラクタ
	Vec3() : DirectX::XMFLOAT3(0, 0, 0) {}
	Vec3(float x, float y, float z) : DirectX::XMFLOAT3(x, y, z) {}
	Vec3(const Vec3& v) : DirectX::XMFLOAT3(v.x, v.y, v.z) {}
	Vec3(DirectX::XMVECTOR v) { DirectX::XMStoreFloat3(this, v); }

	// デストラクタ
	~Vec3() = default;

	// 演算子のオーバーロード
	const Vec3& operator=(const Vec3& v);

	const Vec3& operator+=(const Vec3& v);
	const Vec3& operator-=(const Vec3& v);
	const Vec3& operator*=(const Vec3& v);
	const Vec3& operator*=(float f);
	const Vec3& operator/=(const Vec3& v);
	const Vec3& operator/=(float f);

	const Vec3 operator+() const;
	const Vec3 operator-() const;

	const Vec3 operator+(const Vec3& v) const;
	const Vec3 operator-(const Vec3& v) const;
	const Vec3 operator*(const float f) const;
	const Vec3 operator*(const Vec3& v) const;
	const Vec3 operator/(const float f) const;
	const Vec3 operator/(const Vec3& v) const;

	const bool operator==(const Vec3& v) const;
	const bool operator!=(const Vec3& v) const;

	const bool operator>(const Vec3& v) const;
	const bool operator<(const Vec3& v) const;
	const bool operator>=(const Vec3& v) const;
	const bool operator<=(const Vec3& v) const;

	const float Length() const;
	const float LengthSq() const;
	const Vec3 Normalize() const;
	const DirectX::XMVECTOR GetXMVector() const { return DirectX::XMLoadFloat3(this); };
};

class Vec4 : public DirectX::XMFLOAT4
{
public:
	// コンストラクタ
	Vec4() : DirectX::XMFLOAT4(0, 0, 0, 0) {}
	Vec4(float x, float y, float z, float w) : DirectX::XMFLOAT4(x, y, z, w) {}
	Vec4(const Vec3& v, float w = (0.0f)) : DirectX::XMFLOAT4(v.x, v.y, v.z, w) {}
	Vec4(const Vec4& v) : DirectX::XMFLOAT4(v.x, v.y, v.z, v.w) {}

	// デストラクタ
	~Vec4() = default;

	// 演算子のオーバーロード
	const Vec4& operator=(const Vec4& v);

	const Vec4& operator+=(const Vec4& v);
	const Vec4& operator-=(const Vec4& v);
	const Vec4& operator*=(const Vec4& v);
	const Vec4& operator*=(float f);
	const Vec4& operator/=(const Vec4& v);
	const Vec4& operator/=(float f);

	const Vec4 operator+() const;
	const Vec4 operator-() const;

	const Vec4 operator+(const Vec4& v) const;
	const Vec4 operator-(const Vec4& v) const;
	const Vec4 operator*(const float f) const;
	const Vec4 operator*(const Vec4& v) const;
	const Vec4 operator/(const float f) const;
	const Vec4 operator/(const Vec4& v) const;

	const bool operator==(const Vec4& v) const;
	const bool operator!=(const Vec4& v) const;

	const bool operator>(const Vec4& v) const;
	const bool operator<(const Vec4& v) const;
	const bool operator>=(const Vec4& v) const;
	const bool operator<=(const Vec4& v) const;

	const float Length() const;
	const float LengthSq() const;
	const Vec4 Normalize() const;
	const DirectX::XMVECTOR GetXMVector() const { return DirectX::XMLoadFloat4(this); };
};