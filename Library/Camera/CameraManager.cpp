#include "../Math.h"
#include "../DxSystem.h"
#include "../Input/InputManager.h"

#include "CameraManager.h"

// �R���X�g���N�^
CameraManager::CameraManager() : camera(std::make_unique<Camera>())
{
}

// �C���X�^���X�쐬
void CameraManager::Create()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(���ɃC���X�^���X������Ă��܂�)");
	instance = new CameraManager;
}

// �C���X�^���X�폜
void CameraManager::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// �J�����̏�����
void CameraManager::Initialize(const Vec3& eye, const Vec3& focus, const Vec3& up)
{
	DxSystem& dxSystem = DxSystem::Instance();

	// �X�N���[���T�C�Y�擾
	const float screenWidth = dxSystem.GetScreenWidth();
	const float screenheight = dxSystem.GetScreenHeight();

	// �J�������w������Ɍ�����
	camera->SetLookAt(eye, focus, up);

	// �������e�ݒ�
	const float fovY = Math::ToRadian(45);
	const float aspect = screenWidth / screenheight;
	const float nearZ = 0.1f;
	const float farZ = 1000.0f;
	camera->SetPerspectiveFov(fovY, aspect, nearZ, farZ);
}

// �J�����̍X�V
void CameraManager::Update()
{
	// �X�e�B�b�N���͎擾
	GamePad& gamePad = InputManager::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();

	// �J�����̉�]���x
	float speed = rollSpeed * DxSystem::Instance().GetDeltaTime();

	// �X�e�B�b�N�̓��͂ɍ��킹��X����Y������]
	angle.x += ay * speed;
	angle.y += ax * speed;

	// X���̃J������]�𐧌�
	if (angle.x < MIN_ANGLE_X) angle.x = MIN_ANGLE_X;
	if (angle.x > MAX_ANGLE_X) angle.x = MAX_ANGLE_X;

	// Y���̉�]�l��-3.14~3.14�Ɏ��܂�悤�ɂ���
	angle.y = Math::AngleNormalize(angle.y);

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// ��]�s�񂩂�O���x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	Vec3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	Vec3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	// �J�����Ɏ��_�ƒ����_��ݒ�
	camera->SetLookAt(eye, target, Vec3(0, 1, 0));
}