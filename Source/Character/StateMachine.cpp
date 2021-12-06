#include "StateSwitch.h"

#include "StateMachine.h"

// コンストラクタ
StateMachine::StateMachine() : nowState()
{
}

// 更新処理
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

// ステート登録
void StateMachine::Register(const std::string& name, const std::shared_ptr<StateSwitch> state)
{
	auto it = stateMap.find(name);
	if (it == stateMap.end()) return;

	stateMap.insert(std::make_pair(name, state));
}

// ステートセット
void StateMachine::SetStartState(const std::string& registerName)
{
	auto it = stateMap.find(registerName);
	if (it == stateMap.end()) return;

	nowState = it->second;
	nowState->Start();
}

// ステート削除
void StateMachine::UnRegister(const std::string& registerName)
{
	auto it = stateMap.find(registerName);
	if (it != stateMap.end()) return;

	stateMap.erase(it);
}

// ステート全削除
void StateMachine::Clear()
{
	stateMap.clear();
}
