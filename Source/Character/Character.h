#pragma once

#include "../Library/Utility/Vector.h"

// �L�����N�^�[
class Character
{
public:
	// �R���X�g���N�^
	Character() = default;

	// �f�X�g���N�^
	virtual ~Character() = default;

	// �s��X�V����
	void UpdateTransform();

	// getter
	const Vec3&	GetPosition()	 const { return position; }
	const Vec3&	GetAngle()		 const { return angle; }
	const Vec3&	GetScale()		 const { return scale; }
	const float	GetRadius()		 const { return radius; }
	const float	GetHeight()		 const { return height; }
	const bool	IsGround()		 const { return isGround; }
	const int	GetHitPoint()	 const { return hitPoint; }
	const int	GetMaxHitPoint() const { return maxHitPoint; }

	// setter
	void SetPosition(const Vec3& position_)	{ position = position_; }
	void SetAngle(const Vec3& angle_)		{ angle = angle_; }
	void SetScale(const Vec3& scale_)		{ scale = scale_; }

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
	Vec3				position		= { 0,0,0 };
	Vec3				angle			= { 0,0,0 };
	Vec3				scale			= { 1,1,1 };
	DirectX::XMFLOAT4X4	transform		= { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	float				radius			= 0.5f;
	float				gravity			= -1.0f;
	float				height			= 2.0f;
	Vec3				velocity		= { 0,0,0 };
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