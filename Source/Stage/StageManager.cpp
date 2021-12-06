#include "Stage/StageManager.h"

// �C���X�^���X�쐬
void StageManager::Create()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(���ɃC���X�^���X������Ă��܂�)");
	instance = new StageManager;
}

// �C���X�^���X�폜
void StageManager::Destory()
{
	if (instance != nullptr)
	{
		instance->Clear();
		delete instance;
		instance = nullptr;
	}
}

// �X�V����
void StageManager::Update()
{
	for (auto& stage : stages)
	{
		stage->Update();
	}
}

// �`�揈��
void StageManager::Render(const RenderContext& rc)
{
	for (auto& stage : stages)
	{
		stage->Render(rc);
	}
}

// �X�e�[�W�S�폜
void StageManager::Clear()
{
	for (auto& stage : stages)
	{
		std::vector<std::shared_ptr<Stage>>::iterator it = std::find(stages.begin(), stages.end(), stage);
		if (it != stages.end()) stages.erase(it);
	}
}

// ���C�L���X�g
bool StageManager::RayCast(const Vec3& start,
	const Vec3& end,
	HitResult& hit)
{
	bool result = false;
	hit.distance = FLT_MAX;
	for (auto& stage : stages)
	{
		HitResult tmp = {};
		if (stage->RayCast(start, end, tmp))
		{
			if (hit.distance > tmp.distance)
			{
				hit = tmp;
				result = true;
			}
		}
	}

	return result;
}