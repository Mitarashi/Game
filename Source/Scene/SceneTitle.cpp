#include "../Library/DxSystem.h"
#include "../Library/Input/InputManager.h"

#include "SceneGame.h"
#include "SceneTest.h"
#include "SceneLoading.h"
#include "SceneManager.h"

#include "SceneTitle.h"

// 初期化
void SceneTitle::Initialize()
{
	sceneName = "Title";

	titleSpr = std::make_unique<Sprite>("Data/Sprite/Title.png");
}

// 終了化
void SceneTitle::Finalize()
{
}

// 更新処理
void SceneTitle::Update()
{
	GamePad& gamePad = InputManager::Instance().GetGamePad();

	// なにかボタン押したらゲームシーンへ切り替え
	const GamePadButton anyButton =
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y
		| GamePad::BTN_BACK
		| GamePad::BTN_DOWN
		| GamePad::BTN_LEFT
		| GamePad::BTN_LEFT_SHOULDER
		| GamePad::BTN_LEFT_THUMB
		| GamePad::BTN_LEFT_TRIGGER
		| GamePad::BTN_RIGHT
		| GamePad::BTN_RIGHT_SHOULDER
		| GamePad::BTN_RIGHT_THUMB
		| GamePad::BTN_RIGHT_TRIGGER
		| GamePad::BTN_START
		| GamePad::BTN_UP;

	//if (gamePad.GetButtonDown() & anyButton)
	//{
	//	SceneManager::Instance().ChangeScene(
	//		std::make_unique<SceneLoading>(std::make_unique<SceneGame>()));
	//}
}

// 描画処理
void SceneTitle::Render()
{
	DxSystem& dxSystem = DxSystem::Instance();

	Vec4 clearColor = { 0.0f, 0.0f, 0.5f, 1.0f };
	dxSystem.Clear(clearColor);

	titleSpr->Render({ 0,0 }, {dxSystem.GetScreenWidth(),dxSystem.GetScreenHeight()}, {0,0}, {titleSpr->GetTextureWidth(),titleSpr->GetTextureHeight()}, 0);
}

// ImGui描画
void SceneTitle::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(900, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin(sceneName.c_str(), nullptr, ImGuiWindowFlags_None))
	{
	}
	ImGui::End();
	Scene::OnGui();
}