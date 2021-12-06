#pragma once

#include <wrl.h>
#include <memory>
#include <d3d11.h>

#include "../Model/Model.h"
#include "../Utility/Vector.h"

#include "RenderContext.h"
#include "ShaderFunction.h"

// Shader�N���X
class Shader
{
public:
	Shader()			= default;
	virtual ~Shader()	= default;

	// �`��J�n����
	virtual void Begin(const RenderContext& renderContext) = 0;

	// �`�揈��
	virtual void Render(const Model* model) = 0;

	// �`��I������
	virtual void End() = 0;

protected:
	static const int MAX_BONES = 128;
};