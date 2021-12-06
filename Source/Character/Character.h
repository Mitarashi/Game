#pragma once

#include "../Library/Utility/Vector.h"

// キャラクター
class Character
{
public:
	// コンストラクタ
	Character() = default;

	// デストラクタ
	virtual ~Character() = default;

	// 行列更新処理
	void UpdateTransform();

	// getter
	const Vec3&	GetPosition()	 const { return position; }
	const Vec3&	GetAngle()		 const { return angle; }
	const Vec3&	GetScale()		 const { return scale; }
	const float	GetRadius()		 const { return radius; }
	const float	GetHeight()		 const { return height; }
	const bool	IsGround()		 const { return isGround; }
	const int	GetHitPoint()	 const { return hitPoint; }
	const int	GetMaxHitPoint() const { return maxHitPoint; }

	// setter
	void SetPosition(const Vec3& position_)	{ position = position_; }
	void SetAngle(const Vec3& angle_)		{ angle = angle_; }
	void SetScale(const Vec3& scale_)		{ scale = scale_; }

	// ダメージを与える
	virtual bool ApplyDamage(int damage, float invincibleTime);

	// 衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

protected:
	// 移動処理
	void Move(float vx, float vz, float speed);

	// 旋回処理
	void Turn(float vx, float vz, float speed);

	// ジャンプ処理
	void Jump(float speed);

	// 速力処理更新
	void UpdateVelocity();

protected:
	// 着地したときに呼ばれる
	virtual void OnLanding() {};

	// ダメージを受けた時に呼ばれる
	virtual void OnDamage() {};

	// 死亡したときに呼ばれる
	virtual void OnDead() {};

	// 無敵時間更新
	void UpdateInvincibleTimer();

private:
	// 垂直速力更新処理
	void UpdateVerticalVelocity(float deltaFrame);

	// 垂直移動更新処理
	void UpdateVerticalMove();

	// 水平速力更新処理
	void UpdateHorizontalVelocity(float deltaFrame);

	// 水平移動更新処理
	void UpdateHorizontalMove();

protected:
	Vec3				position		= { 0,0,0 };
	Vec3				angle			= { 0,0,0 };
	Vec3				scale			= { 1,1,1 };
	DirectX::XMFLOAT4X4	transform		= { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	float				radius			= 0.5f;
	float				gravity			= -1.0f;
	float				height			= 2.0f;
	Vec3				velocity		= { 0,0,0 };
	bool				isGround		= false;
	float				invincibleTimer = 1.0f;
	float				friction		= 0.5f;
	float				acceleration	= 1.0f;
	float				maxMoveSpeed	= 5.0f;
	float				moveVecX		= 0.0f;
	float				moveVecZ		= 0.0f;
	float				airControl		= 0.3f;
	float				stepOffset		= 1.0f;
	float				slopRate		= 1.0f;
	int					hitPoint		= 5;
	int					maxHitPoint		= 5;
};