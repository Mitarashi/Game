#pragma once

#include "../Library/Model/Model.h"
#include "../Library/Shader/Shader.h"

#include "Stage.h"

// メインステージ
class StageMain : public Stage
{
public:
	// コンストラクタ
	StageMain();

	// デストラクタ
	~StageMain() override = default;

	// 更新処理
	void Update() override;

	// 描画処理
	void Render(const RenderContext& renderContext);

	// レイキャスト
	bool RayCast(const Vec3& start,
		const Vec3& end,
		HitResult& hit) override;

private:
	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Shader> shader = nullptr;
};