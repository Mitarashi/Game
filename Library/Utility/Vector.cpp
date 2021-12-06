#include "Vector.h"

// =
const Vec2& Vec2::operator=(const Vec2& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

// +=
const Vec2& Vec2::operator+=(const Vec2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

// -=
const Vec2& Vec2::operator-=(const Vec2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

// *=
const Vec2& Vec2::operator*=(const Vec2& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

// *=
const Vec2& Vec2::operator*=(float f)
{
	x *= f;
	y *= f;
	return *this;
}

// /=
const Vec2& Vec2::operator/=(const Vec2& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

// /=
const Vec2& Vec2::operator/=(float f)
{
	x /= f;
	y /= f;
	return *this;
}

// +(符号)
const Vec2 Vec2::operator+() const
{
	return Vec2(x, y);
}

// -(符号)
const Vec2 Vec2::operator-() const
{
	return Vec2(-x, -y);
}

// +(和)
const Vec2 Vec2::operator+(const Vec2& v) const
{
	return Vec2(x + v.x, y + v.y);
}

// -(差)
const Vec2 Vec2::operator-(const Vec2& v) const
{
	return Vec2(x - v.x, y - v.y);
}

// *(積)
const Vec2 Vec2::operator*(const float f) const
{
	return Vec2(x * f, y * f);
}

// *(積)
const Vec2 Vec2::operator*(const Vec2& v) const
{
	return Vec2(x * v.x, y * v.y);
}

// /(商)
const Vec2 Vec2::operator/(float f) const
{
	return Vec2(x / f, y / f);
}

// /(商)
const Vec2 Vec2::operator/(const Vec2& v) const
{
	return Vec2(x / v.x, y / v.y);
}

// ==
const bool Vec2::operator==(const Vec2& v) const
{
	return (x == v.x) && (y == v.y);
}

// !=
const bool Vec2::operator!=(const Vec2& v) const
{
	return (x != v.x) || (y != v.y);
}

// >
const bool Vec2::operator>(const Vec2& v) const
{
	return (x > v.x) && (y > v.y);
}

// <
const bool Vec2::operator<(const Vec2& v) const
{
	return (x < v.x) && (y < v.y);
}

// >=
const bool Vec2::operator>=(const Vec2& v) const
{
	return (x >= v.x) && (y >= v.y);
}

// <=
const bool Vec2::operator<=(const Vec2& v) const
{
	return (x <= v.x) && (y <= v.y);
}

// 長さを取得
const float Vec2::Length() const
{
	float length = 0.0f;
	DirectX::XMVECTOR Length = DirectX::XMVector2Length(this->GetXMVector());
	DirectX::XMStoreFloat(&length, Length);
	return length;
}

// 長さを取得
const float Vec2::LengthSq() const
{
	float lengthSq = 0.0f;
	DirectX::XMVECTOR LengthSq = DirectX::XMVector2LengthSq(this->GetXMVector());
	DirectX::XMStoreFloat(&lengthSq, LengthSq);
	return lengthSq;
}

// 単位ベクトルを取得
const Vec2 Vec2::Normalize() const
{
	const float f = (x * x) + (y * y);
	if (f < FLT_EPSILON * FLT_EPSILON) return *this;
	return (*this / f);
}

// =
const Vec3& Vec3::operator=(const Vec3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

// +=
const Vec3& Vec3::operator+=(const Vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

// -=
const Vec3& Vec3::operator-=(const Vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

// *=
const Vec3& Vec3::operator*=(const Vec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

// *=
const Vec3& Vec3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

// /=
const Vec3& Vec3::operator/=(const Vec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

// /=
const Vec3& Vec3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

// +(符号)
const Vec3 Vec3::operator+() const
{
	return Vec3(x, y, z);
}

// -(符号)
const Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

// +(和)
const Vec3 Vec3::operator+(const Vec3& v) const
{
	return Vec3(x + v.x, y + v.y, z + v.z);
}

// -(差)
const Vec3 Vec3::operator-(const Vec3& v) const
{
	return Vec3(x - v.x, y - v.y, z - v.z);
}

// *(積)
const Vec3 Vec3::operator*(const float f) const
{
	return Vec3(x * f, y * f, z * f);
}

// *(積)
const Vec3 Vec3::operator*(const Vec3& v) const
{
	return Vec3(x * v.x, y * v.y, z * v.z);
}

// /(商)
const Vec3 Vec3::operator/(float f) const
{
	return Vec3(x / f, y / f, z / f);
}

// /(商)
const Vec3 Vec3::operator/(const Vec3& v) const
{
	return Vec3(x / v.x, y / v.y, z / v.z);
}

// ==
const bool Vec3::operator==(const Vec3& v) const
{
	return (x == v.x) && (y == v.y) && (z == v.z);
}

// !=
const bool Vec3::operator!=(const Vec3& v) const
{
	return (x != v.x) || (y != v.y) || (z != v.z);
}

// >
const bool Vec3::operator>(const Vec3& v) const
{
	return (x > v.x) && (y > v.y) && (z > v.z);
}

// <
const bool Vec3::operator<(const Vec3& v) const
{
	return (x < v.x) && (y < v.y) && (z < v.z);
}

// >=
const bool Vec3::operator>=(const Vec3& v) const
{
	return (x >= v.x) && (y >= v.y) && (z >= v.z);
}

// <=
const bool Vec3::operator<=(const Vec3& v) const
{
	return (x <= v.x) && (y <= v.y) && (z <= v.z);
}

// 長さを取得
const float Vec3::Length() const
{
	float length = 0.0f;
	DirectX::XMVECTOR Length = DirectX::XMVector3Length(this->GetXMVector());
	DirectX::XMStoreFloat(&length, Length);
	return length;
}

// 長さを取得
const float Vec3::LengthSq() const
{
	float lengthSq = 0.0f;
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(this->GetXMVector());
	DirectX::XMStoreFloat(&lengthSq, LengthSq);
	return lengthSq;
}

// 単位ベクトルを取得
const Vec3 Vec3::Normalize() const
{
	const float f = (x * x) + (y * y) + (z * z);
	if (f < FLT_EPSILON * FLT_EPSILON) return *this;
	return (*this / f);
}

// =
const Vec4& Vec4::operator=(const Vec4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

// +=
const Vec4& Vec4::operator+=(const Vec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

// -=
const Vec4& Vec4::operator-=(const Vec4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

// *=
const Vec4& Vec4::operator*=(const Vec4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

// *=
const Vec4& Vec4::operator*=(float f)
{
	x *= f;
	y *= f;
	z /= f;
	w /= f;
	return *this;
}

// /=
const Vec4& Vec4::operator/=(const Vec4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

// /=
const Vec4& Vec4::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}

// +(符号)
const Vec4 Vec4::operator+() const
{
	return Vec4(x, y, z, w);
}

// -(符号)
const Vec4 Vec4::operator-() const
{
	return Vec4(-x, -y, -z, -w);
}

// +(和)
const Vec4 Vec4::operator+(const Vec4& v) const
{
	return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

// -(差)
const Vec4 Vec4::operator-(const Vec4& v) const
{
	return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

// *(積)
const Vec4 Vec4::operator*(const float f) const
{
	return Vec4(x * f, y * f, z * f, w * f);
}

// *(積)
const Vec4 Vec4::operator*(const Vec4& v) const
{
	return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

// /(商)
const Vec4 Vec4::operator/(float f) const
{
	return Vec4(x / f, y / f, z / f, w / f);
}

// /(商)
const Vec4 Vec4::operator/(const Vec4& v) const
{
	return Vec4(x / v.x, y / v.y, z / v.z, w / v.w);
}

// ==
const bool Vec4::operator==(const Vec4& v) const
{
	return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
}

// !=
const bool Vec4::operator!=(const Vec4& v) const
{
	return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
}

// >
const bool Vec4::operator>(const Vec4& v) const
{
	return (x > v.x) && (y > v.y) && (z > v.z) && (w > v.w);
}

// <
const bool Vec4::operator<(const Vec4& v) const
{
	return (x < v.x) && (y < v.y) && (z < v.z) && (w < v.w);
}

// >=
const bool Vec4::operator>=(const Vec4& v) const
{
	return (x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w);
}

// <=
const bool Vec4::operator<=(const Vec4& v) const
{
	return (x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w);
}

// 長さを取得
const float Vec4::Length() const
{
	float length = 0.0f;
	DirectX::XMVECTOR Length = DirectX::XMVector4Length(this->GetXMVector());
	DirectX::XMStoreFloat(&length, Length);
	return length;
}

// 長さを取得
const float Vec4::LengthSq() const
{
	float lengthSq = 0.0f;
	DirectX::XMVECTOR LengthSq = DirectX::XMVector4LengthSq(this->GetXMVector());
	DirectX::XMStoreFloat(&lengthSq, LengthSq);
	return lengthSq;
}

// 単位ベクトルを取得
const Vec4 Vec4::Normalize() const
{
	const float f = (x * x) + (y * y) + (z * z) + (w * w);
	if (f < FLT_EPSILON * FLT_EPSILON) return *this;
	return (*this / f);
}
