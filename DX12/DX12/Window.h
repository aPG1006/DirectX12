#pragma once
#include<Windows.h>

// ウィンドウの幅・高さ
const int WindowWidth  = 1280;
const int WindowHeight = 720;

//ウィンドウクラス
class Window
{
private:
	//ウィンドウプロシージャ―
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	//ウィンドウ情報を格納する構造体
	WNDCLASSEX m_window;
	//ウィンドウのサイズを格納する構造体
	RECT m_rect;
	//ウィンドウハンドル
	HWND m_handle;
public:
	//コンストラクタ
	Window() = default;
	//デストラクタ
	~Window();
	//ウィンドウ生成
	void CreateWnd();
	//ウィンドウハンドルの取得
	HWND GetHandle() const { return m_handle; }
};

