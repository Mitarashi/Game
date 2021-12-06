#include <imgui.h>

#include "../Misc.h"

#include "InputManager.h"

// コンストラクタ
InputManager::InputManager(HWND hWnd)
	: mouse(hWnd)
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(既にインスタンス化されています)");
	instance = this;
}

// 更新処理
void InputManager::Update()
{
	gamePad.Update();
	mouse.Update();
}

// デバッグ用GUI描画
void InputManager::DebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 310), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Input", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::CollapsingHeader("GamePad", ImGuiTreeNodeFlags_DefaultOpen))
		{
			GamePadButton button = gamePad.GetButton();

			ImGui::Text("A : %d B : %d X : %d Y : %d", GetInput(button, GamePad::BTN_A),
				GetInput(button, GamePad::BTN_B), GetInput(button, GamePad::BTN_X), GetInput(button, GamePad::BTN_Y));
			ImGui::Text("START : %d BACK : %d", GetInput(button, GamePad::BTN_START), GetInput(button, GamePad::BTN_BACK));
			ImGui::Text("LSTICK : %d RSTICK : %d", GetInput(button, GamePad::BTN_LEFT_THUMB), GetInput(button, GamePad::BTN_RIGHT_THUMB));
			ImGui::Text("LB : %d RB : %d", GetInput(button, GamePad::BTN_LEFT_SHOULDER), GetInput(button, GamePad::BTN_RIGHT_SHOULDER));
			ImGui::Text("LT : %d RT : %d", GetInput(button, GamePad::BTN_LEFT_TRIGGER), GetInput(button, GamePad::BTN_RIGHT_TRIGGER));
			ImGui::Text("LX : %.2f LY : %.2f", gamePad.GetAxisLX(), gamePad.GetAxisLY());
			ImGui::Text("RX : %.2f RY : %.2f", gamePad.GetAxisRX(), gamePad.GetAxisRY());
		}

		if (ImGui::CollapsingHeader("Mouse", ImGuiTreeNodeFlags_DefaultOpen))
		{
			MouseButton button = mouse.GetButton();

			ImGui::Text("Left : %d Right : %d", GetInput(button, Mouse::BTN_LEFT), GetInput(button, Mouse::BTN_RIGHT));
			ImGui::Text("Middle : %d", GetInput(button, Mouse::BTN_MIDDLE));
		}
	}
	ImGui::End();
}
