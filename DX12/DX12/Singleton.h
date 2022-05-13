#pragma once
#include <assert.h>

// �V���O���g���N���X
template<class T>
class Singleton
{
protected:
	// �R���X�g���N�^�̊O���ł̋֎~
	Singleton() {};
	virtual ~Singleton() {}
private:
	// �R�s�[�R���X�g���N�^�̎g�p�֎~
	Singleton(const Singleton&);
	Singleton& operator = (const Singleton&);

	// �C���X�^���X
	static T* m_pInstance;
public:
	// ����
	static void Create()
	{
		assert(!m_pInstance && "�C���X�^���X�̐����Ɏ��s���܂���");
		m_pInstance = new T;
	}

	// �C���X�^���X
	static T& Instance()
	{
		assert(m_pInstance && "�C���X�^���X�̎擾�Ɏ��s���܂���");
		return *m_pInstance;
	}

	// ���
	static void Release()
	{
		assert(m_pInstance && "�C���X�^���X�̉���Ɏ��s���܂���");
		delete m_pInstance;
	}
};

template<class T>
T* Singleton<T>::m_pInstance = nullptr;