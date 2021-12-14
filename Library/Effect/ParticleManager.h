#pragma once

#include "../Library/Effect/Particle.h"

class ParticleManager
{
private:
	// �R���X�g���N�^
	ParticleManager() = default;

	// �f�X�g���N�^
	~ParticleManager() = default;

public:
	// �R���X�g���N�^
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager(ParticleManager&&) = delete;

	// �I�y���[�^�[
	ParticleManager& operator= (const ParticleManager&) = delete;
	ParticleManager& operator= (ParticleManager&&) = delete;

	// �C���X�^���X�쐬
	static void Create();

	// �C���X�^���X�폜
	static void Destroy();

	// �C���X�^���X�擾
	static ParticleManager& Instance() { return *instance; }

	// �X�V����
	void Update();

	// �`�揈��
	void Render(const RenderContext& renderContext);

	// �v�f�S�폜
	void Clear();

	// �p�[�e�B�N���o�^
	//void Register(std::unique_ptr<Particle> particle);

private:
	inline static ParticleManager* instance = nullptr;

	//std::vector<std::unique_ptr<Particle>> particles = {};
};