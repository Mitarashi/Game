#pragma once

#include "../Library/Math.h"
#include "../Library/Model/Model.h"
#include "../Library/Shader/Shader.h"

#include "Character.h"

// �v���C���[
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

	// �X�e�[�g
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
	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player() override = default;

	// �X�V����
	void Update();

	// �`�揈��
	void Render(const RenderContext& renderContext);

	// �f�o�b�O�pGUI�`��
	void DebugGUI();

	// �f�o�b�O�v���~�e�B�u�`��
	void DebugPrimitive();

private:
	// ���n�����Ƃ��ɌĂ΂��
	void OnLanding() override;

	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamage() override;

	// ���S�������ɌĂ΂��
	void OnDead() override;

private:
	// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
	Vec3 GetMoveVec() const;

	// �ړ����͏���
	bool InputMove();

	// �W�����v���͏���
	bool InputJump();

	// �U�����͏���
	bool InputAttack();

	// �v���C���[�ƃG�l�~�[�Ƃ̏Փ˔���
	void CollisionPlayerVsEnemies();

	// �m�[�h�ƃG�l�~�[�̏Փ˔���
	void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

	// �ҋ@�X�e�[�g�֑J��
	void TransitionIdleState();

	// �ҋ@�X�e�[�g�X�V����
	void UpdateIdleState();

	// �ړ��X�e�[�g�֑J��
	void TransitionMoveState();

	// �ړ��X�e�[�g�X�V����
	void UpdateMoveState();

	// �W�����v�X�e�[�g�֑J��
	void TransitionJumpState();

	// �W�����v�X�e�[�g�X�V����
	void UpdateJumpState();

	// ���n�X�e�[�g�֑J��
	void TransitionLandState();

	// ���n�X�e�[�g�X�V����
	void UpdateLandState();

	// �U���X�e�[�g�֑J��
	void TransitionAttackState();

	// �U���X�e�[�g�X�V����
	void UpdateAttackState();

	// �_���[�W�X�e�[�g�֑J��
	void TransitionDamageState();

	// �_���[�W�X�e�[�g�X�V����
	void UpdateDamageState();

	// ���S�X�e�[�g�֑J��
	void TransitionDeathState();

	// ���S�X�e�[�g�X�V����
	void UpdateDeathState();

	// �����X�e�[�g�֑J��
	void TransitionReviveState();

	// �����X�e�[�g�X�V����
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