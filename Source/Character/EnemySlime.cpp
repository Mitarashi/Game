#include "../Library/DxSystem.h"
#include "../Library/Shader/LambertShader.h"

#include "Collision.h"
#include "PlayerManager.h"
#include "EnemySlime.h"

EnemySlime::EnemySlime()
{
	model = std::make_unique<Model>("Data/Model/Slime/Slime.mdl");

	scale = { 0.01f, 0.01f, 0.01f };
	height = 1.0f;

	name = "slime";

	states.emplace_back(&EnemySlime::UpdateWanderState);
	states.emplace_back(&EnemySlime::UpdateIdleState);
	states.emplace_back(&EnemySlime::UpdatePursuitState);
	states.emplace_back(&EnemySlime::UpdateAttackState);
	states.emplace_back(&EnemySlime::UpdateIdleBattleState);
	states.emplace_back(&EnemySlime::UpdateDamageState);
	states.emplace_back(&EnemySlime::UpdateDeathState);

	// TODO 引数
	shader = std::make_unique<LambertShader>();

	TransitionWanderState();
}

void EnemySlime::Update()
{
	DxSystem& dxSystem = DxSystem::Instance();

	(this->*states.at(GetNum(state)))();

	// 速力処理更新
	UpdateVelocity();

	// 無敵時間更新
	UpdateInvincibleTimer();

	// オブジェクト行列を更新
	UpdateTransform();

	// モデルアニメーション更新
	model->UpdateAnimation(dxSystem.GetDeltaTime());

	// モデル行列更新
	model->UpdateTransform(transform);
}

void EnemySlime::Render(const RenderContext& rc)
{
	shader->Begin(rc);
	shader->Render(model.get());
	shader->End();
}

void EnemySlime::DebugGUI()
{
	if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		// 位置
		ImGui::InputFloat3("Position", &position.x);
		ImGui::InputFloat3("Angle", &angle.x);
		ImGui::InputFloat3("Scale", &scale.x);
		ImGui::InputFloat("Radius", &radius);
		ImGui::InputFloat("Height", &height);

		//ImGui::InputFloat("Speed", &moveSpeed);
		ImGui::InputInt("HP", &hitPoint);
		ImGui::InputFloat(u8"無敵時間", &invincibleTimer);

		static int anim = 0;
		ImGui::SliderInt("Animation", &anim, 0, 15);
		if (ImGui::Button("AnimationPlay"))
		{
			model->PlayAnimation(anim, false);
		}
	}
}

void EnemySlime::DebugPrimitive()
{
	Enemy::DebugPrimitive();

	DebugRenderer* debugRenderer = DxSystem::Instance().GetDebugRenderer();

	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, Vec4(0, 1, 0, 1));

	debugRenderer->DrawSphere(targetPosition, radius, Vec4(1, 1, 0, 1));

	debugRenderer->DrawCylinder(position, searchRange, 1.0f, Vec4(0, 0, 1, 1));

	debugRenderer->DrawCylinder(position, attackRange, 1.0f, Vec4(1, 0, 0, 1));
}

// ダメージを受けた時に呼ばれる
void EnemySlime::OnDamage()
{
	// ダメージステートへ遷移
	TransitionDamageState();
}

// 死亡した時に呼ばれる
void EnemySlime::OnDead()
{
	// 死亡ステートへ遷移
	TransitionDeathState();
}

// ターゲット位置をランダム設定
void EnemySlime::SetRandomTargetPosition()
{
	float theta = Math::RandomRange(-Math::PI, Math::PI);
	float range = Math::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// 目標地点へ移動
void EnemySlime::MoveToTarget(float speedRate)
{
	// ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// 移動処理
	Move(vx, vz, moveSpeed * speedRate);
	Turn(vx, vz, turnSpeed * speedRate);
}

// プレイヤー索敵
bool EnemySlime::SearchPlayer()
{
	// プレイヤーとの高低差を考慮して3Dでの距離判定をする
	const Vec3& playerPosition = PlayerManager::Instance().GetPlayer()->GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);

		// 単位ベクトル化
		vx /= distXZ;
		vz /= distXZ;

		// 前方ベクトル
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);

		// ２つのベクトルの内積値で前後判定
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

// ノードとプレイヤーの衝突処理
void EnemySlime::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
	// ノードの位置と当たり判定を行う
	Model::Node* node = model->FindNode(nodeName);
	if (node != nullptr)
	{
		// ノードのワールド座標
		Vec3 nodePosition(
			node->worldTransform._41,
			node->worldTransform._42,
			node->worldTransform._43
		);

		// 当たり判定表示
		DxSystem::Instance().GetDebugRenderer()->DrawSphere(
			nodePosition,
			nodeRadius,
			Vec4(0, 0, 1, 1)
		);

		// プレイヤーと当たり判定
		auto& player = PlayerManager::Instance().GetPlayer();
		JudgeSphere nod(nodePosition, nodeRadius);
		JudgeCylinder pl(*player);
		Vec3 outPosition;
		if (Collision::InversectSphereVsSylinder(nod, pl, outPosition))
		{
			// ダメージを与える
			if (player->ApplyDamage(1, 0.5f))
			{
				// 敵を吹っ飛ばすベクトルを算出
				Vec3 vec;
				vec.x = outPosition.x - nodePosition.x;
				vec.z = outPosition.z - nodePosition.z;
				float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
				vec.x /= length;
				vec.z /= length;

				// XZ平面に吹っ飛ばす力をかける
				float power = 10.0f;
				vec.x *= power;
				vec.z *= power;

				// Y方向  にも力をかける
				vec.y = 5.0f;

				// 吹っ飛ばす
				player->AddImpulse(vec);
			}
		}
	}
}

// 徘徊ステートへ遷移
void EnemySlime::TransitionWanderState()
{
	state = State::Wander;

	// 目標地点設定
	SetRandomTargetPosition();

	// 歩きアニメーション再生
	model->PlayAnimation(Anim_WalkFWD, true);
}

// 徘徊ステート更新処理
void EnemySlime::UpdateWanderState()
{
	// 目標地点までXZ平面 での距離判定
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		// 待機ステートへ遷移
		TransitionIdleState();
	}

	// 目標地点へ移動
	MoveToTarget(0.5f);

	// プレイヤー索敵
	if (SearchPlayer())
	{
		// 見つかったら追跡ステートへ遷移
		TransitionPursuitState();
	}
}

// 待機ステートへ遷移
void EnemySlime::TransitionIdleState()
{
	state = State::Idle;

	// タイマーをランダム設定
	stateTimer = Math::RandomRange(3.0f, 5.0f);

	// 待機アニメーション再生
	model->PlayAnimation(Anim_IdleNormal, true);
}

// 待機ステート更新処理
void EnemySlime::UpdateIdleState()
{
	// タイマー処理
	stateTimer -= DxSystem::Instance().GetDeltaTime();
	if (stateTimer < 0.0f)
	{
		// 徘徊ステートへ遷移
		TransitionWanderState();
	}

	// プレイヤー索敵
	if (SearchPlayer())
	{
		// 見つかったら追跡ステートへ遷移
		TransitionPursuitState();
	}
}

// 追跡ステートへ遷移
void EnemySlime::TransitionPursuitState()
{
	state = State::Pursuit;
	// 数秒間追跡するタイマーをランダム設定
	stateTimer = Math::RandomRange(3.0f, 5.0f);

	// 歩きアニメーション再生
	model->PlayAnimation(Anim_RunFWD, true);
}

// 追跡ステート更新処理
void EnemySlime::UpdatePursuitState()
{
	// 目標地点をプレイヤー位置に設定
	targetPosition = PlayerManager::Instance().GetPlayer()->GetPosition();

	// 目標地点へ移動
	MoveToTarget(1.0f);

	// タイマー処理
	stateTimer -= DxSystem::Instance().GetDeltaTime();
	if (stateTimer < 0.0f)
	{
		// 待機ステートへ遷移
		TransitionIdleState();
	}

	// プレイヤーの近くづくと攻撃ステートへ遷移
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < attackRange)
	{
		// 攻撃ステートへ遷移
		TransitionAttackState();
	}
}

// 攻撃ステートへ遷移
void EnemySlime::TransitionAttackState()
{
	state = State::Attack;

	// 攻撃アニメーション再生
	model->PlayAnimation(Anim_Attack1, false);
}

// 攻撃ステート更新処理
void EnemySlime::UpdateAttackState()
{
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentAnimationSeconds();
	if (animationTime >= 0.1f && animationTime <= 0.35f)
	{
		// 目玉ノードとプレイヤーの衝突処理
		CollisionNodeVsPlayer("EyeBall", 0.2f);
	}

	// 攻撃アニメーションが終わったら戦闘待機ステートへ遷移
	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}
}

// 戦闘待機ステートへ遷移
void EnemySlime::TransitionIdleBattleState()
{
	state = State::IdleBattle;

	// 数秒間待機するタイマーをランダム設定
	stateTimer = Math::RandomRange(2.0f, 3.0f);

	// 戦闘待機アニメーション再生
	model->PlayAnimation(Anim_IdleBattle, true);
}

// 戦闘待機ステート更新処理
void EnemySlime::UpdateIdleBattleState()
{
	// 目標地点をプレイヤー位置に設定
	targetPosition = PlayerManager::Instance().GetPlayer()->GetPosition();

	// タイマー処理
	stateTimer -= DxSystem::Instance().GetDeltaTime();

	if (stateTimer < 0.0f)
	{
		// プレイヤーが攻撃範囲にいた場合は攻撃ステートへ遷移
		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);

		if (dist < attackRange)
		{
			// 攻撃ステートへ遷移
			TransitionAttackState();
		}
		else
		{
			// 徘徊ステートへ遷移
			TransitionWanderState();
		}
	}
	MoveToTarget(0.0f);
}

// ダメージステートへ遷移
void EnemySlime::TransitionDamageState()
{
	state = State::Damage;

	// ダメージアニメーション再生
	model->PlayAnimation(Anim_GetHit, false);
}

// ダメージステート更新処理
void EnemySlime::UpdateDamageState()
{
	// ダメージアニメーションが終わったら戦闘待機ステートへ遷移
	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}
}

// 死亡ステートへ遷移
void EnemySlime::TransitionDeathState()
{
	state = State::Death;

	// ダメージアニメーション再生
	model->PlayAnimation(Anim_Die, false);
}

// 死亡ステート更新処理
void EnemySlime::UpdateDeathState()
{
	// ダメージアニメーションが終わったら自分を破棄
	if (!model->IsPlayAnimation())
	{
		Destroy();
	}
}