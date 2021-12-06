#pragma once

#include "../Library/Model/Model.h"
#include "../Library/Utility/Vector.h"
#include "../Source/Character/Character.h"

// ������p�\����
struct JudgeSphere
{
	DirectX::XMFLOAT3 position = {};
	float radius = 0.0f;
	JudgeSphere(Character& chara) : position(chara.GetPosition()), radius(chara.GetRadius()) {};
	JudgeSphere(DirectX::XMFLOAT3 pos, float radius) : position(pos), radius(radius) {};
};

// �~������p�\����
struct JudgeCylinder
{
	Vec3	position = {};
	float	radius = 0.0f;
	float	height = 0.0f;

	JudgeCylinder(Character& chara) : position(chara.GetPosition()), radius(chara.GetRadius()), height(chara.GetHeight()) {}
};

// �q�b�g����
struct HitResult
{
	Vec3	position		= {};
	Vec3	normal			= {};
	Vec3	rotation		= {};
	float	distance		= 0.0f;
	int		materialIndex	= -1;
};

// �R���W����
class Collision
{
public:
	// �~���Ɖ~���̌�������
	static bool IntersectCylinderVsCylinder(JudgeCylinder& cylinderA, JudgeCylinder& cylinderB, Vec3& outPositionB);

	// ���Ɖ~���̌�������
	static bool InversectSphereVsSylinder(JudgeSphere& sphere, JudgeCylinder& cylinder, Vec3& outCylinderPosition);

	// ���C�ƃ��f���̌�������
	static bool IntersectRayVsModel(const Vec3& start, const Vec3& end, const Model* model, HitResult& result);
};