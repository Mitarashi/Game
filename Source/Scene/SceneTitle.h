#pragma once

#include "../Library/Utility/Sprite.h"

#include "Scene.h"

// タイトルシーン
class SceneTitle : public Scene
{
public:
	// コンストラクタ
	SceneTitle() = default;

	// デストラクタ
	virtual ~SceneTitle() override = default;

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
	std::unique_ptr<Sprite> titleSpr = nullptr;
};