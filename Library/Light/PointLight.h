#pragma once

class PointLight
{
public:
	PointLight() = default;
	~PointLight() = default;

	void SetIndex(int index_) { index = static_cast<float>(index_); }
	void SetRange(float range_) { range = range_; }
	void SetType(float type_) { type = type_; }
	void SetPosition(Vec3 position_) { position = Vec4(position_); }
	void SetColor(Vec3 color_) { color = Vec4(color_); }

private:
	float index		= 0.0f;
	float range		= 0.0f;
	float type		= 0.0f;
	float dummy		= 0.0f;
	Vec4 position	= {};
	Vec4 color		= {};
};