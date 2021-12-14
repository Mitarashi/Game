#pragma once

#include <wrl.h>
#include <string>
#include <vector>
#include <d3d11.h>

#include "../Library/Vector.h"

class ModelResource
{
public:
	ModelResource()		= default;
	~ModelResource()	= default;

	using NodeId = UINT64;
	struct Node
	{
		NodeId		id			= 0;
		std::string	name		= {};
		std::string	path		= {};
		int			parentIndex = 0;
		Vec3		scale		= {};
		Vec4		rotate		= {};
		Vec3		translate	= {};

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Material
	{
		std::string	name				= {};
		std::string	textureFilename		= {};
		Vec4		color				= { 0.8f, 0.8f, 0.8f, 1.0f };

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView = nullptr;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Subset
	{
		UINT		startIndex		= 0;
		UINT		indexCount		= 0;
		int			materialIndex	= 0;

		Material*	material		= nullptr;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Vertex
	{
		Vec3				position	= {};
		Vec3				normal		= {};
		Vec3				tangent		= {};
		Vec2				texcoord	= {};
		Vec4				color		= {};
		Vec4				boneWeight	= { 1, 0, 0, 0 };
		DirectX::XMUINT4	boneIndex	= {};

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Mesh
	{
		std::vector<Vertex>						vertices			= {};
		std::vector<UINT>						indices				= {};
		std::vector<Subset>						subsets				= {};

		int										nodeIndex			= {};
		std::vector<int>						nodeIndices			= {};
		std::vector<DirectX::XMFLOAT4X4>		offsetTransforms	= {};

		Vec3									boundsMin			= {};
		Vec3									boundsMax			= {};

		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer		= nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	indexBuffer			= nullptr;

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct NodeKeyData
	{
		Vec3	scale		= {};
		Vec4	rotate		= {};
		Vec3	translate	= {};

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct KeyFrame
	{
		float						seconds		= 0.0f;
		std::vector<NodeKeyData>	nodeKeys	= {};

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

	struct Animation
	{
		std::string					name			= {};
		float						secondsLength	= 0.0f;
		std::vector<KeyFrame>		keyFrames		= {};

		template<class Archive>
		void serialize(Archive& archive, int version);
	};

public:
	// 各種データ取得
	const std::vector<Mesh>& GetMeshes()			const { return meshes; }
	const std::vector<Node>& GetNodes()				const { return nodes; }
	const std::vector<Animation>& GetAnimations()	const { return animations; }
	const std::vector<Material>& GetMaterials()		const { return materials; }

	// 読み込み
	void Load(const char* filename);

protected:
	// シリアライズ
	void Serialize(const char* filename);

	// デシリアライズ
	void Deserialize(const char* filename);

private:
	// モデルセットアップ
	void BuildModel(const char* dirname);

protected:
	std::vector<Node>		nodes		= {};
	std::vector<Material>	materials	= {};
	std::vector<Mesh>		meshes		= {};
	std::vector<Animation>	animations	= {};
};
