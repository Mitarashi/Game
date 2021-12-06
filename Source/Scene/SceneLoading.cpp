#include <thread>

#include "../Library/Math.h"
#include "../Library/DxSystem.h"
#include "../Library/Input/InputManager.h"

#include "../Source/Scene/SceneManager.h"

#include "SceneLoading.h"

void SceneLoading::Initialize()
{
	sceneName = "Loading";

	sprite = std::make_unique<Sprite>("Data/Sprite/LoadingIcon.png");

	std::thread thread(LoadingThread, this);

	thread.detach();
}

// 終了化
void SceneLoading::Finalize()
{
}

// 更新処理
void SceneLoading::Update()
{
	constexpr float speed = 180;
	angle += speed * DxSystem::Instance().GetDeltaTime();

	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(std::move(nextScene));
		return;
	}
}

// 描画処理
void SceneLoading::Render()
{
	DxSystem& dxSystem = DxSystem::Instance();

	Vec4 clearColor = { 0.0f, 0.0f, 0.5f, 1.0f };
	dxSystem.Clear(clearColor);

	// 2Dスプライト描画
	{
		float screenWidth = static_cast<float>(dxSystem.GetScreenWidth());
		float screenHeight = static_cast<float>(dxSystem.GetScreenHeight());
		float textureWidth = static_cast<float>(sprite->GetTextureWidth());
		float textureHeight = static_cast<float>(sprite->GetTextureHeight());
		float positionX = screenWidth - textureWidth;
		float positionY = screenHeight - textureHeight;

		sprite->Render(positionX, positionY, textureWidth, textureHeight,
			0, 0, textureWidth, textureHeight,
			angle,
			1, 1, 1, 1);
	}
}

// ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	// COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	// 次のシーンの初期化を行う
	scene->nextScene->Initialize();

	// スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	// 次のシーンの準備完了設定
	scene->nextScene->SetReady();
}

void SceneLoading::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(900, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin(sceneName.c_str(), nullptr, ImGuiWindowFlags_None))
	{
	}
	ImGui::End();
}