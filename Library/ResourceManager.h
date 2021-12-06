#pragma once

#include <map>
#include <memory>
#include <string>
#include <WICTextureLoader.h>

#include "Utility/Singleton.h"
#include "Model/ModelResource.h"

// リソースマネージャー
class ResourceManager : public Singleton<ResourceManager>
{
public:
	friend class Singleton<ResourceManager>;

private:
	// コンストラクタ
	ResourceManager() = default;

	// デストラクタ
	~ResourceManager() = default;

public:
	// モデルリソース読み込み
	std::shared_ptr<ModelResource> LoadModelResource(const char* fileName);

	// テクスチャ読み込み
	HRESULT LoadTextureFromFile(
		const char*					fileName,
		ID3D11ShaderResourceView**	shaderResourceView,
		D3D11_TEXTURE2D_DESC*		textureDesc);

	// ダミーテクスチャ生成
	HRESULT CreateDummyTexture(
		ID3D11ShaderResourceView**	shaderResourceView,
		D3D11_TEXTURE2D_DESC*		textureDesc);

	// VertexShader読み込み
	HRESULT CreateVSFromCSO(
		const char*					csoName,
		ID3D11VertexShader**		vertexShader,
		ID3D11InputLayout**			inputLayout,
		D3D11_INPUT_ELEMENT_DESC*	inputElementDesc,
		UINT						numElements);

	// PixelShader読み込み
	HRESULT CreatePSFromCSO(
		const char*					csoName,
		ID3D11PixelShader**			piexlShader);

	// GeometryShader読み込み
	HRESULT CreateGSFromCSO(
		const char*					csoName,
		ID3D11GeometryShader**		geometryShader
	);

private:
	struct VSData
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout = nullptr;

		// コンストラクタ
		VSData(ID3D11VertexShader* vs, ID3D11InputLayout* inputLayout)
			:vertexShader(vs), inputLayout(inputLayout) {}

		//デストラクタ
		~VSData() = default;
	};

private:
	std::map<std::string, std::weak_ptr<ModelResource>>							models;
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	textures;
	std::map<std::string, VSData>												vertexShaders;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>>			pixelShaders;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11GeometryShader>>			geometryShaders;
};