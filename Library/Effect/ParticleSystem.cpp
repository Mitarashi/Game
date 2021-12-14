#include "../Library/Shader/ShaderFunction.h"
#include "../Library/Effect/ParticleSystem.h"

ParticleSyetem::ParticleSyetem(std::shared_ptr<Texture> texture, int num)
{
	// シェーダ―
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		const u_int numElements = ARRAYSIZE(inputElementDesc);

		HRESULT hr = ResourceManager::Instance().CreateVSFromCSO(
			"Shader/PointSprite_vs.cso",
			vertexShader.GetAddressOf(),
			inputLayout.GetAddressOf(),
			inputElementDesc,
			numElements);

		hr = ResourceManager::Instance().CreateGSFromCSO(
			"Shader/PointSprite_gs.cso",
			geometryShader.GetAddressOf());

		hr = ResourceManager::Instance().CreatePSFromCSO(
			"Shader/PointSprite_ps.cso",
			pixelShader.GetAddressOf());
	}

	particleData = new ParticleData[num];
	ZeroMemory(particleData, sizeof(ParticleData) * num);
	vertexData = new VertexData[num];
	ZeroMemory(vertexData, sizeof(VertexData) * num);

	numParticle = num;

	for (int i = 0; i < numParticle; i++) { particleData[i].type = -1; }
	this->texture = texture;

	ID3D11Device* device = DxSystem::Instance().GetDevice();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VertexData) * numParticle;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, vertexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	ShaderFunction::CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(ConstantBuffer));
}

void ParticleSyetem::UpdateAnimation(float speed)
{
	float time = DxSystem::Instance().GetDeltaTime();
	for (int i = 0; i < numParticle; i++)
	{
		auto& data = particleData[i];
		if (data.type < 0) continue;

		data.animeTimer += time;
		if (data.animeTimer > speed)
		{
			data.animeTimer = 0.0f;
			data.type = (data.type + 1) % 6;
		}
	}
}

void ParticleSyetem::Update()
{
	float time = DxSystem::Instance().GetDeltaTime();
	for (int i = 0; i < numParticle; i++)
	{
		auto& data = particleData[i];
		if (data.type < 0) continue;

		data.velocity += data.acceleration * time;

		data.position += data.velocity * time;

		data.timer -= DxSystem::Instance().GetDeltaTime();
		data.alpha = data.timer / 0.2f;
		data.alpha = sqrtf(data.timer);

		if (data.timer <= 0) data.type = -1;
	}
}

void ParticleSyetem::Render(const RenderContext& rc, DxSystem::BS state)
{
	DxSystem& dxSystem = DxSystem::Instance();
	ID3D11DeviceContext* dc = dxSystem.GetDeviceContext();

	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->GSSetShader(geometryShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->IASetInputLayout(inputLayout.Get());

	ConstantBuffer cb = {};
	cb.view = rc.view;
	cb.projection = rc.projection;
	cb.size = 0.1f;
	dc->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);
	dc->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	dc->GSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	dc->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	dc->OMSetBlendState(dxSystem.GetBlendState(state), nullptr, 0xFFFFFFFF);
	dc->RSSetState(dxSystem.GetRasterizerState(DxSystem::RS::CULL_BACK));
	dc->OMSetDepthStencilState(dxSystem.GetDepthStencilState(DxSystem::DS::WRITE_FALSE), 1);

	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	if (texture) texture->Set(0);

	int n = 0; //パーティクル発生数
	for (int i = 0; i < numParticle; i++)
	{
		auto& data = particleData[i];
		if (data.type < 0) continue;

		vertexData[n].position.x = data.position.x;
		vertexData[n].position.y = data.position.y;
		vertexData[n].position.z = data.position.z;
		vertexData[n].Texcoord.x = data.size.x;
		vertexData[n].Texcoord.y = data.size.y;
		vertexData[n].Color.x = (float)data.type;
		vertexData[n].Color.y = vertexData[n].Color.z = 1.0f;
		vertexData[n].Color.w = data.alpha;
		++n;
	}

	// 頂点データ更新
	dc->UpdateSubresource(vertexBuffer.Get(), 0, nullptr, vertexData, 0, 0);

	u_int stride = sizeof(VertexData);
	u_int offset = 0;
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	dc->Draw(n, 0);

	dc->OMSetDepthStencilState(dxSystem.GetDepthStencilState(DxSystem::DS::FALSE), 1);

	dc->VSSetShader(nullptr, nullptr, 0);
	dc->GSSetShader(nullptr, nullptr, 0);
	dc->PSSetShader(nullptr, nullptr, 0);
}

void ParticleSyetem::Set(int type, float timer,
	Vec3 position,
	Vec3 velocity,
	Vec3 acceleration,
	Vec2 size)
{
	for (int i = 0;i<numParticle;i++)
	{
		auto& data = particleData[i];
		if (data.type >= 0) continue;
		data.type = type;
		data.position = position;
		data.velocity = velocity;
		data.acceleration = acceleration;
		data.size = size;
		data.alpha = 1.0f;
		data.timer = timer;

		break;
	}
}

void ParticleSyetem::Snow(Vec3 position, int max)
{
	for (int i = 0; i < max; i++)
	{
		Vec3 pos = {};
		pos = position + Vec3((rand() % 10001 - 5000) * 0.01f, 0, (rand() % 10001 - 5000) * 0.01f);

		Vec3 velocity = {};
		velocity.y = -(rand() % 10001) * 0.0002f - 0.002f;

		Vec3 accelerate = {};
		accelerate.x = (rand() % 10001) * 0.00001f + 0.1f;
		accelerate.z = (rand() % 10001 - 5000) * 0.00001f;

		Vec2 size = { 0.2f, 0.2f };

		Set(4, 10, pos, velocity, accelerate, size);
	}
}

void ParticleSyetem::Spark(Vec3 pos, int max)
{
	for (int i = 0; i < max; i++) {
		Vec3 position = {};
		position = pos;

		Vec3 velocity = {};
		velocity.x = (rand() % 10001 - 5000) * 0.0001f;
		velocity.y = (rand() % 10001) * 0.0002f + 1.2f;
		velocity.z = (rand() % 10001 - 5000) * 0.0001f;

		Vec3 accelerate = { 0, -1.2f, 0 };
		Vec2 size = { 0.1f,0.1f };

		Set(2, 1.5f, position, velocity, accelerate, size);
	}
}

void ParticleSyetem::Fire(Vec3 pos, int max)
{
	for (int i = 0; i < max; i++)
	{
		Vec3 position = {};
		Vec3 velocity = {};
		Vec3 accelerate = { 0, -2.0f,0 };
		Vec2 size = { 0.4f,0.4f };

		position.x = pos.x + (rand() % 10001 - 5000) * 0.00005f;
		position.y = pos.y + (rand() % 10001) * 0.0001f + 0.2f;
		position.z = pos.z + (rand() % 10001 - 5000) * 0.00005f;

		velocity.x = (rand() % 10001 - 5000) * 0.00005f;
		velocity.y = (rand() % 10001) * 0.0001f + 0.2f;
		velocity.z = (rand() % 10001 - 5000) * 0.00005f;

		Set(13, 0.4f, position, velocity, accelerate, size);
	}
}

void ParticleSyetem::Smoke(Vec3 pos, int max)
{
	for (int i = 0; i < max; i++)
	{
		Vec3 position = {};
		Vec3 velocity = {};
		Vec3 accelerate = { 0, -2.0f,0 };
		Vec2 size = { 0.4f,0.4f };

		position.x = pos.x + (rand() % 10001 - 5000) * 0.0001f;
		position.y = pos.y + (rand() % 10001) * 0.0001f + 0.5f;
		position.z = pos.z + (rand() % 10001 - 5000) * 0.0001f;

		velocity.x = (rand() % 10001 - 5000) * 0.0001f;
		velocity.y = (rand() % 10001) * 0.0004f + 0.005f;
		velocity.z = (rand() % 10001 - 5000) * 0.0001f;

		Set(14, 0.6f, position, velocity, accelerate, size);
	}
}