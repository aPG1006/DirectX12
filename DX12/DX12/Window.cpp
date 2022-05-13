#include<tchar.h>
#include "Window.h"

// デストラクタ
Window::~Window()
{
	//ウィンドウ解放
	UnregisterClass(m_window.lpszClassName, m_window.hInstance);
}

// ウィンドウプロシージャ―
LRESULT Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		// ウィンドウを閉じたとき
	case WM_CLOSE:
		// 閉じる際にウィンドウを破棄する
		DestroyWindow(hwnd);
		return 0;
		// ウィンドウが破棄された時
	case WM_DESTROY:
		// OSに対してアプリケーション終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 規定の処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// ウィンドウの生成
void Window::CreateWnd()
{
	// WNDCLASSEXのサイズを指定
	m_window.cbSize = sizeof(WNDCLASSEX);

	// ウィンドウの背景色を指定
	m_window.hbrBackground = CreateSolidBrush(0x000000);
	// マウスカーソルの指定
	m_window.hCursor = LoadCursor(NULL, IDC_ARROW);
	// 使用するアイコンの指定
	m_window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	// インスタンスハンドルの指定
	m_window.hInstance = GetModuleHandle(0);
	// ウィンドウプロシージャ―の指定
	m_window.lpfnWndProc = (WNDPROC)WindowProcedure;
	// ウィンドウの名前を指定
	m_window.lpszClassName = _T("DirectX12");
	// ウィンドウメニューに名前を指定
	m_window.lpszMenuName = _T("DirectX12");
	// ウィンドウの登録
	RegisterClassEx(&m_window);

	// ウィンドウのサイズ指定
	// 縦幅
	m_rect.bottom = WindowHeight;
	m_rect.left   = 0;
	// 横幅
	m_rect.right  = WindowWidth;
	m_rect.top    = 0;
	// サイズの補正
	AdjustWindowRect(&m_rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウ生成
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


