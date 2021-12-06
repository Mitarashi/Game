#pragma once

#include "../Utility/Vector.h"

class SpotLight
{
public:
	SpotLight() = default;
	~SpotLight() = default;

	void SetIndex(int index_)			{ index = static_cast<float>(index_); }
	void SetRange(float range_)			{ range = range_; }
	void SetType(float type_)			{ type = type_; }
	void SetInnerCorn(float inner)		{ innerCorn = inner; }
	void SetOuterCorn(float outer)		{ outerCorn = outer; }
	void SetPosition(Vec3 position_)	{ position = Vec4(position_); }
	void SetColor(Vec3 color_)			{ color = Vec4(color_); }
	void SetDir(Vec3 dir_)				{ dir = Vec4(dir_); }

private:
	float index		= 0.0f;
	float range		= 0.0f;
	float type		= 0.0f;
	float innerCorn = 0.0f;
	float outerCorn = 0.0f;
	Vec3 dummy		= {};
	Vec4 position	= {};
	Vec4 color		= {};
	Vec4 dir		= {};
};