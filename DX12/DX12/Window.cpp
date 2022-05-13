#include<tchar.h>
#include "Window.h"

// �f�X�g���N�^
Window::~Window()
{
	//�E�B���h�E���
	UnregisterClass(m_window.lpszClassName, m_window.hInstance);
}

// �E�B���h�E�v���V�[�W���\
LRESULT Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		// �E�B���h�E������Ƃ�
	case WM_CLOSE:
		// ����ۂɃE�B���h�E��j������
		DestroyWindow(hwnd);
		return 0;
		// �E�B���h�E���j�����ꂽ��
	case WM_DESTROY:
		// OS�ɑ΂��ăA�v���P�[�V�����I����`����
		PostQuitMessage(0);
		return 0;
	}

	// �K��̏������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// �E�B���h�E�̐���
void Window::CreateWnd()
{
	// WNDCLASSEX�̃T�C�Y���w��
	m_window.cbSize = sizeof(WNDCLASSEX);

	// �E�B���h�E�̔w�i�F���w��
	m_window.hbrBackground = CreateSolidBrush(0x000000);
	// �}�E�X�J�[�\���̎w��
	m_window.hCursor = LoadCursor(NULL, IDC_ARROW);
	// �g�p����A�C�R���̎w��
	m_window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	// �C���X�^���X�n���h���̎w��
	m_window.hInstance = GetModuleHandle(0);
	// �E�B���h�E�v���V�[�W���\�̎w��
	m_window.lpfnWndProc = (WNDPROC)WindowProcedure;
	// �E�B���h�E�̖��O���w��
	m_window.lpszClassName = _T("DirectX12");
	// �E�B���h�E���j���[�ɖ��O���w��
	m_window.lpszMenuName = _T("DirectX12");
	// �E�B���h�E�̓o�^
	RegisterClassEx(&m_window);

	// �E�B���h�E�̃T�C�Y�w��
	// �c��
	m_rect.bottom = WindowHeight;
	m_rect.left   = 0;
	// ����
	m_rect.right  = WindowWidth;
	m_rect.top    = 0;
	// �T�C�Y�̕␳
	AdjustWindowRect(&m_rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E����
	m_handle = CreateWindow(
		m_window.lpszClassName,
		_T("DirectX12"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(m_rect.right  - m_rect.left),
		(m_rect.bottom - m_rect.top),
		nullptr,
		nullptr,
		m_window.hInstance,
		nullptr);
}


