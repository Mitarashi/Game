#include "../Library/DxSystem.h"
#include "../Library/Shader/NoLightShader.h"

#include "StageMain.h"

// �R���X�g���N�^
StageMain::StageMain()
{
	// �X�e�[�W���f����ǂݍ���
	model = std::make_unique<Model>("Data/Model/ExampleStage/ExampleStage.mdl");

	// TODO ����
	shader = std::make_unique<NoLightShader>();
}

// �X�V����
void StageMain::Update()
{
}

// �`�揈��
void StageMain::Render(const RenderContext& rc)
{
	// �V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
	shader->Begin(rc);
	shader->Render(model.get());
	shader->End();
}

// ���C�L���X�g
bool StageMain::RayCast(const Vec3& start,
	const Vec3& end,
	HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model.get(), hit);
}
