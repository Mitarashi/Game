#pragma once

#include <wrl.h>
#include <memory>
#include <d3d11.h>

#include "../Model/Model.h"
#include "../Utility/Vector.h"

#include "RenderContext.h"
#include "ShaderFunction.h"

// Shaderクラス
class Shader
{
public:
	Shader()			= default;
	virtual ~Shader()	= default;

	// 描画開始処理
	virtual void Begin(const RenderContext& renderContext) = 0;

	// 描画処理
	virtual void Render(const Model* model) = 0;

	// 描画終了処理
	virtual void End() = 0;

protected:
	static const int MAX_BONES = 128;
};