#pragma once

#include <vector>
#include <wrl/client.h>

#include "../Library/DxSystem.h"
#include "../Library/Utility/Texture.h"
#include "../Library/Shader/RenderContext.h"

//
class ParticleSyetem
{
public:
	// コンストラクタ
	ParticleSyetem(std::shared_ptr<Texture> texture, int num = (10000));

	// デストラクタ
	~ParticleSyetem()
	{
		delete[] particleData;
		delete[] vertexData;
	}

	// 更新処理
	void Update();

	// アニメーション更新
	void UpdateAnimation(float speed = 0.1f);

	// 描画処理
	void Render(const RenderContext& rc, DxSystem::BS state = DxSystem::BS::ALPHA);

	//void Set(Vec3 position);
	void Set(int type, float timer,
		Vec3 position,
		Vec3 velocity	= Vec3(0.0f, 0.0f, 0.0f),
		Vec3 accelerate = Vec3(0.0f, 0.0f, 0.0f),
		Vec2 size		= Vec2(1.0f, 1.0f));

	void Snow(Vec3 position, int max);
	void Spark(Vec3 position, int max);
	void Fire(Vec3 position, int max);
	void Smoke(Vec3 position, int max);

private:
	struct ParticleData
	{
		Vec3	position		= {};
		Vec2	size			= {};
		Vec3	velocity		= {};
		Vec3	acceleration	= {};
		float	alpha			= 1.0f;
		float	timer			= 0.0f;
		float	animeTimer		= 0.0f;
		int		type			= 0;
	};

	struct VertexData
	{
		Vec3 position = {};
		Vec3 Normal = {};
		Vec2 Texcoord = {};
		Vec4 Color = {};
	};

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 view		= {};
		DirectX::XMFLOAT4X4 projection	= {};
		float				size		= 0.0f;
		Vec3				dummy		= {};
	};

public:
	ParticleData*									particleData	= {};
	VertexData*										vertexData		= {};
	int												numParticle		= 0;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			vertexBuffer	= nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constantBuffer	= nullptr;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader	= nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	geometryShader	= nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader		= nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout		= nullptr;

	std::shared_ptr<Texture>						texture			= nullptr;
};