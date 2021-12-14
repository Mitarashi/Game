#include "../Library/DxSystem.h"
#include "../Library/Shader/NoLightShader.h"

#include "StageMain.h"

// コンストラクタ
StageMain::StageMain()
{
	// ステージモデルを読み込み
	model = std::make_unique<Model>("Data/Model/ExampleStage/ExampleStage.mdl");

	// TODO 引数
	shader = std::make_unique<NoLightShader>();

	box = std::make_unique<Box>();
	box->SetPosition(Vec3(0, 1, 0));
}

// 更新処理
void StageMain::Update()
{
	box->Update();
}

// 描画処理
void StageMain::Render(const RenderContext& rc)
{
	// シェーダーにモデルを描画してもらう
	box->Render(rc);

	shader->Begin(rc);
	shader->Render(model.get());
	shader->End();
}

// レイキャスト
bool StageMain::RayCast(const Vec3& start,
	const Vec3& end,
	HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model.get(), hit);
}
