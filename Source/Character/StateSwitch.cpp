#include "StateBase.h"

#include "StateSwitch.h"

// コンストラクタ
StateSwitch::StateSwitch(const std::shared_ptr<StateBase> state, const std::string nextStateName)
	: state(state), nextStateName(nextStateName)
{
}

// 更新処理の前に呼ばれる
void StateSwitch::Start()
{
	state->Start();
}

// 更新処理
void StateSwitch::Update()
{
	state->Update();
}

// ステート切り替え
bool StateSwitch::ChangeState()
{
	if (state->IsNextState())
	{
		state->ChangeState();
		return true;
	}

	return false;
}
