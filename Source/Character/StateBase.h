#pragma once

// ステート
class StateBase
{
public:
	// コンストラクタ
	StateBase() : isNextState(false) {}

	// デストラクタ
	~StateBase() = default;

	// 更新処理の前に呼ばれる
	virtual void Start() = 0;

	// 更新処理
	virtual void Update() = 0;

	// ステート切り替え
	virtual void ChangeState() = 0;

	// 切り替え条件取得
	bool IsNextState() const { return isNextState; }

private:
	bool isNextState = false;
};