#include "../Library/Effect/ParticleSystem.h"

ParticleSyetem::ParticleSyetem(std::shared_ptr<Texture> texture, int num)
{
	particleData.resize(num);
}