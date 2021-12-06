#pragma once

#include <vector>
#include <memory>

#include "../Library/Shader/RenderContext.h"

#include "../Character/Enemy.h"

// エネミーマネージャー
class EnemyManager
{
private:
	// コンストラクタ
	EnemyManager() = default;

	// デストラクタ
	~EnemyManager() = default;

public:
	// コンストラクタ
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager(EnemyManager&&) = delete;

	// オペレーター
	EnemyManager& operator= (const EnemyManager&) = delete;
	EnemyManager& operator= (EnemyManager&&) = delete;

	// インスタンス作成
	static void Create();

	// インスタンス削除
	static void Destroy();

	// インスタンス取得
	static EnemyManager& Instance() { return *instance; }

	// 更新処理
	void Update();

	// 描画処理
	void Render(const RenderContext& renderContext);

	// エネミー登録
	void Register(std::shared_ptr<Enemy> enemy) { enemies.emplace_back(enemy); }

	// エネミー削除
	void Remove(std::shared_ptr<Enemy> enemy) { removes.emplace_back(enemy); }

	// エネミー全削除
	void Clear();

	// デバッグ用GUI描画
	void DebugGUI();

	// デバッグプリミティブ描画
	void DebugPrimitive();

	// エネミーの数取得
	const int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// エネミー取得
	std::shared_ptr<Enemy> GetEnemy(int index)
	{
		if (enemies.size() <= index) return nullptr;
		else return enemies.at(index);
	}

private:
	// エネミー同士の当たり判定
	void CollisionEnemyVsEnemy();

private:
	inline static EnemyManager* instance = nullptr;
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::shared_ptr<Enemy>> removes;
};