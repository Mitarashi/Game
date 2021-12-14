//#include "../Library/Effect/ParticleManager.h"
//
//// インスタンス作成
//void ParticleManager::Create()
//{
//	// インスタンス設定
//	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(既にインスタンス化されています)");
//	instance = new ParticleManager;
//}
//
//// インスタンス削除
//void ParticleManager::Destroy()
//{
//	if (instance != nullptr)
//	{
//		instance->Clear();
//		delete instance;
//		instance = nullptr;
//	}
//}
//
//// 更新処理
//void ParticleManager::Update()
//{
//	for (auto& particle : particles)
//	{
//		if (particle->GetLifeTime() > 0) particle->Update();
//		else
//		{
//			auto& it = std::find(particles.begin(), particles.end(), particle);
//			if (it != particles.end()) particles.erase(it);
//		}
//	}
//}
//
//// 描画処理
//void ParticleManager::Render(const RenderContext& rc)
//{
//	for (auto& particle : particles)
//	{
//		particle->Render(rc);
//	}
//}
//
//// 要素全削除
//void ParticleManager::Clear()
//{
//	for (auto& particle : particles)
//	{
//		auto& it = std::find(particles.begin(), particles.end(), particle);
//		if (it != particles.end())
//		{
//			particles.erase(it);
//		}
//	}
//}
//
//// パーティクル登録
//void ParticleManager::Register(std::unique_ptr<Particle> particle)
//{
//	particles.emplace_back(particle);
//}