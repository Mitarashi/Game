#pragma once

#include "../Library/Math.h"
#include "../Library/Model/Model.h"

#include "../Source/Character/Enemy.h"

// 前方宣言
class Shader;

// エネミーミュータント
class EnemyMutant : public Enemy
{
private:
	// ステート
	enum class State
	{
		Wander,
		Idle,
		Pursuit,
		Attack,
		IdleBattle,
		Damage,
		Death
	};

	// アニメーション
	enum Animation
	{
		Anim_TPOSE,
		Anim_Idle,
		Anim_Walk,
		Anim_Run,
		Anim_Attack1,
		Anim_Attack2,
		Anim_Attack3,
		Anim_GetHit,
		Anim_Die
	};

public:
	// コンストラクタ
	EnemyMutant();

	// デストラクタ
	~EnemyMutant() override = default;

	// 更新処理
	void Update() override;

	// 描画処理
	void Render(const RenderContext& renderContext) override;

	// デバッグ用GUI描画
	void DebugGUI();

	// デバッグプリミティブ描画
	void DebugPrimitive() override;

	// 縄張り設定
	void SetTerritory(const Vec3& origin, float range) { territoryOrigin = origin; territoryRange = range; }

protected:
	// ダメージを受けた時に呼ばれる
	void OnDamage() override;

	// 死亡した時に呼ばれる
	void OnDead() override;

private:
	// ターゲット位置をランダム設定
	void SetRandomTargetPosition();

	// 目標地点へ移動
	void MoveToTarget(float speedRate);

	// プレイヤー索敵
	bool SearchPlayer();

	// ノードとプレイヤーの衝突処理
	void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

	// 徘徊ステートへ遷移
	void TransitionWanderState();

	// 徘徊ステート更新処理
	void UpdateWanderState();

	// 待機ステートへ遷移
	void TransitionIdleState();

	// 待機ステート更新処理
	void UpdateIdleState();

	// 追跡ステートへ遷移
	void TransitionPursuitState();

	// 追跡ステート更新処理
	void UpdatePursuitState();

	// 攻撃ステートへ遷移
	void TransitionAttackState();

	// 攻撃ステート更新処理
	void UpdateAttackState();

	// 戦闘待機ステートへ遷移
	void TransitionIdleBattleState();

	// 戦闘待機ステート更新処理
	void UpdateIdleBattleState();

	// ダメージステートへ遷移
	void TransitionDamageState();

	// ダメージステート更新処理
	void UpdateDamageState();

	// 死亡ステートへ遷移
	void TransitionDeathState();

	// 死亡ステート更新処理
	void UpdateDeathState();

private:
	using States = void (EnemyMutant::*)();

	std::vector<States> states = {};
private:
	std::unique_ptr<Model>	model = nullptr;
	std::unique_ptr<Shader>	shader = nullptr;
	State					state = State::Wander;
	Vec3					targetPosition = { 0, 0, 0 };
	Vec3					territoryOrigin = { 0, 0, 0 };
	float					territoryRange = 10.0f;
	float					moveSpeed = 3.0f;
	float					turnSpeed = Math::ToRadian(360);
	float					stateTimer = 0.0f;
	float					searchRange = 5.0f;
	float					attackRange = 1.5f;
};