#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>

// �O���錾
class StateSwitch;

// �X�e�[�g�}�V��
class StateMachine
{
public:
	// �R���X�g���N�^
	StateMachine();

	// �f�X�g���N�^
	~StateMachine() = default;

	// �X�V����
	void Update();

	// �X�e�[�g�o�^
	void Register(const std::string& name, const std::shared_ptr<StateSwitch> state);

	// �X�e�[�g�Z�b�g
	void SetStartState(const std::string& registerName);

	// �X�e�[�g�폜
	void UnRegister(const std::string& registerName);

	// �X�e�[�g�S�폜
	void Clear();

private:
	std::unordered_map<std::string, std::shared_ptr<StateSwitch>> stateMap;
	std::shared_ptr<StateSwitch> nowState = nullptr;
};