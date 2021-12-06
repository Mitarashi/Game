#pragma once

// �X�e�[�g
class StateBase
{
public:
	// �R���X�g���N�^
	StateBase() : isNextState(false) {}

	// �f�X�g���N�^
	~StateBase() = default;

	// �X�V�����̑O�ɌĂ΂��
	virtual void Start() = 0;

	// �X�V����
	virtual void Update() = 0;

	// �X�e�[�g�؂�ւ�
	virtual void ChangeState() = 0;

	// �؂�ւ������擾
	bool IsNextState() const { return isNextState; }

private:
	bool isNextState = false;
};