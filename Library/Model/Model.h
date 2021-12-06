#pragma once

#include <memory>
#include <vector>

#include "../Library/Math.h"

// 前方宣言
class ModelResource;

// モデル
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
	// コンストラクタ
	Model(const char* fileName);

	// デストラクタ
	~Model() = default;

	// 行列更新
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }
		  std::vector<Node>& GetNodes()		  { return nodes; }

	// リソース取得
	const std::shared_ptr<ModelResource> GetResource() const { return resource; }

	// アニメーション更新処理
	void UpdateAnimation(float deltaTime);

	// アニメーション再生
	void PlayAnimation(int index, bool loop, float blendSeconds = 0.2f);

	// アニメーション再生中か
	bool IsPlayAnimation() const;

	// ノード検索
	Node* FindNode(const char* name);

	// 現在のアニメーションの時間
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