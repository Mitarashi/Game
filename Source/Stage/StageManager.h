#pragma once
#include <vector>

#include "../Source/Stage/Stage.h"

// ステージマネージャー
class StageManager
{
private:
	// コンストラクタ
	StageManager() = default;

	// デストラクタ
	~StageManager() = default;

public:
	// コンストラクタ
	StageManager(const StageManager&) = delete;
	StageManager(StageManager&&) = delete;

	// オペレーター
	StageManager& operator= (const StageManager&) = delete;
	StageManager& operator= (StageManager&&) = delete;

	// インスタンス作成
	static void Create();

	// インスタンス削除
	static void Destory();

	// インスタンス取得
	static StageManager& Instance() { return *instance; }

	// 更新処理
	void Update();

	// 描画処理
	void Render(const RenderContext& renderContext);

	// ステージ登録
	void Register(std::shared_ptr<Stage>& stage) { stages.emplace_back(stage); }

	// ステージ全削除
	void Clear();

	// レイキャスト
	bool RayCast(const Vec3& start,
		const Vec3& end,
		HitResult& hit);

private:
	inline static StageManager* instance = nullptr;
	std::vector<std::shared_ptr<Stage>> stages;
};

