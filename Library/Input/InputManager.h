#pragma once

#include "GamePad.h"
#include "Mouse.h"

// インプットマネージャー
class InputManager
{
public:
	// コンストラクタ
	InputManager(HWND hWnd);

	// デストラクタ
	~InputManager() = default;

public:
	// インスタンス取得
	static InputManager& Instance() { return *instance; }

	// 更新処理
	void Update();

	// ゲームパッド取得
	GamePad& GetGamePad() { return gamePad; }

	// マウス取得
	Mouse& GetMouse() { return mouse; }

	// デバッグ用GUI描画
	void DebugGUI();

private:
	int GetInput(GamePadButton& button, GamePadButton BTN) { return (button & BTN) / BTN; }

private:
	inline static InputManager*	instance = nullptr;
	GamePad						gamePad;
	Mouse						mouse;
};
