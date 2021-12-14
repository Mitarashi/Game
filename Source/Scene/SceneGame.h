#pragma once

#include "Scene.h"

#include "../Library/Utility/Sprite.h"
#include "../Library/Utility/SkyBox.h"
#include "../Library/Utility/Texture.h"
#include "../Library/Camera/CameraManager.h"
#include "../Library/Effect/ParticleSystem.h"

#include "../Source/Stage/StageManager.h"
#include "../Source/Character/EnemyManager.h"
#include "../Source/Character/PlayerManager.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	// �R���X�g���N�^
	SceneGame() = default;

	// �f�X�g���N�^
	virtual ~SceneGame() override = default;

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update() override;

	// �`�揈��
	void Render() override;

	// ImGui�`��
	void OnGui() override;

private:
	std::unique_ptr<SkyBox> skyBox = nullptr;
	std::unique_ptr<Sprite> sprite = nullptr;

	bool					debug = true;
	Vec3					spawnPos = {};
	float					spawnRange = 30.0f;

	std::unique_ptr<ParticleSyetem> snowEffect = nullptr;
	std::shared_ptr<Texture>		snowTexture = nullptr;
	std::unique_ptr<Model>			cube = nullptr;
	std::unique_ptr<Shader>			shader = nullptr;
};