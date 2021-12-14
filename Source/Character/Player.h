#pragma once

#include "../Library/Math.h"
#include "../Library/Model/Model.h"
#include "../Library/Shader/Shader.h"

#include "Character.h"

// プレイヤー
class Player : public Character
{
private:
	enum Animation
	{
		Anim_Attack,
		Anim_Death,
		Anim_Falling,
		Anim_GetHit1,
		Anim_GetHit2,
		Anim_Idle,
		Anim_Jump,
		Anim_Jump_Flip,
		Anim_Landing,
		Anim_Revive,
		Anim_Running,
		Anim_Walking
	};

	// ステート
	enum class State
	{
		Idle,
		Move,
		Jump,
		Land,
		Attack,
		Damage,
		Death,
		Revive
	};

public:
	// コンストラクタ
	Player();

	// デストラクタ
	~Player() override = default;

	// 更新処理
	void Update();

	// 描画処理
	void Render(const RenderContext& renderContext);

	// デバッグ用GUI描画
	void DebugGUI();

	// デバッグプリミティブ描画
	void DebugPrimitive();

private:
	// 着地したときに呼ばれる
	void OnLanding() override;

	// ダメージを受けた時に呼ばれる
	void OnDamage() override;

	// 死亡した時に呼ばれる
	void OnDead() override;

private:
	// スティック入力値から移動ベクトルを取得
	Vec3 GetMoveVec() const;

	// 移動入力処理
	bool InputMove();

	// ジャンプ入力処理
	bool InputJump();

	// 攻撃入力処理
	bool InputAttack();

	// プレイヤーとエネミーとの衝突判定
	void CollisionPlayerVsEnemies();

	// ノードとエネミーの衝突判定
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

	// 待機ステートへ遷移
	void TransitionIdleState();

	// 待機ステート更新処理
	void UpdateIdleState();

	// 移動ステートへ遷移
	void TransitionMoveState();

	// 移動ステート更新処理
	void UpdateMoveState();

	// ジャンプステートへ遷移
	void TransitionJumpState();

	// ジャンプステート更新処理
	void UpdateJumpState();

	// 着地ステートへ遷移
	void TransitionLandState();

	// 着地ステート更新処理
	void UpdateLandState();

	// 攻撃ステートへ遷移
	void TransitionAttackState();

	// 攻撃ステート更新処理
	void UpdateAttackState();

	// ダメージステートへ遷移
	void TransitionDamageState();

	// ダメージステート更新処理
	void UpdateDamageState();

	// 死亡ステートへ遷移
	void TransitionDeathState();

	// 死亡ステート更新処理
	void UpdateDeathState();

	// 復活ステートへ遷移
	void TransitionReviveState();

	// 復活ステート更新処理
	void UpdateReviveState();

private:
	using States = void (Player::*)();

	std::vector<States> states = {};

private:
	float					moveSpeed			= 5.0f;
	float					turnSpeed			= Math::ToRadian(720);
	float					jumpSpeed			= 20.0f;
	int						jumpCount			= 0;
	int						jumpLimit			= 2;
	State					state				= State::Idle;
	float					leftHandRadius		= 0.4f;
	bool					attackCollisionFlag = false;
};