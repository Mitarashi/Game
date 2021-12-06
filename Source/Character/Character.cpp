#include "../Library/Math.h"
#include "../Library/DxSystem.h"

#include "../Source/Collision.h"
#include "../Source/Stage/StageManager.h"

#include "Character.h"

// 行列更新処理
void Character::UpdateTransform()
{
	// スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	// 回転行列を作成
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;

	// 位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	// ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;

	// ワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

// ダメージを与える
bool Character::ApplyDamage(int damage, float invincibleTime)
{
	// ダメージが0の場合は健康状態を変更する必要がない
	if (damage == 0) return false;

	// 死亡している場合は健康状態を変更しない
	if (hitPoint <= 0) return false;

	// 無敵時間があるときは健康状態を変更しない
	if (invincibleTimer > 0.0f) return false;

	// ダメージ処理
	hitPoint -= damage;

	// 死亡処理
	if (hitPoint <= 0)
	{
		OnDead();
	}
	else
	{
		invincibleTimer = invincibleTime;
		OnDamage();
	}

	// 健康状態が変更した場合はtrueを返す
	return true;
}

// 衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// 速力に力を加える
	velocity.x = impulse.x;
	velocity.y = impulse.y;
	velocity.z = impulse.z;
}

// 移動処理
void Character::Move(float vx, float vz, float speed)
{
	// 移動方向ベクトルを設定
	moveVecX = vx;
	moveVecZ = vz;

	// 最大速度設定
	maxMoveSpeed = speed;
}

// 旋回処理
void Character::Turn(float vx, float vz, float speed)
{
	speed *= DxSystem::Instance().GetDeltaTime();

	// 進行ベクトルがゼロベクトルの場合は処理する必要なし
	DirectX::XMVECTOR V = DirectX::XMVectorSet(vx, 0, vz, 0);//DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(vx, 0, vz));
	if (DirectX::XMVector3Equal(V, DirectX::XMVectorZero())) return;

	// 進行ベクトルを単位ベクトル化
	float vecX = vx / sqrtf(vx * vx + vz * vz);
	float vecZ = vz / sqrtf(vx * vx + vz * vz);

	// 自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// 回転角を求めるため、2つの単位ベクトルの内積を計算する
	float dot = frontX * vecX + frontZ * vecZ;

	// 内積値は-1.0~1.0で表現されており、2つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot;
	if (rot > speed) rot = speed;

	// 左右判定を行うために2つの単位ベクトルを外積を計算する
	float cross = frontX * vecZ - frontZ * vecX;

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える
	// 左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}
}

// ジャンプ処理
void Character::Jump(float speed)
{
	// 上方向の力を設定
	velocity.y = speed;
}

// 速力処理更新
void Character::UpdateVelocity()
{
	// 経過フレーム
	float deltaFrame = 60.0f * DxSystem::Instance().GetDeltaTime();

	// 垂直速力更新処理
	UpdateVerticalVelocity(deltaFrame);

	// 水平速力更新処理
	UpdateHorizontalVelocity(deltaFrame);

	// 垂直移動更新処理
	UpdateVerticalMove();

	// 水平移動更新処理
	UpdateHorizontalMove();
}

// 無敵時間更新
void Character::UpdateInvincibleTimer()
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= DxSystem::Instance().GetDeltaTime();
	}

	if (invincibleTimer < 0.0f) invincibleTimer = 0.0f;
}

// 垂直速力更新処理
void Character::UpdateVerticalVelocity(float deltaFrame)
{
	// 重力処理
	velocity.y += gravity * deltaFrame;
}

// 垂直移動更新処理
void Character::UpdateVerticalMove()
{
	// 垂直方向の移動量
	float my = velocity.y * DxSystem::Instance().GetDeltaTime();

	// キャラクターのY軸方向となる法線ベクトル
	DirectX::XMFLOAT3 normal = { 0, 1, 0 };

	// 落下中
	if (my < 0.0f)
	{
		// レイの開始位置は足元より少し上
		Vec3 start = { position.x, position.y + stepOffset, position.z };

		// レイの終点位置は移動後の位置
		Vec3 end = { position.x, position.y + my, position.z };

		// レイキャストによる地面判定
		HitResult hit = {};
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// 法線ベクトル取得
			normal = hit.normal;

			// 地面に接地している
			position = hit.position;

			// 回転
			angle.y += hit.rotation.y;

			// 傾斜率の計算
			float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slopRate = 1.0f / (hit.normal.y / (normalLengthXZ + hit.normal.y));

			// 着地した
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;
		}
		else
		{
			// 空中に浮いている
			position.y += my;
			isGround = false;
		}
	}
	// 上昇中
	else if (my > 0.0f)
	{
		position.y += my;
		isGround = false;
	}
}

// 水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ平面の速力を減速する
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// 摩擦力
		float fric = this->friction * elapsedFrame;

		// 空中にいる時は摩擦力を減らす
		fric *= airControl;

		// 摩擦による横方向の減速処理
		if (length > fric)
		{
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * fric;
			velocity.z -= vz * fric;
		}
		// 横方向の速力が摩擦以下のなったので速力を無効化
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}

	// XZ平面の速力を加速する
	if (length <= maxMoveSpeed)
	{
		// 移動ベクトルがゼロベクトルでないなら加速する
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f)
		{
			// 加速力
			float acc = this->acceleration * elapsedFrame;

			// 空中にいる時は加速力を減らす
			acc *= airControl;

			// 移動ベクトルによる加速処理
			velocity.x += moveVecX * acc;
			velocity.z += moveVecZ * acc;

			// 最大速度制限
			float Length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (Length > maxMoveSpeed)
			{
				float vx = velocity.x / Length;
				float vz = velocity.z / Length;

				velocity.x = vx * maxMoveSpeed;
				velocity.z = vz * maxMoveSpeed;
			}

			// 下り坂でガタガタしないようにする
			if (isGround && slopRate > 0.0f)
			{
				velocity.y -= length * slopRate * elapsedFrame;
			}
		}
	}
	// 移動ベクトルをリセット
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

// 水平移動更新処理
void Character::UpdateHorizontalMove()
{
	DxSystem& dxSystem = DxSystem::Instance();

	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// 水平移動量
		float mx = velocity.x * dxSystem.GetDeltaTime();
		float mz = velocity.z * dxSystem.GetDeltaTime();

		// レイの開始位置と終点位置
		Vec3 start = { position.x, position.y + stepOffset, position.z };
		Vec3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

		// レイキャストによる壁判定判定
		HitResult hit = {};
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
			// 壁の法線
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			// 入射ベクトルを法線に射影
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// 補正位置の計算
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			Vec3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
			// 壁ずり方向へレイキャスト
			HitResult hit2;
			if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
			{
				// 壁ずり方向で壁に当たらなかったら補正位置に移動
				position.x = collectPosition.x;
				position.z = collectPosition.z;
			}
			else
			{
				position.x = hit2.position.x;
				position.z = hit2.position.z;
			}
		}
		else
		{
			// 移動
			position.x += mx;
			position.z += mz;
		}
	}
}