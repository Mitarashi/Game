//#include "../Library/Shader/ShaderFunction.h"
//#include "../Library/Effect/Particle.h"
//
//// 初期化処理
//void Particle::Initialize(int numParticle)
//{
//	HRESULT hr = S_OK;
//
//	// シェーダ―
//	{
//		// 入力レイアウト
//		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
//		{
//			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		};
//		const u_int numElements = ARRAYSIZE(inputElementDesc);
//
//		hr = ResourceManager::Instance().CreateVSFromCSO(
//			"Shader/PointSprite_vs.cso",
//			vertexShader.GetAddressOf(),
//			inputLayout.GetAddressOf(),
//			inputElementDesc,
//			numElements);
//
//		hr = ResourceManager::Instance().CreateGSFromCSO(
//			"Shader/PointSprite_gs.cso",
//			geometryShader.GetAddressOf());
//
//		hr = ResourceManager::Instance().CreatePSFromCSO(
//			"Shader/PointSprite_ps.cso",
//			pixelShader.GetAddressOf());
//	}
//
//	particleData.resize(numParticle);
//	vertexData.resize(numParticle);
//
//	ID3D11Device* device = DxSystem::Instance().GetDevice();
//
//	D3D11_BUFFER_DESC bufferDesc = {};
//	bufferDesc.Usage		= D3D11_USAGE_DEFAULT;
//	bufferDesc.ByteWidth = sizeof(VertexData) * numParticle;
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc.CPUAccessFlags = 0;
//
//	hr = device->CreateBuffer(&bufferDesc, nullptr, vertexBuffer.GetAddressOf());
//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//	ShaderFunction::CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(ConstantBuffer));
//}
//
//// アニメーション更新処理
//void Particle::UpdateAnimation()
//{
//
//}