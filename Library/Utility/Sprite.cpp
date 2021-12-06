#include <cstdio>
#include <WICTextureLoader.h>

#include "../Misc.h"
#include "../Math.h"
#include "../DxSystem.h"

#include "Sprite.h"

// �R���X�g���N�^
Sprite::Sprite(const char* fileName)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	HRESULT hr = S_OK;

	// ���_�f�[�^�̒�`
	Vertex vertices[] = {
		{ Vec3(-0.5, +0.5, 0), Vec2( 0, 0),Vec4(1, 1, 1, 1) },
		{ Vec3(+0.5, +0.5, 0), Vec2( 1, 0),Vec4(1, 0, 0, 1) },
		{ Vec3(-0.5, -0.5, 0), Vec2( 0, 1),Vec4(0, 1, 0, 1) },
		{ Vec3(+0.5, -0.5, 0), Vec2( 1, 1),Vec4(0, 0, 1, 1) },
	};

	// ���_�o�b�t�@�̐���
	{
		// ���_�o�b�t�@���쐬���邽�߂̐ݒ�I�v�V����
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth			= sizeof(vertices);
		bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags			= 0;
		bufferDesc.StructureByteStride = 0;

		// ���_�o�b�t�@�ɒ��_�f�[�^�����邽�߂̐ݒ�
		D3D11_SUBRESOURCE_DATA resource = {};
		resource.pSysMem = vertices;
		resource.SysMemPitch = 0;
		resource.SysMemSlicePitch = 0;

		// ���_�o�b�t�@�I�u�W�F�N�g�̐���
		hr = device->CreateBuffer(&bufferDesc, &resource, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���_�V�F�[�_�[,�s�N�Z���V�F�[�_�[,���̓��C�A�E�g����
	{
		// ���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		const UINT numElements = ARRAYSIZE(inputElementDesc);

		hr = ResourceManager::Instance().CreateVSFromCSO(
			"Shader/Sprite_vs.cso",
			vertexShader.GetAddressOf(),
			inputLayout.GetAddressOf(),
			inputElementDesc,
			numElements
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = ResourceManager::Instance().CreatePSFromCSO(
			"Shader/Sprite_ps.cso",
			pixelShader.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �T���v���X�e�[�g
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.MipLODBias		= 0.0f;
		desc.MaxAnisotropy	= 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD			= -FLT_MAX;
		desc.MaxLOD			= FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		hr = device->CreateSamplerState(&desc, samplerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �e�N�X�`���̐���
	if (fileName != nullptr)
	{
		D3D11_TEXTURE2D_DESC desc = {};

		ResourceManager::Instance().LoadTextureFromFile(
			fileName,
			shaderResourceView.GetAddressOf(),
			&desc
		);

		textureWidth = desc.Width;
		textureHeight = desc.Height;
	}
	else
	{
		D3D11_TEXTURE2D_DESC desc = {};

		ResourceManager::Instance().CreateDummyTexture(
			shaderResourceView.GetAddressOf(),
			&desc);
	}
}

// �`����s
void Sprite::Render(
	const Vec2& pos,	const Vec2& scale,
	const Vec2& texPos, const Vec2& texSize,
	float angle, const Vec4& color) const
{
	DxSystem& dxSystem = DxSystem::Instance();
	ID3D11DeviceContext* dc = dxSystem.GetDeviceContext();

	{
		// ���ݐݒ肳��Ă���r���[�|�[�g����X�N���[���T�C�Y���擾����B
		D3D11_VIEWPORT viewport;
		UINT numViewports = 1;
		dc->RSGetViewports(&numViewports, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;

		// �X�v���C�g���\������S���_�̃X�N���[�����W���v�Z����
		Vec2 positions[] = {
			Vec2(pos.x,				pos.y),				// ����
			Vec2(pos.x + scale.x,	pos.y),				// �E��
			Vec2(pos.x,				pos.y + scale.y),	// ����
			Vec2(pos.x + scale.x,	pos.y + scale.y),	// �E��
		};

		// �X�v���C�g���\������S���_�̃e�N�X�`�����W���v�Z����
		Vec2 texcoords[] = {
			Vec2(texPos.x,				texPos.y),				// ����
			Vec2(texPos.x + texSize.x,	texPos.y),				// �E��
			Vec2(texPos.x,				texPos.y + texSize.y),	// ����
			Vec2(texPos.x + texSize.x,	texPos.y + texSize.y),	// �E��
		};

		// �X�v���C�g�̒��S�ŉ�]������
		// �X�N���[�����W�n����NDC���W�n�֕ϊ�����B
		float mx = pos.x + scale.x * 0.5f;
		float my = pos.y + scale.y * 0.5f;
		float theta = Math::ToRadian(angle);
		float c = cosf(theta);
		float s = sinf(theta);

		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;

			Vec2 temp = p;
			p.x = c * temp.x + -s * temp.y;
			p.y = s * temp.x + c * temp.y;

			p.x += mx;
			p.y += my;

			p.x = 2.0f * p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f * p.y / screen_height;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = color.x;
			v[i].color.y = color.y;
			v[i].color.z = color.z;
			v[i].color.w = color.w;

			v[i].texcoord.x = texcoords[i].x / textureWidth;
			v[i].texcoord.y = texcoords[i].y / textureHeight;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���I������B
		dc->Unmap(vertexBuffer.Get(), 0);
	}

	{
		// �p�C�v���C���ݒ�
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		dc->IASetInputLayout(inputLayout.Get());

		dc->RSSetState(dxSystem.GetRasterizerState(DxSystem::RS::CULL_NONE));

		dc->VSSetShader(vertexShader.Get(), nullptr, 0);
		dc->PSSetShader(pixelShader.Get(), nullptr, 0);

		dc->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
		dc->PSSetSamplers(0, 1, samplerState.GetAddressOf());
		dc->OMSetDepthStencilState(dxSystem.GetDepthStencilState(DxSystem::DS::TRUE), 1);

		// �`��
		dc->Draw(4, 0);
	}
}