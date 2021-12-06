#include "../Library/Math.h"
#include "../Library/DxSystem.h"

#include "../Source/Collision.h"
#include "../Source/Stage/StageManager.h"

#include "Character.h"

// �s��X�V����
void Character::UpdateTransform()
{
	// �X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	// ��]�s����쐬
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;

	// �ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	// ���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;

	// ���[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

// �_���[�W��^����
bool Character::ApplyDamage(int damage, float invincibleTime)
{
	// �_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage == 0) return false;

	// ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (hitPoint <= 0) return false;

	// ���G���Ԃ�����Ƃ��͌��N��Ԃ�ύX���Ȃ�
	if (invincibleTimer > 0.0f) return false;

	// �_���[�W����
	hitPoint -= damage;

	// ���S����
	if (hitPoint <= 0)
	{
		OnDead();
	}
	else
	{
		invincibleTimer = invincibleTime;
		OnDamage();
	}

	// ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
	return true;
}

// �Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// ���͂ɗ͂�������
	velocity.x = impulse.x;
	velocity.y = impulse.y;
	velocity.z = impulse.z;
}

// �ړ�����
void Character::Move(float vx, float vz, float speed)
{
	// �ړ������x�N�g����ݒ�
	moveVecX = vx;
	moveVecZ = vz;

	// �ő呬�x�ݒ�
	maxMoveSpeed = speed;
}

// ���񏈗�
void Character::Turn(float vx, float vz, float speed)
{
	speed *= DxSystem::Instance().GetDeltaTime();

	// �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	DirectX::XMVECTOR V = DirectX::XMVectorSet(vx, 0, vz, 0);//DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(vx, 0, vz));
	if (DirectX::XMVector3Equal(V, DirectX::XMVectorZero())) return;

	// �i�s�x�N�g����P�ʃx�N�g����
	float vecX = vx / sqrtf(vx * vx + vz * vz);
	float vecZ = vz / sqrtf(vx * vx + vz * vz);

	// ���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// ��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = frontX * vecX + frontZ * vecZ;

	// ���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;
	if (rot > speed) rot = speed;

	// ���E������s�����߂�2�̒P�ʃx�N�g�����O�ς��v�Z����
	float cross = frontX * vecZ - frontZ * vecX;

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}
}

// �W�����v����
void Character::Jump(float speed)
{
	// ������̗͂�ݒ�
	velocity.y = speed;
}

// ���͏����X�V
void Character::UpdateVelocity()
{
	// �o�߃t���[��
	float deltaFrame = 60.0f * DxSystem::Instance().GetDeltaTime();

	// �������͍X�V����
	UpdateVerticalVelocity(deltaFrame);

	// �������͍X�V����
	UpdateHorizontalVelocity(deltaFrame);

	// �����ړ��X�V����
	UpdateVerticalMove();

	// �����ړ��X�V����
	UpdateHorizontalMove();
}

// ���G���ԍX�V
void Character::UpdateInvincibleTimer()
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= DxSystem::Instance().GetDeltaTime();
	}

	if (invincibleTimer < 0.0f) invincibleTimer = 0.0f;
}

// �������͍X�V����
void Character::UpdateVerticalVelocity(float deltaFrame)
{
	// �d�͏���
	velocity.y += gravity * deltaFrame;
}

// �����ړ��X�V����
void Character::UpdateVerticalMove()
{
	// ���������̈ړ���
	float my = velocity.y * DxSystem::Instance().GetDeltaTime();

	// �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
	DirectX::XMFLOAT3 normal = { 0, 1, 0 };

	// ������
	if (my < 0.0f)
	{
		// ���C�̊J�n�ʒu�͑�����菭����
		Vec3 start = { position.x, position.y + stepOffset, position.z };

		// ���C�̏I�_�ʒu�͈ړ���̈ʒu
		Vec3 end = { position.x, position.y + my, position.z };

		// ���C�L���X�g�ɂ��n�ʔ���
		HitResult hit = {};
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// �@���x�N�g���擾
			normal = hit.normal;

			// �n�ʂɐڒn���Ă���
			position = hit.position;

			// ��]
			angle.y += hit.rotation.y;

			// �X�Η��̌v�Z
			float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slopRate = 1.0f / (hit.normal.y / (normalLengthXZ + hit.normal.y));

			// ���n����
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = 0.0f;
		}
		else
		{
			// �󒆂ɕ����Ă���
			position.y += my;
			isGround = false;
		}
	}
	// �㏸��
	else if (my > 0.0f)
	{
		position.y += my;
		isGround = false;
	}
}

// �������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ���ʂ̑��͂���������
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// ���C��
		float fric = this->friction * elapsedFrame;

		// �󒆂ɂ��鎞�͖��C�͂����炷
		fric *= airControl;

		// ���C�ɂ�鉡�����̌�������
		if (length > fric)
		{
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * fric;
			velocity.z -= vz * fric;
		}
		// �������̑��͂����C�ȉ��̂Ȃ����̂ő��͂𖳌���
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}

	// XZ���ʂ̑��͂���������
	if (length <= maxMoveSpeed)
	{
		// �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f)
		{
			// ������
			float acc = this->acceleration * elapsedFrame;

			// �󒆂ɂ��鎞�͉����͂����炷
			acc *= airControl;

			// �ړ��x�N�g���ɂ���������
			velocity.x += moveVecX * acc;
			velocity.z += moveVecZ * acc;

			// �ő呬�x����
			float Length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (Length > maxMoveSpeed)
			{
				float vx = velocity.x / Length;
				float vz = velocity.z / Length;

				velocity.x = vx * maxMoveSpeed;
				velocity.z = vz * maxMoveSpeed;
			}

			// �����ŃK�^�K�^���Ȃ��悤�ɂ���
			if (isGround && slopRate > 0.0f)
			{
				velocity.y -= length * slopRate * elapsedFrame;
			}
		}
	}
	// �ړ��x�N�g�������Z�b�g
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

// �����ړ��X�V����
void Character::UpdateHorizontalMove()
{
	DxSystem& dxSystem = DxSystem::Instance();

	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// �����ړ���
		float mx = velocity.x * dxSystem.GetDeltaTime();
		float mz = velocity.z * dxSystem.GetDeltaTime();

		// ���C�̊J�n�ʒu�ƏI�_�ʒu
		Vec3 start = { position.x, position.y + stepOffset, position.z };
		Vec3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

		// ���C�L���X�g�ɂ��ǔ��蔻��
		HitResult hit = {};
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
			// �ǂ̖@��
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			// ���˃x�N�g����@���Ɏˉe
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// �␳�ʒu�̌v�Z
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			Vec3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
			// �ǂ�������փ��C�L���X�g
			HitResult hit2;
			if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
			{
				// �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
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
			// �ړ�
			position.x += mx;
			position.z += mz;
		}
	}
}