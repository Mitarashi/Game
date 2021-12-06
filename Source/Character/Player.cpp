#include "../Library/DxSystem.h"
#include "../Library/Input/InputManager.h"
#include "../Library/Camera/CameraManager.h"
#include "../Library/Shader/LambertShader.h"

#include "Collision.h"
#include "EnemyManager.h"
#include "Player.h"

// コンストラクタ
Player::Player()
{
	model = std::make_unique<Model>("Data/Model/Jummo/Jummo.mdl");

	// モデルが大きいのでスケーリング
	scale = Vec3(0.01f, 0.01f, 0.01f);

	shader = std::make_unique<LambertShader>();

	states.emplace_back(&Player::UpdateIdleState);
	states.emplace_back(&Player::UpdateMoveState);
	states.emplace_back(&Player::UpdateJumpState);
	states.emplace_back(&Player::UpdateLandState);
	states.emplace_back(&Player::UpdateAttackState);
	states.emplace_back(&Player::UpdateDamageState);
	states.emplace_back(&Player::UpdateDeathState);
	states.emplace_back(&Player::UpdateReviveState);

	// 待機ステートへ遷移
	TransitionIdleState();
}

// 更新処理
void Player::Update()
{
	DxSystem& dxSystem = DxSystem::Instance();

	// 更新処理
	(this->*states.at(GetNum(state)))();

	// 速度処理更新
	UpdateVelocity();

	// 無敵時間更新
	UpdateInvincibleTimer();

	// プレイヤーとエネミーとの衝突判定
	CollisionPlayerVsEnemies();

	// オブジェクト行列を更新
	UpdateTransform();

	// モデルアニメーション更新処理
	model->UpdateAnimation(dxSystem.GetDeltaTime());

	// モデル行列更新
	model->UpdateTransform(transform);
}

// 描画処理
void Player::Render(const RenderContext&	rc)
{
	shader->Begin(rc);
	shader->Render(model.get());
	shader->End();
}

// デバッグ用GUI描画
void Player::DebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::InputFloat3("Position", &position.x);
		ImGui::InputFloat3("Angle", &angle.x);
		ImGui::InputFloat3("Scale", &scale.x);
		ImGui::InputFloat("Speed", &moveSpeed);
		ImGui::InputFloat("Radius", &radius);
		ImGui::InputFloat("Height", &height);
		ImGui::InputInt("HP", &hitPoint);
		ImGui::InputFloat(u8"無敵時間", &invincibleTimer);

		ImGui::InputInt(u8"ジャンプ回数", &jumpCount);
		ImGui::InputInt(u8"ジャンプLimit", &jumpLimit);
	}
	ImGui::End();
}

// デバッグプリミティブ描画
void Player::DebugPrimitive()
{
	DebugRenderer* debugRenderer = DxSystem::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, Vec4(0, 0, 0, 1));

	// 攻撃衝突用の左手ノードのデバッグ球を描画
	if (attackCollisionFlag)
	{
		Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
		debugRenderer->DrawSphere(Vec3(
			leftHandBone->worldTransform._41,
			leftHandBone->worldTransform._42,
			leftHandBone->worldTransform._43),
			leftHandRadius,
			Vec4(1, 0, 0, 1)
		);
	}
}

// 着地したときに呼ばれる
void Player::OnLanding()
{
	jumpCount = 0;

	// 下方向の速力が一定以上なら着地ステートへ
	if (velocity.y < gravity * 5.0f)
	{
		// ダメージ、死亡ステート時は着地した時にステート遷移しないようにする
		if (state != State::Damage && state != State::Death)
		{
			// 着地ステートへ遷移
			TransitionLandState();
		}
	}
}

// ダメージを受けた時に呼ばれる
void Player::OnDamage()
{
	// ダメージステートへ遷移
	TransitionDamageState();
}

// 死亡した時に呼ばれる
void Player::OnDead()
{
	// 死亡ステートへ遷移
	TransitionDeathState();
}

// スティック入力値から移動ベクトルを取得
Vec3 Player::GetMoveVec() const
{
	// 入力情報を取得
	GamePad& gamePad = InputManager::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// カメラ方向とスティックの入力値によって進行方向を計算する
	CameraManager& cameraManager = CameraManager::Instance();
	const Vec3& cameraRight = cameraManager.GetMainCamera()->GetRight();
	const Vec3& cameraFront = cameraManager.GetMainCamera()->GetFront();

	// 移動ベクトルはXZ平面に水平なベクトルになるようにする
	// カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// 単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// 単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// スティックの水平入力値をカメラ右方向に反映し、
	// スティックの垂直入力値をカメラ前方向に反映し、
	// 進行ベクトルを計算する
	Vec3 vec = {};
	vec.x = cameraRightX * ax + cameraFrontX * ay;
	vec.z = cameraRightZ * ax + cameraFrontZ * ay;

	// Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;
}

// 移動入力処理
bool Player::InputMove()
{
	// 進行ベクトル取得
	Vec3 moveVec = GetMoveVec();

	// 移動処理
	Move(moveVec.x, moveVec.z, moveSpeed);

	// 旋回処理
	Turn(moveVec.x, moveVec.z, turnSpeed);

	// 進行ベクトルがゼロベクトルでない場合は入力された
	return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

// ジャンプ入力処理
bool Player::InputJump()
{
	GamePad& gamePad = InputManager::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		// ジャンプ回数制限
		if (jumpCount < jumpLimit)
		{
			// ジャンプ
			jumpCount++;
			Jump(jumpSpeed);

			// ジャンプ入力した
			return true;
		}
	}

	return false;
}

// 攻撃入力処理
bool Player::InputAttack()
{
	GamePad& gamePad = InputManager::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		return true;
	}

	return false;
}

// プレイヤーとエネミーとの衝突判定
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// 全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		auto enemy = enemyManager.GetEnemy(i);

		// 衝突判定
		JudgeCylinder c1(*this);
		JudgeCylinder c2(*enemy);
		Vec3 outPosition = {};
		if (Collision::IntersectCylinderVsCylinder(c1, c2, outPosition))
		{
			if (position.y < enemy->GetPosition().y + enemy->GetHeight())
			{
				if (velocity.y >= 0.0f)	enemy->SetPosition(outPosition);
				else
				{
					position.y = enemy->GetPosition().y + enemy->GetHeight();
					velocity.y = 0.0f;
					velocity.y += 10.0f;
				}
			}
		}
	}
}

// ノードと敵の衝突処理
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
	Model::Node* node = model->FindNode(nodeName);

	// ノード位置取得
	Vec3 nodePosition;
	nodePosition.x = node->worldTransform._41;
	nodePosition.y = node->worldTransform._42;
	nodePosition.z = node->worldTransform._43;

	// 指定のノードと全ての敵を総当たりで衝突処理
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		std::shared_ptr<Enemy> enemy = enemyManager.GetEnemy(i);

		// 衝突処理
		Vec3 outPosition;
		JudgeSphere nod(nodePosition, nodeRadius);
		JudgeCylinder enm(*enemy);
		if (Collision::InversectSphereVsSylinder(nod, enm, outPosition))
		{
			// ダメージを与える
			if (enemy->ApplyDamage(1, 0.5f))
			{
				// 吹っ飛ばす
				{
					const float power = 10.0f;
					const DirectX::XMFLOAT3& e = enemy->GetPosition();
					float vx = e.x - nodePosition.x;
					float vz = e.z - nodePosition.z;
					float lengthXZ = sqrtf(vx * vx + vz * vz);
					vx /= lengthXZ;
					vz /= lengthXZ;
					DirectX::XMFLOAT3 impulse;
					impulse.x = vx * power;
					impulse.y = power * 0.5f;
					impulse.z = vz * power;
					enemy->AddImpulse(impulse);
				}

				// ヒットエフェクト再生
				//{
				//	DirectX::XMFLOAT3 e = enemy->GetPosition();
				//	e.y += enemy->GetHeight() * 0.5f;
				//	hitEffect->Play(e);
				//}
			}
		}
	}
}

// 待機ステートへ遷移
void Player::TransitionIdleState()
{
	state = State::Idle;

	// 待機アニメーション再生
	model->PlayAnimation(Anim_Idle, true);
}

// 待機ステート更新処理
void Player::UpdateIdleState()
{
	// 移動入力処理
	if (InputMove())
	{
		// 移動ステートへ遷移
		TransitionMoveState();
	}

	// ジャンプ入力処理
	if (InputJump())
	{
		// ジャンプステートへ遷移
		TransitionJumpState();
	}

	// 攻撃入力処理
	if (InputAttack())
	{
		// 攻撃ステートへ遷移
		TransitionAttackState();
	}
}

// 移動ステートへ遷移
void Player::TransitionMoveState()
{
	state = State::Move;

	// 走りアニメーション再生
	model->PlayAnimation(Anim_Running, true);
}

// 移動ステート更新処理
void Player::UpdateMoveState()
{
	// 移動入力処理
	if (!InputMove())
	{
		// 待機ステートへ遷移
		TransitionIdleState();
	}

	// ジャンプ入力処理
	if (InputJump())
	{
		// ジャンプステートへ遷移
		TransitionJumpState();
	}

	// 攻撃入力処理
	if (InputAttack())
	{
		// 攻撃ステートへ遷移
		TransitionAttackState();
	}
}

// ジャンプステートへ遷移
void Player::TransitionJumpState()
{
	state = State::Jump;

	// ジャンプアニメーション再生
	model->PlayAnimation(Anim_Jump, false);
}

// ジャンプステート更新処理
void Player::UpdateJumpState()
{
	// 移動入力処理
	InputMove();

	// ジャンプ入力処理
	if (InputJump())
	{
		// ジャンプフリップアニメーション再生
		model->PlayAnimation(Anim_Jump_Flip, false);
	}

	// ジャンプアニメーション終了後
	if (!model->IsPlayAnimation())
	{
		// 落下アニメーション再生
		model->PlayAnimation(Anim_Falling, true);
	}
}

// 着地ステートへ遷移
void Player::TransitionLandState()
{
	state = State::Land;
	// 着地アニメーション再生
	model->PlayAnimation(Anim_Landing, false);
}

// 着地ステート更新処理
void Player::UpdateLandState()
{
	// 着地アニメーション終了後
	if (!model->IsPlayAnimation())
	{
		// 待機ステートへ遷移
		TransitionIdleState();
	}
}

// 攻撃ステートへ遷移
void Player::TransitionAttackState()
{
	state = State::Attack;
	// 攻撃アニメーション再生
	model->PlayAnimation(Anim_Attack, false);
}

// 攻撃ステート更新処理
void Player::UpdateAttackState()
{
	// 攻撃アニメーション終了後
	if (!model->IsPlayAnimation())
	{
		// 待機ステートへ遷移
		TransitionIdleState();
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentAnimationSeconds();
	attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
	if (attackCollisionFlag)
	{
		// 左手ノードとエネミーの衝突処理
		CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
	}
}

// ダメージステートへ遷移
void Player::TransitionDamageState()
{
	state = State::Damage;

	// ダメージアニメーション再生
	model->PlayAnimation(Anim_GetHit1, false);
}

// ダメージステート更新処理
void Player::UpdateDamageState()
{
	// ダメージアニメーションが終わったら待機ステートへ遷移
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}

// 死亡ステートへ遷移
void Player::TransitionDeathState()
{
	state = State::Death;

	// 死亡アニメーション再生
	model->PlayAnimation(Anim_Death, false);
}

// 死亡ステート更新処理
void Player::UpdateDeathState()
{
	if (!model->IsPlayAnimation())
	{
		// ボタンを押したら復活ステートへ遷移
		GamePad& gamePad = InputManager::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			TransitionReviveState();
		}
	}
}

// 復活ステートへ遷移
void Player::TransitionReviveState()
{
	state = State::Revive;

	// 体力回復
	hitPoint = maxHitPoint;

	// 復活アニメーション再生
	model->PlayAnimation(Anim_Revive, false);
}

// 復活ステート更新処理
void Player::UpdateReviveState()
{
	// 復活アニメーション終了後に待機ステートへ遷移
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}