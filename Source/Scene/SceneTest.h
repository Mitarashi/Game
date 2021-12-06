#pragma once

#include "Scene.h"

// タイトルシーン
class SceneTest : public Scene
{
public:
	// コンストラクタ
	SceneTest() = default;

	// デストラクタ
	virtual ~SceneTest() override = default;

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Render() override;

	// ImGui描画
	void OnGui() override;

private:
};