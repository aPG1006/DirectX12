#include <tchar.h>
#include <memory>
#include "Window.h"
#include "DirectX12.h"
#include "Sprite.h"
#include "PipelineState.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// 生成
	DirectX12::Create();
	std::unique_ptr<Window>pWindow = std::make_unique<Window>();
	pWindow->CreateWnd();

	// ウィンドウ表示
	ShowWindow(pWindow->GetHandle(), SW_SHOW);

	// 初期化
	DIRECTX().Initialize(pWindow->GetHandle());

	Sprite spr;
	spr.Load("Assets/Test.png");
	Sprite spr2;
	spr2.Load("Assets/Test.png");
	PipelineState pipeline;
	pipeline.Initialize("Assets/Hlsl/2D.hlsl", "VSMain", "PSMain");

	// メインループ
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// 仮想キーメッセージを文字メッセージに変換
			TranslateMessage(&msg);
			// 1つのウィンドウプロシージャ―にメッセージを送出する
			DispatchMessage(&msg);
		}
		else
		{
			// 更新

			//描画
			DIRECTX().Clear();

			spr.SetSize(Vector2(256, 256));
			spr2.SetSize(Vector2(64, 64));

			pipeline.Render();

			spr.SetColor(Vector4(0.f,0.f,1.f,1.f));
			spr.SetPosition(Vector2(256, 0));
			spr.Render();
			spr2.SetPosition(Vector2(500, 0));
			spr2.Render();

			DIRECTX().Flip();
		}
	}

	//解放
	DIRECTX().Release();

	return 0;
}
