//#include "../Library/Effect/ParticleFire.h"
//
//// コンストラクタ
//ParticleFire::ParticleFire(std::shared_ptr<Texture> texture, float lifeTimer, DxSystem::BS state, int numParticle)
//{
//	Particle::Initialize(numParticle);
//
//	particleTexture = texture;
//	this->lifeTime	= lifeTimer;
//	blendState		= state;
//}
//
//// 更新処理
//void ParticleFire::Update()
//{
//	float deltaTime = DxSystem::Instance().GetDeltaTime();
//	for (int i = 0; i < numParticle; ++i)
//	{
//		auto& data = particleData.at(i);
//		if (data.type < 0) continue;
//
//		if (isAnimation)
//		{
//			data.animeTimer += deltaTime;
//			//if (data.animeTimer > )
//		}
//
//		data.velocity += data.acceleration * deltaTime;
//
//		data.position += data.velocity * deltaTime;
//
//		data.timer -= deltaTime;
//		data.alpha = data.timer / 0.2f;
//		data.alpha = sqrtf(data.timer);
//
//		if (data.timer <= 0) data.type = -1;
//	}
//}
//
//// 描画処理
//void ParticleFire::Render(const RenderContext& rc)
//{
//}
