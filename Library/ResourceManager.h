#pragma once

#include <map>
#include <memory>
#include <string>
#include <WICTextureLoader.h>

#include "Utility/Singleton.h"
#include "Model/ModelResource.h"

// ���\�[�X�}�l�[�W���[
class ResourceManager : public Singleton<ResourceManager>
{
public:
	friend class Singleton<ResourceManager>;

private:
	// �R���X�g���N�^
	ResourceManager() = default;

	// �f�X�g���N�^
	~ResourceManager() = default;

public:
	// ���f�����\�[�X�ǂݍ���
	std::shared_ptr<ModelResource> LoadModelResource(const char* fileName);

	// �e�N�X�`���ǂݍ���
	HRESULT LoadTextureFromFile(
		const char*					fileName,
		ID3D11ShaderResourceView**	shaderResourceView,
		D3D11_TEXTURE2D_DESC*		textureDesc);

	// �_�~�[�e�N�X�`������
	HRESULT CreateDummyTexture(
		ID3D11ShaderResourceView**	shaderResourceView,
		D3D11_TEXTURE2D_DESC*		textureDesc);

	// VertexShader�ǂݍ���
	HRESULT CreateVSFromCSO(
		const char*					csoName,
		ID3D11VertexShader**		vertexShader,
		ID3D11InputLayout**			inputLayout,
		D3D11_INPUT_ELEMENT_DESC*	inputElementDesc,
		UINT						numElements);

	// PixelShader�ǂݍ���
	HRESULT CreatePSFromCSO(
		const char*					csoName,
		ID3D11PixelShader**			piexlShader);

	// GeometryShader�ǂݍ���
	HRESULT CreateGSFromCSO(
		const char*					csoName,
		ID3D11GeometryShader**		geometryShader
	);

private:
	struct VSData
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout = nullptr;

		// �R���X�g���N�^
		VSData(ID3D11VertexShader* vs, ID3D11InputLayout* inputLayout)
			:vertexShader(vs), inputLayout(inputLayout) {}

		//�f�X�g���N�^
		~VSData() = default;
	};

private:
	std::map<std::string, std::weak_ptr<ModelResource>>							models;
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>	textures;
	std::map<std::string, VSData>												vertexShaders;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>>			pixelShaders;
	std::map<std::string, Microsoft::WRL::ComPtr<ID3D11GeometryShader>>			geometryShaders;
};