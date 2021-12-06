#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>

// 前方宣言
class StateSwitch;

// ステートマシン
class StateMachine
{
public:
	// コンストラクタ
	StateMachine();

	// デストラクタ
	~StateMachine() = default;

	// 更新処理
	void Update();

	// ステート登録
	void Register(const std::string& name, const std::shared_ptr<StateSwitch> state);

	// ステートセット
	void SetStartState(const std::string& registerName);

	// ステート削除
	void UnRegister(const std::string& registerName);

	// ステート全削除
	void Clear();

private:
	std::unordered_map<std::string, std::shared_ptr<StateSwitch>> stateMap;
	std::shared_ptr<StateSwitch> nowState = nullptr;
};