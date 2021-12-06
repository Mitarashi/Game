#pragma once

#include "Scene.h"

#include "../Library/Utility/Sprite.h"
#include "../Library/Utility/SkyBox.h"
#include "../Library/Camera/CameraManager.h"

#include "../Source/Stage/StageManager.h"
#include "../Source/Character/EnemyManager.h"
#include "../Source/Character/PlayerManager.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	// コンストラクタ
	SceneGame() = default;

	// デストラクタ
	virtual ~SceneGame() override = default;

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
	std::unique_ptr<SkyBox> skyBox = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;

	bool					debug = true;
	Vec3					spawnPos = {};
	float					spawnRange = 30.0f;
};