#pragma once

// テンプレートの定義
template<class Type>

// Singletonクラス
class Singleton
{
public:
	// インスタンス取得
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
