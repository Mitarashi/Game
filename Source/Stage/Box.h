#pragma once

#include "Obj3d.h"

// ボックス
class Box : public Obj3d
{
public:
	// コンストラクタ
	Box();

	// デストラクタ
	~Box() override = default;

	// 更新処理
	void Update();

	// 描画処理
	void Render(const RenderContext& renderContext);

	// デバッグ用GUI描画
	void DebugGUI();

	// デバッグプリミティブ描画
	void DebugPrimitive();
};