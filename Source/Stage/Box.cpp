#include "../Library/DxSystem.h"
#include "../Library/Shader/NoLightShader.h"

#include "Box.h"

// コンストラクタ
Box::Box()
{
	model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");

	shader = std::make_unique<NoLightShader>();
}

// 更新処理
void Box::Update()
{
	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);
}

// 描画処理
void Box::Render(const RenderContext& rc)
{
	shader->Begin(rc);
	shader->Render(model.get());
	shader->End();
}

// デバッグ用GUI描画
void Box::DebugGUI()
{

}

// デバッグプリミティブ描画
void Box::DebugPrimitive()
{
}
