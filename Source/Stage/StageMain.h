#pragma once

#include "../Library/Model/Model.h"
#include "../Library/Shader/Shader.h"

#include "Box.h"
#include "Stage.h"

// ���C���X�e�[�W
class StageMain : public Stage
{
public:
	// �R���X�g���N�^
	StageMain();

	// �f�X�g���N�^
	~StageMain() override = default;

	// �X�V����
	void Update() override;

	// �`�揈��
	void Render(const RenderContext& renderContext);

	// ���C�L���X�g
	bool RayCast(const Vec3& start,
		const Vec3& end,
		HitResult& hit) override;

private:
	std::unique_ptr<Box> box = nullptr;
};