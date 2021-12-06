#include "../Misc.h"
#include "../DxSystem.h"
#include "../ResourceManager.h"

#include "Primitive.h"

//  コンストラクタ
Primitive::Primitive() : vertexBuffer(nullptr)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	HRESULT hr = S_OK;

	// 頂点シェーダー,ピクセルシェーダー,入力レイアウト生成
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,     D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		const UINT numElements = ARRAYSIZE(inputElementDesc);

		hr = ResourceManager::Instance().CreateVSFromCSO(
			"Shader/Primitive_vs.cso",
			vertexShader.GetAddressOf(),
			inputLayout.GetAddressOf(),
			inputElementDesc,
			numElements
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = ResourceManager::Instance().CreatePSFromCSO(
			"Shader/Primitive_ps.cso",
			pixelShader.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 頂点バッファの作成
	Vertex vertices[PRIMITIVE_VERTEX_NUM] = { Vec3(0,0,0), Vec4(0,0,0,0) };
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(vertices);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		hr = device->CreateBuffer(&bufferDesc, nullptr, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

//  四角形の描画
void Primitive::Rect(
	float x, float y, float w, float h,
	float angle, float r, float g, float b, float a)
{
	Rect(Vec2(x, y), Vec2(w, h), angle, Vec4(r, g, b, a));
}

//  四角形の描画
void Primitive::Rect(
	const Vec2&				pos,
	const Vec2&				scale,
	float					angle,
	const Vec4&				color
) const
{
	DxSystem& dxSystem = DxSystem::Instance();
	ID3D11DeviceContext* dc = dxSystem.GetDeviceContext();

	if (scale.x == 0.0f || scale.y == 0.0f) return;

	D3D11_VIEWPORT viewport;
	UINT numViewport = 1;
	dc->RSGetViewports(&numViewport, &viewport);

	Vertex vertices[] =
	{
		{ Vec3(pos.x,			pos.y,			 0), color },
		{ Vec3(pos.x + scale.x,	pos.y,			 0), color },
		{ Vec3(pos.x,			pos.y + scale.y, 0), color },
		{ Vec3(pos.x + scale.x,	pos.y + scale.y, 0), color },
	};

	Vec2 m = {};
	m.x = pos.x + scale.y * 0.5f;
	m.y = pos.y + scale.y * 0.5f;
	float sinValue = sinf(angle);
	float cosValue = cosf(angle);

	for (auto& v : vertices)
	{
		v.position.x -= m.x;
		v.position.y -= m.y;

		Vec2 vec = { v.position.x, v.position.y };
		v.position.x = cosValue * vec.x - sinValue * vec.y;
		v.position.y = sinValue * vec.x + cosValue * vec.y;

		v.position.x += m.x;
		v.position.y += m.y;

		v.position.x = -1.0f + v.position.x * 2 / viewport.Width;
		v.position.y =  1.0f - v.position.y * 2 / viewport.Height;
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, vertices, sizeof(vertices));
	dc->Unmap(vertexBuffer.Get(), 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	dc->RSSetState(dxSystem.GetRasterizerState(DxSystem::RS::CULL_NONE));
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->IASetInputLayout(inputLayout.Get());

	dc->OMSetDepthStencilState(dxSystem.GetDepthStencilState(DxSystem::DS::FALSE), 1);

	dc->Draw(4, 0);
}