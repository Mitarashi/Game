#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "../Library/Vector.h"

// プリミティブ
class Primitive
{
public:
	// コンストラクタ
	Primitive();

	// デストラクタ
	~Primitive() = default;

	// 矩形描画
	void Rect(
		float x, float y,
		float w, float h,
		float angle,
		float r, float g, float b, float a);

	void Rect(
		const Vec2&				pos,
		const Vec2&				size,
		float					angle,
		const Vec4&				color
	) const;

private:
	struct Vertex
	{
		Vec3 position;
		Vec4 color;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			vertexBuffer = nullptr;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout =nullptr;

private:
	static const int PRIMITIVE_VERTEX_NUM = 130;
};