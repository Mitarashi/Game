#include "StateBase.h"

#include "StateSwitch.h"

// �R���X�g���N�^
StateSwitch::StateSwitch(const std::shared_ptr<StateBase> state, const std::string nextStateName)
	: state(state), nextStateName(nextStateName)
{
}

// �X�V�����̑O�ɌĂ΂��
void StateSwitch::Start()
{
	state->Start();
}

// �X�V����
void StateSwitch::Update()
{
	state->Update();
}

// �X�e�[�g�؂�ւ�
bool StateSwitch::ChangeState()
{
	if (state->IsNextState())
	{
		state->ChangeState();
		return true;
	}

	return false;
}
