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

	// TODO ����
	shader = std::make_unique<LambertShader>();

	TransitionWanderState();
}

void EnemySlime::Update()
{
	DxSystem& dxSystem = DxSystem::Instance();

	(this->*states.at(GetNum(state)))();

	// ���͏����X�V
	UpdateVelocity();

	// ���G���ԍX�V
	UpdateInvincibleTimer();

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���A�j���[�V�����X�V
	model->UpdateAnimation(dxSystem.GetDeltaTime());

	// ���f���s��X�V
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
		// �ʒu
		ImGui::InputFloat3("Position", &position.x);
		ImGui::InputFloat3("Angle", &angle.x);
		ImGui::InputFloat3("Scale", &scale.x);
		ImGui::InputFloat("Radius", &radius);
		ImGui::InputFloat("Height", &height);

		//ImGui::InputFloat("Speed", &moveSpeed);
		ImGui::InputInt("HP", &hitPoint);
		ImGui::InputFloat(u8"���G����", &invincibleTimer);

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

// �_���[�W���󂯂����ɌĂ΂��
void EnemySlime::OnDamage()
{
	// �_���[�W�X�e�[�g�֑J��
	TransitionDamageState();
}

// ���S�������ɌĂ΂��
void EnemySlime::OnDead()
{
	// ���S�X�e�[�g�֑J��
	TransitionDeathState();
}

// �^�[�Q�b�g�ʒu�������_���ݒ�
void EnemySlime::SetRandomTargetPosition()
{
	float theta = Math::RandomRange(-Math::PI, Math::PI);
	float range = Math::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// �ڕW�n�_�ֈړ�
void EnemySlime::MoveToTarget(float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;

	// �ړ�����
	Move(vx, vz, moveSpeed * speedRate);
	Turn(vx, vz, turnSpeed * speedRate);
}

// �v���C���[���G
bool EnemySlime::SearchPlayer()
{
	// �v���C���[�Ƃ̍��፷���l������3D�ł̋������������
	const Vec3& playerPosition = PlayerManager::Instance().GetPlayer()->GetPosition();
	float vx = playerPosition.x - position.x;
	float vy = playerPosition.y - position.y;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	if (dist < searchRange)
	{
		float distXZ = sqrtf(vx * vx + vz * vz);

		// �P�ʃx�N�g����
		vx /= distXZ;
		vz /= distXZ;

		// �O���x�N�g��
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);

		// �Q�̃x�N�g���̓��ϒl�őO�㔻��
		float dot = (frontX * vx) + (frontZ * vz);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}

// �m�[�h�ƃv���C���[�̏Փˏ���
void EnemySlime::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
	// �m�[�h�̈ʒu�Ɠ����蔻����s��
	Model::Node* node = model->FindNode(nodeName);
	if (node != nullptr)
	{
		// �m�[�h�̃��[���h���W
		Vec3 nodePosition(
			node->worldTransform._41,
			node->worldTransform._42,
			node->worldTransform._43
		);

		// �����蔻��\��
		DxSystem::Instance().GetDebugRenderer()->DrawSphere(
			nodePosition,
			nodeRadius,
			Vec4(0, 0, 1, 1)
		);

		// �v���C���[�Ɠ����蔻��
		auto& player = PlayerManager::Instance().GetPlayer();
		JudgeSphere nod(nodePosition, nodeRadius);
		JudgeCylinder pl(*player);
		Vec3 outPosition;
		if (Collision::InversectSphereVsSylinder(nod, pl, outPosition))
		{
			// �_���[�W��^����
			if (player->ApplyDamage(1, 0.5f))
			{
				// �G�𐁂���΂��x�N�g�����Z�o
				Vec3 vec;
				vec.x = outPosition.x - nodePosition.x;
				vec.z = outPosition.z - nodePosition.z;
				float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
				vec.x /= length;
				vec.z /= length;

				// XZ���ʂɐ�����΂��͂�������
				float power = 10.0f;
				vec.x *= power;
				vec.z *= power;

				// Y����  �ɂ��͂�������
				vec.y = 5.0f;

				// ������΂�
				player->AddImpulse(vec);
			}
		}
	}
}

// �p�j�X�e�[�g�֑J��
void EnemySlime::TransitionWanderState()
{
	state = State::Wander;

	// �ڕW�n�_�ݒ�
	SetRandomTargetPosition();

	// �����A�j���[�V�����Đ�
	model->PlayAnimation(Anim_WalkFWD, true);
}

// �p�j�X�e�[�g�X�V����
void EnemySlime::UpdateWanderState()
{
	// �ڕW�n�_�܂�XZ���� �ł̋�������
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float distSq = vx * vx + vz * vz;
	if (distSq < radius * radius)
	{
		// �ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}

	// �ڕW�n�_�ֈړ�
	MoveToTarget(0.5f);

	// �v���C���[���G
	if (SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		TransitionPursuitState();
	}
}

// �ҋ@�X�e�[�g�֑J��
void EnemySlime::TransitionIdleState()
{
	state = State::Idle;

	// �^�C�}�[�������_���ݒ�
	stateTimer = Math::RandomRange(3.0f, 5.0f);

	// �ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_IdleNormal, true);
}

// �ҋ@�X�e�[�g�X�V����
void EnemySlime::UpdateIdleState()
{
	// �^�C�}�[����
	stateTimer -= DxSystem::Instance().GetDeltaTime();
	if (stateTimer < 0.0f)
	{
		// �p�j�X�e�[�g�֑J��
		TransitionWanderState();
	}

	// �v���C���[���G
	if (SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		TransitionPursuitState();
	}
}

// �ǐՃX�e�[�g�֑J��
void EnemySlime::TransitionPursuitState()
{
	state = State::Pursuit;
	// ���b�ԒǐՂ���^�C�}�[�������_���ݒ�
	stateTimer = Math::RandomRange(3.0f, 5.0f);

	// �����A�j���[�V�����Đ�
	model->PlayAnimation(Anim_RunFWD, true);
}

// �ǐՃX�e�[�g�X�V����
void EnemySlime::UpdatePursuitState()
{
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	targetPosition = PlayerManager::Instance().GetPlayer()->GetPosition();

	// �ڕW�n�_�ֈړ�
	MoveToTarget(1.0f);

	// �^�C�}�[����
	stateTimer -= DxSystem::Instance().GetDeltaTime();
	if (stateTimer < 0.0f)
	{
		// �ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}

	// �v���C���[�̋߂��Â��ƍU���X�e�[�g�֑J��
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	if (dist < attackRange)
	{
		// �U���X�e�[�g�֑J��
		TransitionAttackState();
	}
}

// �U���X�e�[�g�֑J��
void EnemySlime::TransitionAttackState()
{
	state = State::Attack;

	// �U���A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Attack1, false);
}

// �U���X�e�[�g�X�V����
void EnemySlime::UpdateAttackState()
{
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentAnimationSeconds();
	if (animationTime >= 0.1f && animationTime <= 0.35f)
	{
		// �ڋʃm�[�h�ƃv���C���[�̏Փˏ���
		CollisionNodeVsPlayer("EyeBall", 0.2f);
	}

	// �U���A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}
}

// �퓬�ҋ@�X�e�[�g�֑J��
void EnemySlime::TransitionIdleBattleState()
{
	state = State::IdleBattle;

	// ���b�ԑҋ@����^�C�}�[�������_���ݒ�
	stateTimer = Math::RandomRange(2.0f, 3.0f);

	// �퓬�ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_IdleBattle, true);
}

// �퓬�ҋ@�X�e�[�g�X�V����
void EnemySlime::UpdateIdleBattleState()
{
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	targetPosition = PlayerManager::Instance().GetPlayer()->GetPosition();

	// �^�C�}�[����
	stateTimer -= DxSystem::Instance().GetDeltaTime();

	if (stateTimer < 0.0f)
	{
		// �v���C���[���U���͈͂ɂ����ꍇ�͍U���X�e�[�g�֑J��
		float vx = targetPosition.x - position.x;
		float vy = targetPosition.y - position.y;
		float vz = targetPosition.z - position.z;
		float dist = sqrtf(vx * vx + vy * vy + vz * vz);

		if (dist < attackRange)
		{
			// �U���X�e�[�g�֑J��
			TransitionAttackState();
		}
		else
		{
			// �p�j�X�e�[�g�֑J��
			TransitionWanderState();
		}
	}
	MoveToTarget(0.0f);
}

// �_���[�W�X�e�[�g�֑J��
void EnemySlime::TransitionDamageState()
{
	state = State::Damage;

	// �_���[�W�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_GetHit, false);
}

// �_���[�W�X�e�[�g�X�V����
void EnemySlime::UpdateDamageState()
{
	// �_���[�W�A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleBattleState();
	}
}

// ���S�X�e�[�g�֑J��
void EnemySlime::TransitionDeathState()
{
	state = State::Death;

	// �_���[�W�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Die, false);
}

// ���S�X�e�[�g�X�V����
void EnemySlime::UpdateDeathState()
{
	// �_���[�W�A�j���[�V�������I������玩����j��
	if (!model->IsPlayAnimation())
	{
		Destroy();
	}
}