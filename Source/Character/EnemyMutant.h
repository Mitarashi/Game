#pragma once

#include "../Library/Math.h"
#include "../Library/Model/Model.h"

#include "../Source/Character/Enemy.h"

// �O���錾
class Shader;

// �G�l�~�[�~���[�^���g
class EnemyMutant : public Enemy
{
private:
	// �X�e�[�g
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

	// �A�j���[�V����
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
	// �R���X�g���N�^
	EnemyMutant();

	// �f�X�g���N�^
	~EnemyMutant() override = default;

	// �X�V����
	void Update() override;

	// �`�揈��
	void Render(const RenderContext& renderContext) override;

	// �f�o�b�O�pGUI�`��
	void DebugGUI();

	// �f�o�b�O�v���~�e�B�u�`��
	void DebugPrimitive() override;

	// �꒣��ݒ�
	void SetTerritory(const Vec3& origin, float range) { territoryOrigin = origin; territoryRange = range; }

protected:
	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamage() override;

	// ���S�������ɌĂ΂��
	void OnDead() override;

private:
	// �^�[�Q�b�g�ʒu�������_���ݒ�
	void SetRandomTargetPosition();

	// �ڕW�n�_�ֈړ�
	void MoveToTarget(float speedRate);

	// �v���C���[���G
	bool SearchPlayer();

	// �m�[�h�ƃv���C���[�̏Փˏ���
	void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

	// �p�j�X�e�[�g�֑J��
	void TransitionWanderState();

	// �p�j�X�e�[�g�X�V����
	void UpdateWanderState();

	// �ҋ@�X�e�[�g�֑J��
	void TransitionIdleState();

	// �ҋ@�X�e�[�g�X�V����
	void UpdateIdleState();

	// �ǐՃX�e�[�g�֑J��
	void TransitionPursuitState();

	// �ǐՃX�e�[�g�X�V����
	void UpdatePursuitState();

	// �U���X�e�[�g�֑J��
	void TransitionAttackState();

	// �U���X�e�[�g�X�V����
	void UpdateAttackState();

	// �퓬�ҋ@�X�e�[�g�֑J��
	void TransitionIdleBattleState();

	// �퓬�ҋ@�X�e�[�g�X�V����
	void UpdateIdleBattleState();

	// �_���[�W�X�e�[�g�֑J��
	void TransitionDamageState();

	// �_���[�W�X�e�[�g�X�V����
	void UpdateDamageState();

	// ���S�X�e�[�g�֑J��
	void TransitionDeathState();

	// ���S�X�e�[�g�X�V����
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