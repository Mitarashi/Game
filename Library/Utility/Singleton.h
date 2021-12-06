#pragma once

// �e���v���[�g�̒�`
template<class Type>

// Singleton�N���X
class Singleton
{
public:
	// �C���X�^���X�擾
	static Type& Instance()
	{
		static Type inst = {};
		return inst;
	}

protected:
	Singleton() = default;
	virtual ~Singleton() = default;

private:
	void operator=(const Singleton& obj) = delete;
	Singleton(const Singleton& obj) = delete;
};
