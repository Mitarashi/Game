#pragma once

#include <memory>
#include <string>

// 前方宣言
class StateBase;

// ステート切り替え
class StateSwitch
{
public:
	// コンストラクタ
	StateSwitch() = default;
	StateSwitch(const std::shared_ptr<StateBase> state, const std::string nextState);

	// デストラクタ
	~StateSwitch() = default;

	// 更新処理の前に呼ばれる
	void Start();

	// 更新処理
	void Update();

	// ステート切り替え
	bool ChangeState();

	// getter
	const std::string& GetNextStateName() const { return nextStateName; }

private:
	const std::string nextStateName;
	std::shared_ptr<StateBase> state = nullptr;
};