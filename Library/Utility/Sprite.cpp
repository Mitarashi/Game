#include <cstdio>
#include <WICTextureLoader.h>

#include "../Misc.h"
#include "../Math.h"
#include "../DxSystem.h"

#include "Sprite.h"

// コンストラクタ
Sprite::Sprite(const char* fileName)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	HRESULT hr = S_OK;

	// 頂点データの定義
	Vertex vertices[] = {
		{ Vec3(-0.5, +0.5, 0), Vec2( 0, 0),Vec4(1, 1, 1, 1) },
		{ Vec3(+0.5, +0.5, 0), Vec2( 1, 0),Vec4(1, 0, 0, 1) },
		{ Vec3(-0.5, -0.5, 0), Vec2( 0, 1),Vec4(0, 1, 0, 1) },
		{ Vec3(+0.5, -0.5, 0), Vec2( 1, 1),Vec4(0, 0, 1, 1) },
	};

	// 頂点バッファの生成
	{
		// 頂点バッファを作成するための設定オプション
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth			= sizeof(vertices);
		bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags			= 0;
		bufferDesc.StructureByteStride = 0;

		// 頂点バッファに頂点データを入れるための設定
		D3D11_SUBRESOURCE_DATA resource = {};
		resource.pSysMem = vertices;
		resource.SysMemPitch = 0;
		resource.SysMemSlicePitch = 0;

		// 頂点バッファオブジェクトの生成
		hr = device->CreateBuffer(&bufferDesc, &resource, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 頂点シェーダー,ピクセルシェーダー,入力レイアウト生成
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		const UINT numElements = ARRAYSIZE(inputElementDesc);

		hr = ResourceManager::Instance().CreateVSFromCSO(
			"Shader/Sprite_vs.cso",
			vertexShader.GetAddressOf(),
			inputLayout.GetAddressOf(),
			inputElementDesc,
			numElements
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = ResourceManager::Instance().CreatePSFromCSO(
			"Shader/Sprite_ps.cso",
			pixelShader.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// サンプラステート
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.MipLODBias		= 0.0f;
		desc.MaxAnisotropy	= 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD			= -FLT_MAX;
		desc.MaxLOD			= FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		hr = device->CreateSamplerState(&desc, samplerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// テクスチャの生成
	if (fileName != nullptr)
	{
		D3D11_TEXTURE2D_DESC desc = {};

		ResourceManager::Instance().LoadTextureFromFile(
			fileName,
			shaderResourceView.GetAddressOf(),
			&desc
		);

		textureWidth = desc.Width;
		textureHeight = desc.Height;
	}
	else
	{
		D3D11_TEXTURE2D_DESC desc = {};

		ResourceManager::Instance().CreateDummyTexture(
			shaderResourceView.GetAddressOf(),
			&desc);
	}
}

// 描画実行
void Sprite::Render(
	const Vec2& pos,	const Vec2& scale,
	const Vec2& texPos, const Vec2& texSize,
	float angle, const Vec4& color) const
{
	DxSystem& dxSystem = DxSystem::Instance();
	ID3D11DeviceContext* dc = dxSystem.GetDeviceContext();

	{
		// 現在設定されているビューポートからスクリーンサイズを取得する。
		D3D11_VIEWPORT viewport;
		UINT numViewports = 1;
		dc->RSGetViewports(&numViewports, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;

		// スプライトを構成する４頂点のスクリーン座標を計算する
		Vec2 positions[] = {
			Vec2(pos.x,				pos.y),				// 左上
			Vec2(pos.x + scale.x,	pos.y),				// 右上
			Vec2(pos.x,				pos.y + scale.y),	// 左下
			Vec2(pos.x + scale.x,	pos.y + scale.y),	// 右下
		};

		// スプライトを構成する４頂点のテクスチャ座標を計算する
		Vec2 texcoords[] = {
			Vec2(texPos.x,				texPos.y),				// 左上
			Vec2(texPos.x + texSize.x,	texPos.y),				// 右上
			Vec2(texPos.x,				texPos.y + texSize.y),	// 左下
			Vec2(texPos.x + texSize.x,	texPos.y + texSize.y),	// 右下
		};

		// スプライトの中心で回転させて
		// スクリーン座標系からNDC座標系へ変換する。
		float mx = pos.x + scale.x * 0.5f;
		float my = pos.y + scale.y * 0.5f;
		float theta = Math::ToRadian(angle);
		float c = cosf(theta);
		float s = sinf(theta);

		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;

			Vec2 temp = p;
			p.x = c * temp.x + -s * temp.y;
			p.y = s * temp.x + c * temp.y;

			p.x += mx;
			p.y += my;

			p.x = 2.0f * p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f * p.y / screen_height;
		}

		// 頂点バッファの内容の編集を開始する。
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pDataを編集することで頂点データの内容を書き換えることができる。
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = color.x;
			v[i].color.y = color.y;
			v[i].color.z = color.z;
			v[i].color.w = color.w;

			v[i].texcoord.x = texcoords[i].x / textureWidth;
			v[i].texcoord.y = texcoords[i].y / textureHeight;
		}

		// 頂点バッファの内容の編集を終了する。
		dc->Unmap(vertexBuffer.Get(), 0);
	}

	{
		// パイプライン設定
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		dc->IASetInputLayout(inputLayout.Get());

		dc->RSSetState(dxSystem.GetRasterizerState(DxSystem::RS::CULL_NONE));

		dc->VSSetShader(vertexShader.Get(), nullptr, 0);
		dc->PSSetShader(pixelShader.Get(), nullptr, 0);

		dc->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
		dc->PSSetSamplers(0, 1, samplerState.GetAddressOf());
		dc->OMSetDepthStencilState(dxSystem.GetDepthStencilState(DxSystem::DS::TRUE), 1);

		// 描画
		dc->Draw(4, 0);
	}
}