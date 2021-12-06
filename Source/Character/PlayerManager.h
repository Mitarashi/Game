#pragma once

#include <memory>

#include "../Library/Misc.h"

#include "../Source/Character/Player.h"

// �v���C���[�}�l�[�W���[
class PlayerManager
{
private:
	// �R���X�g���N�^
	PlayerManager() = default;

	// �f�X�g���N�^
	~PlayerManager() = default;

public:
	// �R���X�g���N�^
	PlayerManager(const PlayerManager&) = delete;
	PlayerManager(PlayerManager&&) = delete;

	// �I�y���[�^�[
	PlayerManager& operator= (const PlayerManager&) = delete;
	PlayerManager& operator= (PlayerManager&&) = delete;

	// �C���X�^���X�쐬
	static void Create();

	// �C���X�^���X�폜
	static void Destroy();

	// �C���X�^���X�擾
	static PlayerManager& Instance() { return* instance; };

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Render(const RenderContext& renderContext);

	// �f�o�b�O�pGUI�`��
	void DebugGUI();

	// �f�o�b�O�v���~�e�B�u�`��
	void DebugPrimitive();

	// �v���C���[�擾
	const std::unique_ptr<Player>& GetPlayer() const { return player; }

private:
	inline static PlayerManager* instance = nullptr;
	std::unique_ptr<Player> player = nullptr;
};