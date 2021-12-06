#pragma once

#include <memory>

#include "../Library/Utility/Sprite.h"

#include "../Source/Scene/Scene.h"

// ローディングシーン
class SceneLoading : public Scene
{
public:
	// コンストラクタ
	SceneLoading() = default;
	SceneLoading(std::unique_ptr<Scene> nextScene) : nextScene(std::move(nextScene)) {};

	// デストラクタ
	~SceneLoading() override = default;

	// 初期化処理
	void Initialize() override;

	// 終了化処理
	void Finalize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Render() override;

	// ImGui描画
	void OnGui() override;

private:
	// ローディングスレッド
	static void LoadingThread(SceneLoading* scene);

private:
	std::unique_ptr<Sprite> sprite = nullptr;
	float					angle = 0.0f;
	std::unique_ptr<Scene>	nextScene = nullptr;
};