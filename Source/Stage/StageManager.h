#pragma once
#include <vector>

#include "../Source/Stage/Stage.h"

// �X�e�[�W�}�l�[�W���[
class StageManager
{
private:
	// �R���X�g���N�^
	StageManager() = default;

	// �f�X�g���N�^
	~StageManager() = default;

public:
	// �R���X�g���N�^
	StageManager(const StageManager&) = delete;
	StageManager(StageManager&&) = delete;

	// �I�y���[�^�[
	StageManager& operator= (const StageManager&) = delete;
	StageManager& operator= (StageManager&&) = delete;

	// �C���X�^���X�쐬
	static void Create();

	// �C���X�^���X�폜
	static void Destory();

	// �C���X�^���X�擾
	static StageManager& Instance() { return *instance; }

	// �X�V����
	void Update();

	// �`�揈��
	void Render(const RenderContext& renderContext);

	// �X�e�[�W�o�^
	void Register(std::shared_ptr<Stage>& stage) { stages.emplace_back(stage); }

	// �X�e�[�W�S�폜
	void Clear();

	// ���C�L���X�g
	bool RayCast(const Vec3& start,
		const Vec3& end,
		HitResult& hit);

private:
	inline static StageManager* instance = nullptr;
	std::vector<std::shared_ptr<Stage>> stages;
};

