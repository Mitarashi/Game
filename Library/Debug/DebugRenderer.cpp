#include "../DxSystem.h"
#include "../ResourceManager.h"
#include "../Shader/ShaderFunction.h"

#include "DebugRenderer.h"

// コンストラクタ
DebugRenderer::DebugRenderer()
{
	HRESULT hr = S_OK;
	// シェーダー
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{"POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		};

		u_int numElements = ARRAYSIZE(inputElementDesc);

		hr = ResourceManager::Instance().CreateVSFromCSO(
			"Shader/Debug_vs.cso",
			vertexShader.GetAddressOf(),
			inputLayout.GetAddressOf(),
			inputElementDesc,
			numElements
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = ResourceManager::Instance().CreatePSFromCSO(
			"Shader/Debug_ps.cso",
			pixelShader.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 定数バッファ
	{
		// シーン用バッファ
		ShaderFunction::CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(CBufferMesh));
	}

	// 球メッシュ
	CreateSphereMesh(1.0f, 16, 16);

	// 円柱メッシュ
	CreateCylinderMesh(1.0f, 1.0f, 0.0f, 1.0f, 16, 1);
}

// 描画実行
void DebugRenderer::Render(const RenderContext& renderContext)
{
	DxSystem& dxSystem = DxSystem::Instance();
	ID3D11DeviceContext* dc = dxSystem.GetDeviceContext();

	// シェーダー設定
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->IASetInputLayout(inputLayout.Get());

	// 定数バッファ設定
	dc->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// レンダーステート
	const float blendFactor[4] = { 1.0f,1.0f,1.0f,1.0f };
	dc->OMSetBlendState(dxSystem.GetBlendState(DxSystem::BS::ALPHA), blendFactor, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(dxSystem.GetDepthStencilState(DxSystem::DS::WRITE_FALSE), 0);
	dc->RSSetState(dxSystem.GetRasterizerState(DxSystem::RS::CULL_NONE));

	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&renderContext.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&renderContext.projection);
	DirectX::XMMATRIX VP = V * P;

	// プリミティブ設定
	u_int stride = sizeof(DirectX::XMFLOAT3);
	u_int offset = 0;
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 球描画
	dc->IASetVertexBuffers(0, 1, sphereVectexBuffer.GetAddressOf(), &stride, &offset);
	for (const Sphere& sphere : spheres)
	{
		// ワールドビュープロジェクション行列作成
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(sphere.radius, sphere.radius, sphere.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(sphere.center.x, sphere.center.y, sphere.center.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;

		// 定数バッファ更新
		CBufferMesh cbMesh = {};
		cbMesh.color = sphere.color;
		DirectX::XMStoreFloat4x4(&cbMesh.worldViewProjection, WVP);

		dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMesh, 0, 0);
		dc->Draw(sphereVertexCount, 0);
	}
	spheres.clear();

	// 円柱描画
	dc->IASetVertexBuffers(0, 1, cylinderVertexBuffer.GetAddressOf(), &stride, &offset);
	for (const Cylinder& cylinder : cylinders)
	{
		// ワールドビュープロジェクション行列作成
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(cylinder.radius, cylinder.height, cylinder.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(cylinder.position.x, cylinder.position.y, cylinder.position.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;

		// 定数バッファ更新
		CBufferMesh cbMesh = {};
		cbMesh.color = cylinder.color;
		DirectX::XMStoreFloat4x4(&cbMesh.worldViewProjection, WVP);

		dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMesh, 0, 0);
		dc->Draw(cylinderVertexCount, 0);
	}
	cylinders.clear();
}

// 球描画
void DebugRenderer::DrawSphere(
	const Vec3& center,
	float		radius,
	const Vec4& color)
{
	Sphere sphere = {};
	sphere.center = center;
	sphere.radius = radius;
	sphere.color = color;
	spheres.emplace_back(sphere);
}

// 円柱描画
void DebugRenderer::DrawCylinder(
	const Vec3& position,
	float		radius,
	float		height,
	const Vec4& color)
{
	Cylinder cylinder = {};
	cylinder.position = position;
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.color = color;
	cylinders.emplace_back(cylinder);
}

// 球メッシュ作成
void DebugRenderer::CreateSphereMesh(
	float			radius,
	int				slices,
	int				stacks)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	sphereVertexCount = stacks * slices * 2 + slices * stacks * 2;
	std::unique_ptr<Vec3[]> vertices = std::make_unique<Vec3[]>(sphereVertexCount);

	float phiStep = DirectX::XM_PI / stacks;
	float thetaStep = DirectX::XM_2PI / slices;

	Vec3* p = vertices.get();

	for (int i = 0; i < stacks; ++i)
	{
		float phi = i * phiStep;
		float y = radius * cosf(phi);
		float r = radius * sinf(phi);

		for (int j = 0; j < slices; ++j)
		{
			float theta = j * thetaStep;
			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;

			theta += thetaStep;

			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;
		}
	}

	thetaStep = DirectX::XM_2PI / stacks;
	for (int i = 0; i < slices; ++i)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(i * thetaStep);
		for (int j = 0; j < stacks; ++j)
		{
			float theta = j * thetaStep;
			DirectX::XMVECTOR V1 = DirectX::XMVectorSet(
				radius * sinf(theta),
				radius * cosf(theta),
				0.0f, 1.0f);
			DirectX::XMVECTOR P1 = DirectX::XMVector3TransformCoord(V1, M);
			DirectX::XMStoreFloat3(p++, P1);

			//int n = (j + 1) % stacks;
			theta += thetaStep;

			DirectX::XMVECTOR V2 = DirectX::XMVectorSet(
				radius * sinf(theta),
				radius * cosf(theta),
				0.0f, 1.0f);
			DirectX::XMVECTOR P2 = DirectX::XMVector3TransformCoord(V2, M);
			DirectX::XMStoreFloat3(p++, P2);
		}
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subResourceDate = {};

		desc.ByteWidth = static_cast<u_int>(sizeof(Vec3) * sphereVertexCount);
		desc.Usage = D3D11_USAGE_IMMUTABLE; // D3D11_USAGE_DEFAULT
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subResourceDate.pSysMem = vertices.get();
		subResourceDate.SysMemPitch = 0;
		subResourceDate.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subResourceDate, sphereVectexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

// 円柱メッシュ作成
void DebugRenderer::CreateCylinderMesh(
	float			radius1,
	float			radius2,
	float			start,
	float			height,
	int				slices,
	int				stacks)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	cylinderVertexCount = 2 * slices * (stacks + 1) + 2 * slices;
	std::unique_ptr<Vec3[]> vertices = std::make_unique<Vec3[]>(cylinderVertexCount);

	Vec3* p = vertices.get();

	float stackHeight = height / stacks;
	float radiusStep = (radius2 - radius1) / stacks;

	// vertices of ring
	float dTheta = DirectX::XM_2PI / slices;

	for (int i = 0; i < slices; ++i)
	{
		int n = (i + 1) % slices;

		float c1 = cosf(i * dTheta);
		float s1 = sinf(i * dTheta);

		float c2 = cosf(n * dTheta);
		float s2 = sinf(n * dTheta);

		for (int j = 0; j <= stacks; ++j)
		{
			float y = start + j * stackHeight;
			float r = radius1 + j * radiusStep;

			p->x = r * c1;
			p->y = y;
			p->z = r * s1;
			p++;

			p->x = r * c2;
			p->y = y;
			p->z = r * s2;
			p++;
		}

		p->x = radius1 * c1;
		p->y = start;
		p->z = radius1 * s1;
		p++;

		p->x = radius2 * c1;
		p->y = start + height;
		p->z = radius2 * s1;
		p++;
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subResourceData = {};

		desc.ByteWidth = static_cast<u_int>(sizeof(Vec3) * cylinderVertexCount);
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subResourceData.pSysMem = vertices.get();
		subResourceData.SysMemPitch = 0;
		subResourceData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subResourceData, cylinderVertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}