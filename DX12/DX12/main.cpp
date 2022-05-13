#include <tchar.h>
#include <memory>
#include "Window.h"
#include "DirectX12.h"
#include "Sprite.h"
#include "PipelineState.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// ����
	DirectX12::Create();
	std::unique_ptr<Window>pWindow = std::make_unique<Window>();
	pWindow->CreateWnd();

	// �E�B���h�E�\��
	ShowWindow(pWindow->GetHandle(), SW_SHOW);

	// ������
	DIRECTX().Initialize(pWindow->GetHandle());

	Sprite spr;
	spr.Load("Assets/Test.png");
	Sprite spr2;
	spr2.Load("Assets/Test.png");
	PipelineState pipeline;
	pipeline.Initialize("Assets/Hlsl/2D.hlsl", "VSMain", "PSMain");

	// ���C�����[�v
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
			TranslateMessage(&msg);
			// 1�̃E�B���h�E�v���V�[�W���\�Ƀ��b�Z�[�W�𑗏o����
			DispatchMessage(&msg);
		}
		else
		{
			// �X�V

			//�`��
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

	//���
	DIRECTX().Release();

	return 0;
}
