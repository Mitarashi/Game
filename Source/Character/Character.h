#pragma once

#include "Obj3d.h"

// �L�����N�^�[
class Character : public Obj3d
{
public:
	// �R���X�g���N�^
	Character() = default;

	// �f�X�g���N�^
	virtual ~Character() = default;

	// getter
	const float	GetRadius()		 const { return radius; }
	const float	GetHeight()		 const { return height; }
	const bool	IsGround()		 const { return isGround; }
	const int	GetHitPoint()	 const { return hitPoint; }
	const int	GetMaxHitPoint() const { return maxHitPoint; }

	// �_���[�W��^����
	virtual bool ApplyDamage(int damage, float invincibleTime);

	// �Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

protected:
	// �ړ�����
	void Move(float vx, float vz, float speed);

	// ���񏈗�
	void Turn(float vx, float vz, float speed);

	// �W�����v����
	void Jump(float speed);

	// ���͏����X�V
	void UpdateVelocity();

protected:
	// ���n�����Ƃ��ɌĂ΂��
	virtual void OnLanding() {};

	// �_���[�W���󂯂����ɌĂ΂��
	virtual void OnDamage() {};

	// ���S�����Ƃ��ɌĂ΂��
	virtual void OnDead() {};

	// ���G���ԍX�V
	void UpdateInvincibleTimer();

private:
	// �������͍X�V����
	void UpdateVerticalVelocity(float deltaFrame);

	// �����ړ��X�V����
	void UpdateVerticalMove();

	// �������͍X�V����
	void UpdateHorizontalVelocity(float deltaFrame);

	// �����ړ��X�V����
	void UpdateHorizontalMove();

protected:
	float				radius			= 0.5f;
	float				height			= 2.0f;
	bool				isGround		= false;
	float				invincibleTimer = 1.0f;
	float				friction		= 0.5f;
	float				acceleration	= 1.0f;
	float				maxMoveSpeed	= 5.0f;
	float				moveVecX		= 0.0f;
	float				moveVecZ		= 0.0f;
	float				airControl		= 0.3f;
	float				stepOffset		= 1.0f;
	float				slopRate		= 1.0f;
	int					hitPoint		= 5;
	int					maxHitPoint		= 5;
};