#include "Scene.h"
#include "SceneManager.h"

// インスタンス作成
void SceneManager::Create()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(既にインスタンス化されています)");
	instance = new SceneManager;
}

// インスタンス削除
void SceneManager::Destroy()
{
	if (instance != nullptr)
	{
		instance->Clear();
		delete instance;
		instance = nullptr;
	}
}

// 更新処理
void SceneManager::Update()
{
	if (currentScene != nullptr)
	{
		currentScene->Update();
	}
}

// 描画処理
void SceneManager::Render()
{
	if (currentScene != nullptr)
	{
		currentScene->Render();
	}
}

// ImGui描画
void SceneManager::OnGui()
{
	if (currentScene != nullptr)
	{
		currentScene->OnGui();
	}
}

// シーンクリア
void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		currentScene.reset();
		currentScene = nullptr;
	}
}

// シーン切り替え
void SceneManager::ChangeScene(std::unique_ptr<Scene> scene)
{
	// 現在のシーンの終了処理
	Clear();

	// 新しいシーンを設定
	currentScene = std::move(scene);

	if (!currentScene->IsReady())
	{
		currentScene->Initialize();
	}
}