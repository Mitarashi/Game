//#include "../Library/Effect/ParticleManager.h"
//
//// �C���X�^���X�쐬
//void ParticleManager::Create()
//{
//	// �C���X�^���X�ݒ�
//	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(���ɃC���X�^���X������Ă��܂�)");
//	instance = new ParticleManager;
//}
//
//// �C���X�^���X�폜
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
//// �X�V����
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
//// �`�揈��
//void ParticleManager::Render(const RenderContext& rc)
//{
//	for (auto& particle : particles)
//	{
//		particle->Render(rc);
//	}
//}
//
//// �v�f�S�폜
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
//// �p�[�e�B�N���o�^
//void ParticleManager::Register(std::unique_ptr<Particle> particle)
//{
//	particles.emplace_back(particle);
//}