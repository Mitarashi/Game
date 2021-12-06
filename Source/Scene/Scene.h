#pragma once

#include <string>

// �V�[��
class Scene
{
public:
	// �R���X�g���N�^
	Scene() = default;

	// �f�X�g���N�^
	virtual ~Scene() = default;

	// ������
	virtual void Initialize() = 0;

	// �I����
	virtual void Finalize() = 0;

	// �X�V����
	virtual void Update() = 0;

	// �`�揈��
	virtual void Render() = 0;

	// ImGui�`��
	virtual void OnGui();

	// �������I���ς݂��ǂ���
	bool IsReady() const { return ready; }

	// �����������ݒ�
	void SetReady() { ready = true; }

private:
	bool ready = false;
	bool isLoadScene = false;

protected:
	std::string sceneName = {};
	int nextScene = 0;
};