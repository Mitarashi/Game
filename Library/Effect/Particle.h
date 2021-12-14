//#pragma once
//
//#include "../Library/DxSystem.h"
//#include "../Library/Utility/Texture.h"
#include "../Library/Shader/RenderContext.h"
//
//class Particle
//{
//public:
//	// �R���X�g���N�^
//	Particle() = default;
//
//	// �f�X�g���N�^
//	virtual ~Particle() = default;
//
//	// ����������
//	virtual void Initialize(int numParticle);
//
//	// �A�j���[�V�����X�V����
//	virtual void UpdateAnimation();
//
//	// �X�V����
//	virtual void Update() = 0;
//
//	// �`�揈��
//	virtual void Render(const RenderContext& renderContext) = 0;
//
//	const float GetLifeTime() const { return lifeTime; }
//
//public:
//	struct ParticleData
//	{
//		Vec3	position		= {};
//		Vec2	size			= {};
//		Vec3	velocity		= {};
//		Vec3	acceleration	= {};
//		float	alpha			= 1.0f;
//		float	timer			= 0.0f;
//		float	animeTimer		= 0.0f;
//		int		type			= -1;
//	};
//
//	struct VertexData
//	{
//		Vec3 position = {};
//		Vec3 Normal = {};
//		Vec2 Texcoord = {};
//		Vec4 Color = {};
//	};
//
//	struct ConstantBuffer
//	{
//		DirectX::XMFLOAT4X4 view = {};
//		DirectX::XMFLOAT4X4 projection = {};
//		float				size = 0.0f;
//		Vec3				dummy = {};
//	};
//
//protected:
//	std::vector<ParticleData>					 particleData	= {};
//	std::vector<VertexData>						 vertexData		= {};
//
//protected:
//	Microsoft::WRL::ComPtr<ID3D11Buffer>		 vertexBuffer	= nullptr;
//	Microsoft::WRL::ComPtr<ID3D11Buffer>		 constantBuffer = nullptr;
//
//	Microsoft::WRL::ComPtr<ID3D11VertexShader>	 vertexShader	= nullptr;
//	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader = nullptr;
//	Microsoft::WRL::ComPtr<ID3D11PixelShader>	 pixelShader	= nullptr;
//	Microsoft::WRL::ComPtr<ID3D11InputLayout>	 inputLayout	= nullptr;
//
//	std::shared_ptr<Texture>					particleTexture = nullptr;
//
//	DxSystem::BS								blendState		= DxSystem::BS::NONE;
//
//	float										lifeTime		= 0.0f;
//	float										animationSpeed	= 0.2f;
//
//	int											numParticle		= 0;
//
//	bool										isAnimation		= false;
//};