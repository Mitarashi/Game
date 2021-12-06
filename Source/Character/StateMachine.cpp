#include "StateSwitch.h"

#include "StateMachine.h"

// �R���X�g���N�^
StateMachine::StateMachine() : nowState()
{
}

// �X�V����
void StateMachine::Update()
{
	nowState->Update();
	if (nowState->ChangeState())
	{
		auto it = stateMap.find(nowState->GetNextStateName());
		if (it != stateMap.end()) return;

		nowState = it->second;
		nowState->Start();
	}
}

// �X�e�[�g�o�^
void StateMachine::Register(const std::string& name, const std::shared_ptr<StateSwitch> state)
{
	auto it = stateMap.find(name);
	if (it == stateMap.end()) return;

	stateMap.insert(std::make_pair(name, state));
}

// �X�e�[�g�Z�b�g
void StateMachine::SetStartState(const std::string& registerName)
{
	auto it = stateMap.find(registerName);
	if (it == stateMap.end()) return;

	nowState = it->second;
	nowState->Start();
}

// �X�e�[�g�폜
void StateMachine::UnRegister(const std::string& registerName)
{
	auto it = stateMap.find(registerName);
	if (it != stateMap.end()) return;

	stateMap.erase(it);
}

// �X�e�[�g�S�폜
void StateMachine::Clear()
{
	stateMap.clear();
}
