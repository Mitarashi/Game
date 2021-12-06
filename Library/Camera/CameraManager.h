#pragma once

#include "../Library/Math.h"
#include "../Library/Camera/Camera.h"

// �J�����}�l�[�W���[
class CameraManager
{
private:
	// �R���X�g���N�^
	CameraManager();

	// �f�X�g���N�^
	~CameraManager() = default;

public:
	// �R���X�g���N�^
	CameraManager(const CameraManager&) = delete;
	CameraManager(CameraManager&&) = delete;

	// �I�y���[�^�[
	CameraManager& operator= (const CameraManager&) = delete;
	CameraManager& operator= (CameraManager&&) = delete;

	// �C���X�^���X�쐬
	static void Create();

	// �C���X�^���X�폜
	static void Destroy();

	// �C���X�^���X�擾
	static CameraManager& Instance() { return *instance; }

	// ������
	void Initialize(
		const Vec3& eye		= Vec3(0, 10, -10),
		const Vec3& focus	= Vec3(0, 0, 0),
		const Vec3& up		= Vec3(0, 1, 0));

	// �X�V����
	void Update();

	// �^�[�Q�b�g�ݒ�
	void SetTarget(const Vec3& target_) { target = target_; }

	// �Q�b�^�[
	const std::unique_ptr<Camera>& GetMainCamera() const { return camera; }

private:
	// �萔
	static constexpr float MAX_ANGLE_X = DirectX::XMConvertToRadians( 45);
	static constexpr float MIN_ANGLE_X	= DirectX::XMConvertToRadians(-45);
	static constexpr float ROLL_SPEED	= DirectX::XMConvertToRadians(90);

private:
	inline static CameraManager*	instance	= nullptr;

	// �ϐ�
	std::unique_ptr<Camera>			camera		= nullptr;

	Vec3							target		= {};
	Vec3							angle		= {};
	float							rollSpeed	= Math::ToRadian(90);
	float							range		= 10.0f;
};