#include "Character/PlayerManager.h"

// インスタンス作成
void PlayerManager::Create()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(既にインスタンス化されています)");
	instance = new PlayerManager;
}

// インスタンス削除
void PlayerManager::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// 初期化処理
void PlayerManager::Initialize()
{
	player = std::make_unique<Player>();
}

// 更新処理
void PlayerManager::Update()
{
	player->Update();
}

// 描画処理
void PlayerManager::Render(const RenderContext& rc)
{
	player->Render(rc);
}

// デバッグ用GUI描画
void PlayerManager::DebugGUI()
{
	player->DebugGUI();
}

// デバッグプリミティブ描画
void PlayerManager::DebugPrimitive()
{
	player->DebugPrimitive();
}