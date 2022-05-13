#pragma once
#include<Windows.h>

// �E�B���h�E�̕��E����
const int WindowWidth  = 1280;
const int WindowHeight = 720;

//�E�B���h�E�N���X
class Window
{
private:
	//�E�B���h�E�v���V�[�W���\
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	//�E�B���h�E�����i�[����\����
	WNDCLASSEX m_window;
	//�E�B���h�E�̃T�C�Y���i�[����\����
	RECT m_rect;
	//�E�B���h�E�n���h��
	HWND m_handle;
public:
	//�R���X�g���N�^
	Window() = default;
	//�f�X�g���N�^
	~Window();
	//�E�B���h�E����
	void CreateWnd();
	//�E�B���h�E�n���h���̎擾
	HWND GetHandle() const { return m_handle; }
};

