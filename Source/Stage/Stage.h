#pragma once

#include "../Library/Misc.h"
#include "../Library/Shader/RenderContext.h"

#include "../Collision.h"

// �X�e�[�W
class Stage : public Obj3d
{
public:
	// �R���X�g���N�^
	Stage() = default;

	// �f�X�g���N�^
	virtual ~Stage() = default;

	// �X�V����
	virtual void Update() = 0;

	// �`�揈��
	virtual void Render(const RenderContext& renderContext) = 0;

	// ���C�L���X�g
	virtual bool RayCast(const Vec3& start,
		const Vec3& end,
		HitResult& hit) = 0;
};