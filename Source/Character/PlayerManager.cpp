#include "Character/PlayerManager.h"

// �C���X�^���X�쐬
void PlayerManager::Create()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(���ɃC���X�^���X������Ă��܂�)");
	instance = new PlayerManager;
}

// �C���X�^���X�폜
void PlayerManager::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// ����������
void PlayerManager::Initialize()
{
	player = std::make_unique<Player>();
}

// �X�V����
void PlayerManager::Update()
{
	player->Update();
}

// �`�揈��
void PlayerManager::Render(const RenderContext& rc)
{
	player->Render(rc);
}

// �f�o�b�O�pGUI�`��
void PlayerManager::DebugGUI()
{
	player->DebugGUI();
}

// �f�o�b�O�v���~�e�B�u�`��
void PlayerManager::DebugPrimitive()
{
	player->DebugPrimitive();
}