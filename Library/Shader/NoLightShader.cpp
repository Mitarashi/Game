#include "../DxSystem.h"
#include "../ResourceManager.h"

#include "NoLightShader.h"

NoLightShader::NoLightShader()
{
	HRESULT hr = S_OK;

	// シェーダ―
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		const u_int numElements = ARRAYSIZE(inputElementDesc);

		hr = ResourceManager::Instance().CreateVSFromCSO(
			"Shader/NoLight_vs.cso",
			vertexShader.GetAddressOf(),
			inputLayout.GetAddressOf(),
			inputElementDesc,
			numElements
		);

		hr = ResourceManager::Instance().CreatePSFromCSO(
			"Shader/NoLight_ps.cso",
			pixelShader.GetAddressOf());
	}

	// 定数バッファ
	{
		// シーン用バッファ
		ShaderFunction::CreateConstantBuffer(sceneConstantBuffer.GetAddressOf(), sizeof(CBufferScene));

		// メッシュ用バッファ
		ShaderFunction::CreateConstantBuffer(meshConstantBuffer.GetAddressOf(), sizeof(CBufferMesh));

		// サブセット用バッファ
		ShaderFunction::CreateConstantBuffer(subsetConstantBuffer.GetAddressOf(), sizeof(CBufferSubset));
	}

	// サンプラステート
	ShaderFunction::SetSamplerState(samplerState, D3D11_TEXTURE_ADDRESS_WRAP);
}

// 描画開始処理
void NoLightShader::Begin(const RenderContext& renderContext)
{
	DxSystem& dxSystem = DxSystem::Instance();
	ID3D11DeviceContext* dc = dxSystem.GetDeviceContext();

	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->IASetInputLayout(inputLayout.Get());

	ID3D11Buffer* constantBuffers[] =
	{
		sceneConstantBuffer.Get(),
		meshConstantBuffer.Get(),
		subsetConstantBuffer.Get()
	};
	dc->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	dc->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(dxSystem.GetBlendState(DxSystem::BS::ALPHA), blend_factor, 0xFFFFFFFF);

	dc->OMSetDepthStencilState(dxSystem.GetDepthStencilState(DxSystem::DS::TRUE), 0);
	dc->RSSetState(dxSystem.GetRasterizerState(DxSystem::RS::CULL_BACK));
	dc->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	// シーン用定数バッファ更新
	CBufferScene cbScene;

	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&renderContext.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&renderContext.projection);
	DirectX::XMStoreFloat4x4(&cbScene.viewProjection, V * P);

	cbScene.lightDirection = renderContext.lightDirection;
	dc->UpdateSubresource(sceneConstantBuffer.Get(), 0, 0, &cbScene, 0, 0);
}

// 描画処理
void NoLightShader::Render(const Model* model)
{
	ID3D11DeviceContext* dc = DxSystem::Instance().GetDeviceContext();

	const auto resource = model->GetResource();
	const std::vector<Model::Node>& nodes = model->GetNodes();

	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// メッシュ用定数バッファ更新
		CBufferMesh cbMesh;
		::memset(&cbMesh, 0, sizeof(cbMesh));

		if (mesh.nodeIndices.size() > 0)
		{
			for (size_t i = 0; i < mesh.nodeIndices.size(); ++i)
			{
				DirectX::XMMATRIX worldTransform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.nodeIndices.at(i)).worldTransform);
				DirectX::XMMATRIX offsetTransform = DirectX::XMLoadFloat4x4(&mesh.offsetTransforms.at(i));
				DirectX::XMMATRIX boneTransform = offsetTransform * worldTransform;
				DirectX::XMStoreFloat4x4(&cbMesh.boneTransform[i], boneTransform);
			}
		}
		else
		{
			cbMesh.boneTransform[0] = nodes.at(mesh.nodeIndex).worldTransform;
		}
		dc->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &cbMesh, 0, 0);

		UINT stride = sizeof(ModelResource::Vertex);
		UINT offset = 0;
		dc->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		dc->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			CBufferSubset cbSubset;
			cbSubset.materialColor = subset.material->color;
			dc->UpdateSubresource(subsetConstantBuffer.Get(), 0, 0, &cbSubset, 0, 0);
			dc->PSSetShaderResources(0, 1, subset.material->shaderResourceView.GetAddressOf());
			dc->PSSetSamplers(0, 1, samplerState.GetAddressOf());
			dc->DrawIndexed(subset.indexCount, subset.startIndex, 0);
		}
	}
}

// 描画終了処理
void NoLightShader::End()
{
	ID3D11DeviceContext* dc = DxSystem::Instance().GetDeviceContext();

	dc->VSSetShader(nullptr, nullptr, 0);
	dc->PSSetShader(nullptr, nullptr, 0);
	dc->IASetInputLayout(nullptr);
}
