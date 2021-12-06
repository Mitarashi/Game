#pragma once

#include <memory>
#include <string>

// �O���錾
class StateBase;

// �X�e�[�g�؂�ւ�
class StateSwitch
{
public:
	// �R���X�g���N�^
	StateSwitch() = default;
	StateSwitch(const std::shared_ptr<StateBase> state, const std::string nextState);

	// �f�X�g���N�^
	~StateSwitch() = default;

	// �X�V�����̑O�ɌĂ΂��
	void Start();

	// �X�V����
	void Update();

	// �X�e�[�g�؂�ւ�
	bool ChangeState();

	// getter
	const std::string& GetNextStateName() const { return nextStateName; }

private:
	const std::string nextStateName;
	std::shared_ptr<StateBase> state = nullptr;
};