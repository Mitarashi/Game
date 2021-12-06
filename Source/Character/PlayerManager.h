#pragma once

#include <memory>

#include "../Library/Misc.h"

#include "../Source/Character/Player.h"

// プレイヤーマネージャー
class PlayerManager
{
private:
	// コンストラクタ
	PlayerManager() = default;

	// デストラクタ
	~PlayerManager() = default;

public:
	// コンストラクタ
	PlayerManager(const PlayerManager&) = delete;
	PlayerManager(PlayerManager&&) = delete;

	// オペレーター
	PlayerManager& operator= (const PlayerManager&) = delete;
	PlayerManager& operator= (PlayerManager&&) = delete;

	// インスタンス作成
	static void Create();

	// インスタンス削除
	static void Destroy();

	// インスタンス取得
	static PlayerManager& Instance() { return* instance; };

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Render(const RenderContext& renderContext);

	// デバッグ用GUI描画
	void DebugGUI();

	// デバッグプリミティブ描画
	void DebugPrimitive();

	// プレイヤー取得
	const std::unique_ptr<Player>& GetPlayer() const { return player; }

private:
	inline static PlayerManager* instance = nullptr;
	std::unique_ptr<Player> player = nullptr;
};