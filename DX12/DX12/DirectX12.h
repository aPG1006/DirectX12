#pragma once
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "Singleton.h"

// DirectXのシステムクラス
class DirectX12 :public Singleton<DirectX12>
{
	friend Singleton<DirectX12>;
private:
	// コンストラクタ
	DirectX12();
	// デストラクタ
	~DirectX12() = default;
	// Direct3D初期化
	HRESULT InitializeDXGIDevice();
	// コマンドキュー生成
	HRESULT CreateCommandQueue();
	// スワップチェイン生成
	HRESULT CreateSwapChain(HWND hwnd);
	// ディスクリプタヒープ生成
	HRESULT CreateDescriptorHeap();
	// レンダーターゲットビュー生成
	HRESULT CreateRenderTargetView();
	// 深度バッファ生成
	HRESULT CreateDepthBuffer();
	// 深度ヒープ生成
	HRESULT CreateDepthHeap();
	// 深度ビュー生成
	void CreateDepthStencilView();
	// ビューポート生成
	void CreateViewPort();
	// シザーレクトの生成
	void CreateScissorRect();

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device>m_pDevice;
	// DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory6>m_pDxgiFactory;
	// スワップチェイン
	Microsoft::WRL::ComPtr<IDXGISwapChain4>m_pSwapChain;
	// コマンドアロケーター
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>m_pCommandAllocator;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>m_pCommandList;
	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>m_pCommandQueue;
	// レンダーターゲット用ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>m_pRenderTargetHeap;
	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence>m_pFence;
	// 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>m_pDepthBuffer;
	// 深度用ディスクリプターヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>m_pDephtHeap;
	// バックバッファ
	std::vector<ID3D12Resource*>m_pBackBuffers;
	// フェンス値
	UINT64 m_FenceVal;
	// シザー短形
	D3D12_RECT m_ScissorRect;
	// ビューポート
	D3D12_VIEWPORT m_ViewPort;
public:
	// 初期化
	void Initialize(HWND hwnd);
	// クリア
	void Clear();
	// フリップ
	void Flip();
	// コマンドキュー待ち
	void WaitForCommandQueue();
	// デバッグレイヤー有効化
	void EnableDebugLayer();

	// デバイスの取得
	ID3D12Device* GetDevice() const { return m_pDevice.Get(); };
	// コマンドリストの取得
	ID3D12GraphicsCommandList* GetCommandList() const { return m_pCommandList.Get(); };
};

#define DIRECTX() DirectX12::Instance()