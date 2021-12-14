#pragma once

#include "../Library/Effect/Particle.h"

class ParticleFire : public Particle
{
public:
	// �R���X�g���N�^
	ParticleFire(std::shared_ptr<Texture> texture, float lifeTimer, DxSystem::BS state, int numParticle);

	// �f�X�g���N�^
	~ParticleFire() override = default;

	// �X�V����
	void Update() override;

	// �`�揈��
	void Render(const RenderContext& renderContext);
};