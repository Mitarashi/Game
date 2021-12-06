#pragma once

#include "../Library/Model/Model.h"
#include "../Library/Utility/Vector.h"
#include "../Source/Character/Character.h"

// 球判定用構造体
struct JudgeSphere
{
	DirectX::XMFLOAT3 position = {};
	float radius = 0.0f;
	JudgeSphere(Character& chara) : position(chara.GetPosition()), radius(chara.GetRadius()) {};
	JudgeSphere(DirectX::XMFLOAT3 pos, float radius) : position(pos), radius(radius) {};
};

// 円柱判定用構造体
struct JudgeCylinder
{
	Vec3	position = {};
	float	radius = 0.0f;
	float	height = 0.0f;

	JudgeCylinder(Character& chara) : position(chara.GetPosition()), radius(chara.GetRadius()), height(chara.GetHeight()) {}
};

// ヒット結果
struct HitResult
{
	Vec3	position		= {};
	Vec3	normal			= {};
	Vec3	rotation		= {};
	float	distance		= 0.0f;
	int		materialIndex	= -1;
};

// コリジョン
class Collision
{
public:
	// 円柱と円柱の交差判定
	static bool IntersectCylinderVsCylinder(JudgeCylinder& cylinderA, JudgeCylinder& cylinderB, Vec3& outPositionB);

	// 球と円柱の交差判定
	static bool InversectSphereVsSylinder(JudgeSphere& sphere, JudgeCylinder& cylinder, Vec3& outCylinderPosition);

	// レイとモデルの交差判定
	static bool IntersectRayVsModel(const Vec3& start, const Vec3& end, const Model* model, HitResult& result);
};