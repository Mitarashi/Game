#include "../DxSystem.h"
#include "../ResourceManager.h"

#include "ModelResource.h"

#include "Model.h"

// コンストラクタ
Model::Model(const char* fileName)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	// リソース読み込み
	resource = ResourceManager::Instance().LoadModelResource(fileName);

	// ノード
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

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	UpdateTransform(transform);
}

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX S = Math::MatrixScaling(node.scale);
		DirectX::XMMATRIX R = Math::MatrixRotationQuaternion(node.rotate);
		DirectX::XMMATRIX T = Math::MatrixTranslation(node.translate);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
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

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

// アニメーション更新処理
void Model::UpdateAnimation(float deltaTime)
{
	// 再生中でないなら処理しない
	if (!IsPlayAnimation()) return;

	// ブレンド率の計算
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

	// 指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	// アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::KeyFrame>& keyFrames = animation.keyFrames;
	int keyCount = static_cast<int>(keyFrames.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// 現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::KeyFrame& keyFrame0 = keyFrames.at(keyIndex);
		const ModelResource::KeyFrame& keyFrame1 = keyFrames.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyFrame0.seconds && currentAnimationSeconds < keyFrame1.seconds)
		{
			// 再生時間とキーフレームの時間から補完率を算出する
			float rate = (currentAnimationSeconds - keyFrame0.seconds) / (keyFrame1.seconds - keyFrame0.seconds);
			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				// ２つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyFrame0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyFrame1.nodeKeys.at(nodeIndex);
				Node& node = nodes[nodeIndex];

				// ブレンド補完処理
				if (blendRate < 1.0f)
				{
					// 現在の姿勢と次のキーフレームとの姿勢の補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					// 計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
				// 通常の計算
				else {
					// 前のキーフレームと次のキーフレームの姿勢を補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					// 計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	// 最終フレーム処理
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;

		return;
	}

	// 時間経過
	currentAnimationSeconds += deltaTime;

	// 再生時間が終端時間を超えたら
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		// 再生時間を巻き戻す
		if (animationLoopFlag)
		{
			currentAnimationSeconds -= animation.secondsLength;
		}
		// 再生終了時間にする
		else
		{
			currentAnimationSeconds = animation.secondsLength;
			animationEndFlag = true;
		}
	}
}

// アニメーション再生
void Model::PlayAnimation(int index, bool loop, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

// アニメーション再生中か
bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0) return false;
	if (currentAnimationIndex >= resource->GetAnimations().size()) return false;

	return true;
}

// ノード検索
Model::Node* Model::FindNode(const char* name)
{
	// 全てのノードを総当たりで名前比較する
	for (Node& node : nodes)
	{
		if (::strcmp(node.name, name) == 0)
		{
			return &node;
		}
	}

	// 見つからなかった
	return nullptr;
}