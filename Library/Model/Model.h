#pragma once

#include <memory>
#include <vector>

#include "../Library/Math.h"

// �O���錾
class ModelResource;

// ���f��
class Model
{
public:
	struct Node
	{
		const char*			name			= nullptr;
		Node*				parent			= nullptr;
		Vec3				scale			= { 1,1,1 };
		Vec4				rotate			= {};
		Vec3				translate		= {};
		DirectX::XMFLOAT4X4 localTransform	= {};
		DirectX::XMFLOAT4X4 worldTransform	= {};

		std::vector<Node*>	children		= {};
	};

public:
	// �R���X�g���N�^
	Model(const char* fileName);

	// �f�X�g���N�^
	~Model() = default;

	// �s��X�V
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
		  std::vector<Node>& GetNodes()		  { return nodes; }

	// ���\�[�X�擾
	const std::shared_ptr<ModelResource> GetResource() const { return resource; }

	// �A�j���[�V�����X�V����
	void UpdateAnimation(float deltaTime);

	// �A�j���[�V�����Đ�
	void PlayAnimation(int index, bool loop, float blendSeconds = 0.2f);

	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation() const;

	// �m�[�h����
	Node* FindNode(const char* name);

	// ���݂̃A�j���[�V�����̎���
	float GetCurrentAnimationSeconds() const { return currentAnimationSeconds; }

private:
	std::shared_ptr<ModelResource>	resource = nullptr;
	std::vector<Node>				nodes = {};

	int     currentAnimationIndex	= -1;
	float	currentAnimationSeconds = 0.0f;
	bool    animationLoopFlag		= false;
	bool    animationEndFlag		= false;
	float   animationBlendTime		= 0.0f;
	float   animationBlendSeconds	= 0.0f;

};