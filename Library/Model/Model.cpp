#include "../DxSystem.h"
#include "../ResourceManager.h"

#include "ModelResource.h"

#include "Model.h"

// �R���X�g���N�^
Model::Model(const char* fileName)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	// ���\�[�X�ǂݍ���
	resource = ResourceManager::Instance().LoadModelResource(fileName);

	// �m�[�h
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// �s��v�Z
	const DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	UpdateTransform(transform);
}

// �ϊ��s��v�Z
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = Math::MatrixScaling(node.scale);
		DirectX::XMMATRIX R = Math::MatrixRotationQuaternion(node.rotate);
		DirectX::XMMATRIX T = Math::MatrixTranslation(node.translate);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX ParentTransform = {};
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

// �A�j���[�V�����X�V����
void Model::UpdateAnimation(float deltaTime)
{
	// �Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation()) return;

	// �u�����h���̌v�Z
	float blendRate = 1.0f;
	if (animationBlendTime < animationBlendSeconds)
	{
		animationBlendTime += deltaTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		blendRate *= blendRate;
	}

	// �w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	// �A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::KeyFrame>& keyFrames = animation.keyFrames;
	int keyCount = static_cast<int>(keyFrames.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::KeyFrame& keyFrame0 = keyFrames.at(keyIndex);
		const ModelResource::KeyFrame& keyFrame1 = keyFrames.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyFrame0.seconds && currentAnimationSeconds < keyFrame1.seconds)
		{
			// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSeconds - keyFrame0.seconds) / (keyFrame1.seconds - keyFrame0.seconds);
			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// �Q�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyFrame0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyFrame1.nodeKeys.at(nodeIndex);
				Node& node = nodes[nodeIndex];

				// �u�����h�⊮����
				if (blendRate < 1.0f)
				{
					// ���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					// �v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
				// �ʏ�̌v�Z
				else {
					// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					// �v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	// �ŏI�t���[������
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;

		return;
	}

	// ���Ԍo��
	currentAnimationSeconds += deltaTime;

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		// �Đ����Ԃ������߂�
		if (animationLoopFlag)
		{
			currentAnimationSeconds -= animation.secondsLength;
		}
		// �Đ��I�����Ԃɂ���
		else
		{
			currentAnimationSeconds = animation.secondsLength;
			animationEndFlag = true;
		}
	}
}

// �A�j���[�V�����Đ�
void Model::PlayAnimation(int index, bool loop, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

// �A�j���[�V�����Đ�����
bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0) return false;
	if (currentAnimationIndex >= resource->GetAnimations().size()) return false;

	return true;
}

// �m�[�h����
Model::Node* Model::FindNode(const char* name)
{
	// �S�Ẵm�[�h�𑍓�����Ŗ��O��r����
	for (Node& node : nodes)
	{
		if (::strcmp(node.name, name) == 0)
		{
			return &node;
		}
	}

	// ������Ȃ�����
	return nullptr;
}