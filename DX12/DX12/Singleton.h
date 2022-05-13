#pragma once
#include <assert.h>

// シングルトンクラス
template<class T>
class Singleton
{
protected:
	// コンストラクタの外部での禁止
	Singleton() {};
	virtual ~Singleton() {}
private:
	// コピーコンストラクタの使用禁止
	Singleton(const Singleton&);
	Singleton& operator = (const Singleton&);

	// インスタンス
	static T* m_pInstance;
public:
	// 生成
	static void Create()
	{
		assert(!m_pInstance && "インスタンスの生成に失敗しました");
		m_pInstance = new T;
	}

	// インスタンス
	static T& Instance()
	{
		assert(m_pInstance && "インスタンスの取得に失敗しました");
		return *m_pInstance;
	}

	// 解放
	static void Release()
	{
		assert(m_pInstance && "インスタンスの解放に失敗しました");
		delete m_pInstance;
	}
};

template<class T>
T* Singleton<T>::m_pInstance = nullptr;