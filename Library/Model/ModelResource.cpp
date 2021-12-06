#include <stdlib.h>
#include <fstream>
#include <functional>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <WICTextureLoader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../Misc.h"
#include "../Debug/Logger.h"
#include "../Library/DxSystem.h"

#include "ModelResource.h"

// CEREAL�o�[�W������`
CEREAL_CLASS_VERSION(ModelResource::Node, 1)
CEREAL_CLASS_VERSION(ModelResource::Material, 1)
CEREAL_CLASS_VERSION(ModelResource::Subset, 1)
CEREAL_CLASS_VERSION(ModelResource::Vertex, 1)
CEREAL_CLASS_VERSION(ModelResource::Mesh, 1)
CEREAL_CLASS_VERSION(ModelResource::NodeKeyData, 1)
CEREAL_CLASS_VERSION(ModelResource::KeyFrame, 1)
CEREAL_CLASS_VERSION(ModelResource::Animation, 1)
CEREAL_CLASS_VERSION(ModelResource, 1)

// �V���A���C�Y
namespace DirectX
{
	template<class Archive>
	void serialize(Archive& archive, XMUINT4& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, Vec2& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, Vec3& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, Vec4& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT4X4& m)
	{
		archive(
			cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12), cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
			cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22), cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
			cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32), cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
			cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42), cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
		);
	}
}

template<class Archive>
void ModelResource::Node::serialize(Archive& archive, [[maybe_unused]] int version)
{
	archive(
		CEREAL_NVP(id),
		CEREAL_NVP(name),
		CEREAL_NVP(path),
		CEREAL_NVP(parentIndex),
		CEREAL_NVP(scale),
		CEREAL_NVP(rotate),
		CEREAL_NVP(translate)
	);
}

template<class Archive>
void ModelResource::Material::serialize(Archive& archive, [[maybe_unused]] int version)
{
	archive(
		CEREAL_NVP(name),
		CEREAL_NVP(textureFilename),
		CEREAL_NVP(color)
	);
}

template<class Archive>
void ModelResource::Subset::serialize(Archive& archive, [[maybe_unused]] int version)
{
	archive(
		CEREAL_NVP(startIndex),
		CEREAL_NVP(indexCount),
		CEREAL_NVP(materialIndex)
	);
}

template<class Archive>
void ModelResource::Vertex::serialize(Archive& archive, [[maybe_unused]] int version)
{
	archive(
		CEREAL_NVP(position),
		CEREAL_NVP(normal),
		CEREAL_NVP(tangent),
		CEREAL_NVP(texcoord),
		CEREAL_NVP(color),
		CEREAL_NVP(boneWeight),
		CEREAL_NVP(boneIndex)
	);
}

template<class Archive>
void ModelResource::Mesh::serialize(Archive& archive, [[maybe_unused]] int version)
{
	archive(
		CEREAL_NVP(vertices),
		CEREAL_NVP(indices),
		CEREAL_NVP(subsets),
		CEREAL_NVP(nodeIndex),
		CEREAL_NVP(nodeIndices),
		CEREAL_NVP(offsetTransforms),
		CEREAL_NVP(boundsMin),
		CEREAL_NVP(boundsMax)
	);
}

template<class Archive>
void ModelResource::NodeKeyData::serialize(Archive& archive, [[maybe_unused]] int version)
{
	archive(
		CEREAL_NVP(scale),
		CEREAL_NVP(rotate),
		CEREAL_NVP(translate)
	);
}

template<class Archive>
void ModelResource::KeyFrame::serialize(Archive& archive, [[maybe_unused]] int version)
{
	archive(
		CEREAL_NVP(seconds),
		CEREAL_NVP(nodeKeys)
	);
}

template<class Archive>
void ModelResource::Animation::serialize(Archive& archive, [[maybe_unused]] int version)
{
	archive(
		CEREAL_NVP(name),
		CEREAL_NVP(secondsLength),
		CEREAL_NVP(keyFrames)
	);
}

// �ǂݍ���
void ModelResource::Load(const char* filename)
{
	// �f�B���N�g���p�X�擾
	char drive[32], dir[256], dirname[256];
	::_splitpath_s(filename, drive, sizeof(drive), dir, sizeof(dir), nullptr, 0, nullptr, 0);
	::_makepath_s(dirname, sizeof(dirname), drive, dir, nullptr, nullptr);

	// �f�V���A���C�Y
	Deserialize(filename);

	// ���f���\�z
	BuildModel(dirname);
}

// ���f���\�z
void ModelResource::BuildModel(const char* dirname)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	for (Material& material : materials)
	{
		// ���΃p�X�̉���
		char fileName[256];
		::_makepath_s(fileName, 256, nullptr, dirname, material.textureFilename.c_str(), nullptr);

		// �}���`�o�C�g�������烏�C�h�����֕ϊ�
		wchar_t wfilename[256];
		::MultiByteToWideChar(CP_ACP, 0, fileName, -1, wfilename, 256);

		// �e�N�X�`���ǂݍ���
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfilename, resource.GetAddressOf(), material.shaderResourceView.GetAddressOf());
		if (FAILED(hr))
		{
			// WIC�ŃT�|�[�g����Ă��Ȃ��t�H�[�}�b�g�̏ꍇ�iTGA�Ȃǁj��
			// STB�ŉ摜�ǂݍ��݂����ăe�N�X�`���𐶐�����
			int width = 0, height = 0, bpp = 0;
			unsigned char* pixels = stbi_load(fileName, &width, &height, &bpp, STBI_rgb_alpha);
			if (pixels != nullptr)
			{
				D3D11_TEXTURE2D_DESC desc = { 0 };
				desc.Width = width;
				desc.Height = height;
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				D3D11_SUBRESOURCE_DATA data;
				::memset(&data, 0, sizeof(data));
				data.pSysMem = pixels;
				data.SysMemPitch = width * 4;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
				hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

				hr = device->CreateShaderResourceView(texture.Get(), nullptr, material.shaderResourceView.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

				// ��n��
				stbi_image_free(pixels);
			}
			else
			{
				// �ǂݍ��ݎ��s������_�~�[�e�N�X�`�������
				if (strcmp("", material.textureFilename.c_str()) != 0)
					LOG("load failed : %s\n", fileName);

				width = 8;
				height = 8;
				UINT pixels[64] = {};
				::memset(pixels, 0xFF, sizeof(pixels));

				D3D11_TEXTURE2D_DESC desc = { 0 };
				desc.Width = width;
				desc.Height = height;
				desc.MipLevels = 1;
				desc.ArraySize = 1;
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				D3D11_SUBRESOURCE_DATA data;
				::memset(&data, 0, sizeof(data));
				data.pSysMem = pixels;
				data.SysMemPitch = width;

				Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
				hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

				hr = device->CreateShaderResourceView(texture.Get(), nullptr, material.shaderResourceView.GetAddressOf());
				_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
			}
		}
	}

	for (Mesh& mesh : meshes)
	{
		// �T�u�Z�b�g
		for (Subset& subset : mesh.subsets)
		{
			subset.material = &materials.at(subset.materialIndex);
		}

		// ���_�o�b�t�@
		{
			D3D11_BUFFER_DESC bufferDesc = {};
			D3D11_SUBRESOURCE_DATA subresourceData = {};

			bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
			//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			subresourceData.pSysMem = mesh.vertices.data();
			subresourceData.SysMemPitch = 0;
			subresourceData.SysMemSlicePitch = 0;

			HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mesh.vertexBuffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}

		// �C���f�b�N�X�o�b�t�@
		{
			D3D11_BUFFER_DESC bufferDesc = {};
			D3D11_SUBRESOURCE_DATA subresourceData = {};

			bufferDesc.ByteWidth = static_cast<UINT>(sizeof(u_int) * mesh.indices.size());
			//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			subresourceData.pSysMem = mesh.indices.data();
			subresourceData.SysMemPitch = 0; //Not use for index buffers.
			subresourceData.SysMemSlicePitch = 0; //Not use for index buffers.
			HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mesh.indexBuffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}
	}
}

// �V���A���C�Y
void ModelResource::Serialize(const char* filename)
{
	std::ofstream ostream(filename, std::ios::binary);
	if (ostream.is_open())
	{
		cereal::BinaryOutputArchive archive(ostream);

		try
		{
			archive(
				CEREAL_NVP(nodes),
				CEREAL_NVP(materials),
				CEREAL_NVP(meshes),
				CEREAL_NVP(animations)
			);
		}
		catch (...)
		{
			LOG("model deserialize failed.\n%s\n", filename);
			return;
		}
	}
}

// �f�V���A���C�Y
void ModelResource::Deserialize(const char* filename)
{
	std::ifstream istream(filename, std::ios::binary);
	if (istream.is_open())
	{
		cereal::BinaryInputArchive archive(istream);

		try
		{
			archive(
				CEREAL_NVP(nodes),
				CEREAL_NVP(materials),
				CEREAL_NVP(meshes),
				CEREAL_NVP(animations)
			);
		}
		catch (...)
		{
			LOG("Model Deserialize Failed.\n%s\n", filename);
			return;
		}
	}
}