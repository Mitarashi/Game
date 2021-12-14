#pragma once

#include <vector>

#include "../Shader/RenderContext.h"

class DebugRenderer
{
public:
	// �R���X�g���N�^
	DebugRenderer();

	// �f�X�g���N�^
	~DebugRenderer() = default;

	// �`����s
	void Render(const RenderContext& renderContext);

	// ���`��
	void DrawSphere(
		const Vec3& center,
		float		radius,
		const Vec4& color);

	// �~���`��
	void DrawCylinder(
		const Vec3& position,
		float		radius,
		float		height,
		const Vec4& color);

private:
	// �����b�V���쐬
	void CreateSphereMesh(
		float			radius,
		int				slices,
		int				stacks);

	// �~�����b�V���쐬
	void CreateCylinderMesh(
		float			radius1,
		float			radius2,
		float			start,
		float			height,
		int				slices,
		int				stacks);

private:
	struct CBufferMesh
	{
		DirectX::XMFLOAT4X4 worldViewProjection	= {};
		Vec4				color				= {};
	};

	struct Sphere
	{
		Vec4	color	= {};
		Vec3	center	= {};
		float	radius	= 0.0f;
	};

	struct Cylinder
	{
		Vec4	color		= {};
		Vec3	position	= {};
		float	radius		= 0.0f;
		float	height		= 0.0f;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>		sphereVectexBuffer		= nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		cylinderVertexBuffer	= nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		constantBuffer			= nullptr;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader			= nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader				= nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout				= nullptr;

	std::vector<Sphere>							spheres					= {};
	std::vector<Cylinder>						cylinders				= {};

	u_int										sphereVertexCount		= 0;
	u_int										cylinderVertexCount		= 0;
};