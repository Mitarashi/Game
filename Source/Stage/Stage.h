#pragma once

#include "../Library/Misc.h"
#include "../Library/Shader/RenderContext.h"

#include "../Collision.h"

// ステージ
class Stage : public Obj3d
{
public:
	// コンストラクタ
	Stage() = default;

	// デストラクタ
	virtual ~Stage() = default;

	// 更新処理
	virtual void Update() = 0;

	// 描画処理
	virtual void Render(const RenderContext& renderContext) = 0;

	// レイキャスト
	virtual bool RayCast(const Vec3& start,
		const Vec3& end,
		HitResult& hit) = 0;
};