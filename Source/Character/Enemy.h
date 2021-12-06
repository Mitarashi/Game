#pragma once

#include <d3d11.h>
#include <string>

#include "../Library/Shader/RenderContext.h"

#include "Character.h"

// エネミー
class Enemy : public Character, public std::enable_shared_from_this<Enemy>
{
public:
	// コンストラクタ
	Enemy() = default;

	// デストラクタ
	~Enemy() = default;

	// 更新処理
	virtual void Update() = 0;

	// 描画処理
	virtual void Render(const RenderContext& renderContext) = 0;

	// 削除処理
	void Destroy();

	// デバッグ用GUI描画
	virtual void DebugGUI() = 0;

	// デバッグプリミティブ描画
	virtual void DebugPrimitive();

protected:
	std::string name = {};
};