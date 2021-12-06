#pragma once

#include <wrl.h>
#include <d3d11.h>

#include "Vector.h"
#include "../ResourceManager.h"

// スプライト
class Sprite
{
public:
	// コンストラクタ
	Sprite(const char* fileName = (nullptr));

	// デストラクタ
	~Sprite() = default;

	// 描画実行
	void Render(
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float angle, float r = (1.0f), float g = (1.0f), float b = (1.0f), float a = (1.0f)) const
	{
		Render(Vec2(dx, dy), Vec2(dw, dh), Vec2(sx, sy), Vec2(sw, sh), angle, Vec4(r, g, b, a));
	}

	void Render(
		const Vec2& pos, const Vec2& scale,
		const Vec2& texPos, const Vec2& texSize,
		float angle, const Vec4& color = Vec4(1,1,1,1)) const;

	// テクスチャ幅取得
	float GetTextureWidth() const { return static_cast<float>(textureWidth); }

	// テクスチャ高さ取得
	float GetTextureHeight() const { return static_cast<float>(textureHeight); }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer = nullptr;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout = nullptr;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView = nullptr;

	int textureWidth = 0;
	int textureHeight = 0;

	// Vertex構造体
	struct Vertex
	{
		Vec3	position;
		Vec2	texcoord;
		Vec4	color;
	};
};