#pragma once

#include "../Library/Effect/Particle.h"

class ParticleFire : public Particle
{
public:
	// コンストラクタ
	ParticleFire(std::shared_ptr<Texture> texture, float lifeTimer, DxSystem::BS state, int numParticle);

	// デストラクタ
	~ParticleFire() override = default;

	// 更新処理
	void Update() override;

	// 描画処理
	void Render(const RenderContext& renderContext);
};