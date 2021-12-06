#include "../Library/DxSystem.h"
#include "../Library/Input/InputManager.h"
#include "../Library/Camera/CameraManager.h"
#include "../Library/Shader/LambertShader.h"

#include "Collision.h"
#include "EnemyManager.h"
#include "Player.h"

// �R���X�g���N�^
Player::Player()
{
	model = std::make_unique<Model>("Data/Model/Jummo/Jummo.mdl");

	// ���f�����傫���̂ŃX�P�[�����O
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

	// �ҋ@�X�e�[�g�֑J��
	TransitionIdleState();
}

// �X�V����
void Player::Update()
{
	DxSystem& dxSystem = DxSystem::Instance();

	// �X�V����
	(this->*states.at(GetNum(state)))();

	// ���x�����X�V
	UpdateVelocity();

	// ���G���ԍX�V
	UpdateInvincibleTimer();

	// �v���C���[�ƃG�l�~�[�Ƃ̏Փ˔���
	CollisionPlayerVsEnemies();

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���A�j���[�V�����X�V����
	model->UpdateAnimation(dxSystem.GetDeltaTime());

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

// �`�揈��
void Player::Render(const RenderContext&	rc)
{
	shader->Begin(rc);
	shader->Render(model.get());
	shader->End();
}

// �f�o�b�O�pGUI�`��
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
		ImGui::InputFloat(u8"���G����", &invincibleTimer);

		ImGui::InputInt(u8"�W�����v��", &jumpCount);
		ImGui::InputInt(u8"�W�����vLimit", &jumpLimit);
	}
	ImGui::End();
}

// �f�o�b�O�v���~�e�B�u�`��
void Player::DebugPrimitive()
{
	DebugRenderer* debugRenderer = DxSystem::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, Vec4(0, 0, 0, 1));

	// �U���Փ˗p�̍���m�[�h�̃f�o�b�O����`��
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

// ���n�����Ƃ��ɌĂ΂��
void Player::OnLanding()
{
	jumpCount = 0;

	// �������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
	if (velocity.y < gravity * 5.0f)
	{
		// �_���[�W�A���S�X�e�[�g���͒��n�������ɃX�e�[�g�J�ڂ��Ȃ��悤�ɂ���
		if (state != State::Damage && state != State::Death)
		{
			// ���n�X�e�[�g�֑J��
			TransitionLandState();
		}
	}
}

// �_���[�W���󂯂����ɌĂ΂��
void Player::OnDamage()
{
	// �_���[�W�X�e�[�g�֑J��
	TransitionDamageState();
}

// ���S�������ɌĂ΂��
void Player::OnDead()
{
	// ���S�X�e�[�g�֑J��
	TransitionDeathState();
}

// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
Vec3 Player::GetMoveVec() const
{
	// ���͏����擾
	GamePad& gamePad = InputManager::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
	CameraManager& cameraManager = CameraManager::Instance();
	const Vec3& cameraRight = cameraManager.GetMainCamera()->GetRight();
	const Vec3& cameraFront = cameraManager.GetMainCamera()->GetFront();

	// �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���
	// �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	// �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	// �i�s�x�N�g�����v�Z����
	Vec3 vec = {};
	vec.x = cameraRightX * ax + cameraFrontX * ay;
	vec.z = cameraRightZ * ax + cameraFrontZ * ay;

	// Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;
}

// �ړ����͏���
bool Player::InputMove()
{
	// �i�s�x�N�g���擾
	Vec3 moveVec = GetMoveVec();

	// �ړ�����
	Move(moveVec.x, moveVec.z, moveSpeed);

	// ���񏈗�
	Turn(moveVec.x, moveVec.z, turnSpeed);

	// �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
	return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

// �W�����v���͏���
bool Player::InputJump()
{
	GamePad& gamePad = InputManager::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		// �W�����v�񐔐���
		if (jumpCount < jumpLimit)
		{
			// �W�����v
			jumpCount++;
			Jump(jumpSpeed);

			// �W�����v���͂���
			return true;
		}
	}

	return false;
}

// �U�����͏���
bool Player::InputAttack()
{
	GamePad& gamePad = InputManager::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		return true;
	}

	return false;
}

// �v���C���[�ƃG�l�~�[�Ƃ̏Փ˔���
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// �S�Ă̓G�Ƒ�������ŏՓˏ���
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		auto enemy = enemyManager.GetEnemy(i);

		// �Փ˔���
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

// �m�[�h�ƓG�̏Փˏ���
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
	Model::Node* node = model->FindNode(nodeName);

	// �m�[�h�ʒu�擾
	Vec3 nodePosition;
	nodePosition.x = node->worldTransform._41;
	nodePosition.y = node->worldTransform._42;
	nodePosition.z = node->worldTransform._43;

	// �w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		std::shared_ptr<Enemy> enemy = enemyManager.GetEnemy(i);

		// �Փˏ���
		Vec3 outPosition;
		JudgeSphere nod(nodePosition, nodeRadius);
		JudgeCylinder enm(*enemy);
		if (Collision::InversectSphereVsSylinder(nod, enm, outPosition))
		{
			// �_���[�W��^����
			if (enemy->ApplyDamage(1, 0.5f))
			{
				// ������΂�
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

				// �q�b�g�G�t�F�N�g�Đ�
				//{
				//	DirectX::XMFLOAT3 e = enemy->GetPosition();
				//	e.y += enemy->GetHeight() * 0.5f;
				//	hitEffect->Play(e);
				//}
			}
		}
	}
}

// �ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
	state = State::Idle;

	// �ҋ@�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Idle, true);
}

// �ҋ@�X�e�[�g�X�V����
void Player::UpdateIdleState()
{
	// �ړ����͏���
	if (InputMove())
	{
		// �ړ��X�e�[�g�֑J��
		TransitionMoveState();
	}

	// �W�����v���͏���
	if (InputJump())
	{
		// �W�����v�X�e�[�g�֑J��
		TransitionJumpState();
	}

	// �U�����͏���
	if (InputAttack())
	{
		// �U���X�e�[�g�֑J��
		TransitionAttackState();
	}
}

// �ړ��X�e�[�g�֑J��
void Player::TransitionMoveState()
{
	state = State::Move;

	// ����A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Running, true);
}

// �ړ��X�e�[�g�X�V����
void Player::UpdateMoveState()
{
	// �ړ����͏���
	if (!InputMove())
	{
		// �ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}

	// �W�����v���͏���
	if (InputJump())
	{
		// �W�����v�X�e�[�g�֑J��
		TransitionJumpState();
	}

	// �U�����͏���
	if (InputAttack())
	{
		// �U���X�e�[�g�֑J��
		TransitionAttackState();
	}
}

// �W�����v�X�e�[�g�֑J��
void Player::TransitionJumpState()
{
	state = State::Jump;

	// �W�����v�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Jump, false);
}

// �W�����v�X�e�[�g�X�V����
void Player::UpdateJumpState()
{
	// �ړ����͏���
	InputMove();

	// �W�����v���͏���
	if (InputJump())
	{
		// �W�����v�t���b�v�A�j���[�V�����Đ�
		model->PlayAnimation(Anim_Jump_Flip, false);
	}

	// �W�����v�A�j���[�V�����I����
	if (!model->IsPlayAnimation())
	{
		// �����A�j���[�V�����Đ�
		model->PlayAnimation(Anim_Falling, true);
	}
}

// ���n�X�e�[�g�֑J��
void Player::TransitionLandState()
{
	state = State::Land;
	// ���n�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Landing, false);
}

// ���n�X�e�[�g�X�V����
void Player::UpdateLandState()
{
	// ���n�A�j���[�V�����I����
	if (!model->IsPlayAnimation())
	{
		// �ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}
}

// �U���X�e�[�g�֑J��
void Player::TransitionAttackState()
{
	state = State::Attack;
	// �U���A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Attack, false);
}

// �U���X�e�[�g�X�V����
void Player::UpdateAttackState()
{
	// �U���A�j���[�V�����I����
	if (!model->IsPlayAnimation())
	{
		// �ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentAnimationSeconds();
	attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
	if (attackCollisionFlag)
	{
		// ����m�[�h�ƃG�l�~�[�̏Փˏ���
		CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
	}
}

// �_���[�W�X�e�[�g�֑J��
void Player::TransitionDamageState()
{
	state = State::Damage;

	// �_���[�W�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_GetHit1, false);
}

// �_���[�W�X�e�[�g�X�V����
void Player::UpdateDamageState()
{
	// �_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}

// ���S�X�e�[�g�֑J��
void Player::TransitionDeathState()
{
	state = State::Death;

	// ���S�A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Death, false);
}

// ���S�X�e�[�g�X�V����
void Player::UpdateDeathState()
{
	if (!model->IsPlayAnimation())
	{
		// �{�^�����������畜���X�e�[�g�֑J��
		GamePad& gamePad = InputManager::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			TransitionReviveState();
		}
	}
}

// �����X�e�[�g�֑J��
void Player::TransitionReviveState()
{
	state = State::Revive;

	// �̗͉�
	hitPoint = maxHitPoint;

	// �����A�j���[�V�����Đ�
	model->PlayAnimation(Anim_Revive, false);
}

// �����X�e�[�g�X�V����
void Player::UpdateReviveState()
{
	// �����A�j���[�V�����I����ɑҋ@�X�e�[�g�֑J��
	if (!model->IsPlayAnimation())
	{
		TransitionIdleState();
	}
}