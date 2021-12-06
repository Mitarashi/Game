#pragma once

#include <memory>

// 前方宣言
class Scene;

// シーンマネージャー
class SceneManager
{
private:
	// コンストラクタ
	SceneManager() = default;

	// デストラクタ
	~SceneManager() = default;

public:
	// コンストラクタ
	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;

	// オペレーター
	SceneManager& operator= (const SceneManager&) = delete;
	SceneManager& operator= (SceneManager&&) = delete;

	// インスタンス作成
	static void Create();

	// インスタンス削除
	static void Destroy();

	// インスタンス取得
	static SceneManager& Instance() { return *instance; }

	// 更新処理
	void Update();

	// 描画処理
	void Render();

	// ImGui描画
	void OnGui();

	// シーンクリア
	void Clear();

	// シーン切り替え
	void ChangeScene(std::unique_ptr<Scene> scene);

private:
	inline static SceneManager* instance = nullptr;
	std::unique_ptr<Scene> currentScene = nullptr;
};