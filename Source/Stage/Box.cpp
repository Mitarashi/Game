#include "../Library/DxSystem.h"
#include "../Library/Shader/NoLightShader.h"

#include "Box.h"

// �R���X�g���N�^
Box::Box()
{
	model = std::make_unique<Model>("Data/Model/Cube/Cube.mdl");

	shader = std::make_unique<NoLightShader>();
}

// �X�V����
void Box::Update()
{
	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

// �`�揈��
void Box::Render(const RenderContext& rc)
{
	shader->Begin(rc);
	shader->Render(model.get());
	shader->End();
}

// �f�o�b�O�pGUI�`��
void Box::DebugGUI()
{

}

// �f�o�b�O�v���~�e�B�u�`��
void Box::DebugPrimitive()
{
}
