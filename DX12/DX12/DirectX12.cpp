#include <string>
#include<d3dcompiler.h>
#include "DirectX12.h"
#include"Window.h"
#include "d3dx12.h"

// ライブラリのインクルード
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

// コンストラクタ
DirectX12::DirectX12():
    m_pDevice(nullptr),
    m_pDxgiFactory(nullptr),
    m_pSwapChain(nullptr),
    m_pCommandAllocator(nullptr),
    m_pCommandList(nullptr),
    m_pCommandQueue(nullptr),
    m_pRenderTargetHeap(nullptr),
    m_pFence(nullptr),
    m_pDepthBuffer(nullptr),
    m_pDephtHeap(nullptr),
    m_FenceVal(0) {}

// 初期化
void DirectX12::Initialize(HWND hwnd)
{
    // デバッグレイヤーオン
    EnableDebugLayer();

    HRESULT Result = InitializeDXGIDevice();
    Result = CreateCommandQueue();
    Result = CreateSwapChain(hwnd);
    Result = CreateDescriptorHeap();
    Result = CreateRenderTargetView();
    Result = m_pDevice->CreateFence(m_FenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
    Result = CreateDepthBuffer();
    Result = CreateDepthHeap();
    CreateDepthStencilView();
    CreateViewPort();
    CreateScissorRect();
}

// Direct3D初期化
HRESULT DirectX12::InitializeDXGIDevice()
{
    UINT FlagsDXGI = 0;

    FlagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;

    D3D_FEATURE_LEVEL FeatureLevel;

    auto Result = CreateDXGIFactory2(FlagsDXGI, IID_PPV_ARGS(&m_pDxgiFactory));

    //読み込み失敗
    if (FAILED(Result))
    {
        ::OutputDebugStringA("読み込み失敗");
        return Result;
    }

    // アダプターの列挙用
    std::vector<IDXGIAdapter*>Adapters;

    // アダプターオブジェクト挿入
    IDXGIAdapter* TmpAdapter = nullptr;

    for (int i = 0; m_pDxgiFactory->EnumAdapters(i, &TmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        Adapters.push_back(TmpAdapter);
    }

    // アダプターを識別するための情報をループで取得
    for (auto adapt : Adapters)
    {
        DXGI_ADAPTER_DESC AdapterDesc = {};
        //アダプターの説明オブジェクト取得
        adapt->GetDesc(&AdapterDesc);

        std::wstring StrDesc = AdapterDesc.Description;

        //探したいアダプターの名前を確認
        if (StrDesc.find(L"NVIDIA") != std::string::npos)
        {
            TmpAdapter = adapt;
            break;
        }
    }

    // デバイス生成可能なフューチャーレベル探査用配列
    D3D_FEATURE_LEVEL FeatureLevels[4] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    Result = S_FALSE;
    // Direct3Dの初期化
    for (auto level : FeatureLevels)
    {
        if (SUCCEEDED(D3D12CreateDevice(TmpAdapter, level, IID_PPV_ARGS(m_pDevice.ReleaseAndGetAddressOf()))))
        {
            //生成可能なバージョンが見つかったらループを抜ける
            FeatureLevel = level;
            Result = S_OK;
            break;
        }
    }

    return Result;
}

// コマンドキュー生成
HRESULT DirectX12::CreateCommandQueue()
{
    // コマンドアロケーターの生成
    HRESULT Result = m_pDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(m_pCommandAllocator.ReleaseAndGetAddressOf()));

    // コマンドリストの生成
    Result = m_pDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_pCommandAllocator.Get(),
        nullptr,
        IID_PPV_ARGS(&m_pCommandList));

    // コマンドキュー作成
    D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
    // タイムアウトなし
    CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    // アダプタを一つしか使わない場合は0
    CommandQueueDesc.NodeMask = 0;
    // プライオリティは特になし
    CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    // コマンドリストと合わせる
    CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    // キュー生成
    Result = m_pDevice->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&m_pCommandQueue));

    return Result;
}

// スワップチェイン生成
HRESULT DirectX12::CreateSwapChain(HWND hwnd)
{
    // スワップチェイン作成
    DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
    SwapChainDesc.Width  = WindowWidth;
    SwapChainDesc.Height = WindowHeight;
    SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.Stereo = false;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    SwapChainDesc.BufferCount = 2;
    // バックバッファーは伸び縮み可能
    SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    // フリップ後は速やかに破棄
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    // 指定なし
    SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    // ウィンドウ⇔フルスクリーン切り替え可能
    SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT Result = m_pDxgiFactory->CreateSwapChainForHwnd(
        m_pCommandQueue.Get(),
        hwnd,
        &SwapChainDesc,
        nullptr,
        nullptr,
        (IDXGISwapChain1**)m_pSwapChain.ReleaseAndGetAddressOf());

    return Result;
}

// ディスクリプタヒープ生成
HRESULT DirectX12::CreateDescriptorHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc = {};
    DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    // 複数のGPUがある場合に識別を行うビットフラグ
    DescriptorHeapDesc.NodeMask = 0;
    // 表裏の2つ
    DescriptorHeapDesc.NumDescriptors = 2;
    // 指定なし
    DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT Result = m_pDevice->CreateDescriptorHeap(
        &DescriptorHeapDesc,
        IID_PPV_ARGS(m_pRenderTargetHeap.ReleaseAndGetAddressOf()));

    return Result;
}

// レンダーターゲットビュー生成
HRESULT DirectX12::CreateRenderTargetView()
{

    DXGI_SWAP_CHAIN_DESC SwcDesc = {};
    auto  Result = m_pSwapChain->GetDesc(&SwcDesc);
    m_pBackBuffers.resize(SwcDesc.BufferCount);
    // 1番目以降のディスクリプタヒープを取得するために1つ分後ろにずらす
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = m_pRenderTargetHeap->GetCPUDescriptorHandleForHeapStart();

    // SRGBA用のレンダーターゲットビュー設定
    D3D12_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc = {};
    // 
    RenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

    for (size_t i = 0; i < SwcDesc.BufferCount; ++i)
    {
        // BackBuffersの中にスワップチェーン上のバックバッファを取得
        Result = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pBackBuffers[i]));
        RenderTargetViewDesc.Format = m_pBackBuffers[i]->GetDesc().Format;
        // レンダーターゲットビュー生成
        m_pDevice->CreateRenderTargetView(m_pBackBuffers[i], &RenderTargetViewDesc, Handle);
        // ディスクリプタ1つあたりのサイズを加算
        Handle.ptr += m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    return Result;
}

// 深度バッファ生成
HRESULT DirectX12::CreateDepthBuffer()
{
    // 深度バッファ作成
    D3D12_RESOURCE_DESC DepthResourceDesc = {};
    // 2次元テクスチャーデータ
    DepthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    // 幅と高さはレンダーターゲットと同じ
    DepthResourceDesc.Width  = WindowWidth;
    DepthResourceDesc.Height = WindowHeight;
    // テクスチャ配列でも3D配列でもない
    DepthResourceDesc.DepthOrArraySize = 1;
    // 深度値書き込み用フォーマット
    DepthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
    // サンプルは1ピクセル当たり1つ
    DepthResourceDesc.SampleDesc.Count = 1;
    // デプスステンシルとして使用
    DepthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    // 深度値用ヒーププロパティ
    D3D12_HEAP_PROPERTIES DepthHeapProperties = {};
    // DEFAULTなのであとはUNKNOWNでよい
    DepthHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    DepthHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    DepthHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    // クリアバリュー
    D3D12_CLEAR_VALUE DepthClearValue = {};
    // 深さ1.f(最大値)でクリア
    DepthClearValue.DepthStencil.Depth = 1.f;
    // 32ビットflaot値としてクリア
    DepthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

    // 深度バッファ
    return m_pDevice->CreateCommittedResource(
        &DepthHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &DepthResourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,// 深度値書き込み使用
        &DepthClearValue,
        IID_PPV_ARGS(&m_pDepthBuffer));
}

// 深度ヒープ生成
HRESULT DirectX12::CreateDepthHeap()
{
    // 深度値のためのディスクリプタヒープ作成
    D3D12_DESCRIPTOR_HEAP_DESC DepthHeapDesc = {};
    // 深度ビュー1つ 
    DepthHeapDesc.NumDescriptors = 1;
    // デプスステンシルビューとして使用
    DepthHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    // ヒープ生成
    HRESULT Result = m_pDevice->CreateDescriptorHeap(
        &DepthHeapDesc,
        IID_PPV_ARGS(m_pDephtHeap.ReleaseAndGetAddressOf()));

    // 深度ヒープの生成に失敗
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "深度ヒープの生成に失敗しました");
    }

    return Result;
}

// 深度ビューの生成
void DirectX12::CreateDepthStencilView()
{
    // 深度ビュー作成
    D3D12_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
    // 深度値に32ビット使用
    DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    // 2Dテクスチャ
    DepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    // フラグは特になし
    DepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

    m_pDevice->CreateDepthStencilView(
        m_pDepthBuffer.Get(),
        &DepthStencilViewDesc,
        m_pDephtHeap->GetCPUDescriptorHandleForHeapStart());
}

// ビューポート生成
void DirectX12::CreateViewPort()
{
    // 出力先の幅
    m_ViewPort.Width  = static_cast<FLOAT>(WindowWidth);
    // 出力先の高さ 
    m_ViewPort.Height = static_cast<FLOAT>(WindowHeight);
    // 出力先の左上座標X
    m_ViewPort.TopLeftX = 0.f;
    // 出力先の左上座標Y
    m_ViewPort.TopLeftY = 0.f;
    // 深度最大値
    m_ViewPort.MaxDepth = 1.f;
    // 深度最小値
    m_ViewPort.MinDepth = 0.f;
}

// シザー短形の生成
void DirectX12::CreateScissorRect()
{
    // 切り抜き上座標
    m_ScissorRect.top = 0;
    // 切り抜き左座標
    m_ScissorRect.left = 0;
    // 切り抜き上座標
    m_ScissorRect.right = m_ScissorRect.left + WindowWidth;
    // 切り抜き下座標
    m_ScissorRect.bottom = m_ScissorRect.top + WindowHeight;
}


// クリア
void DirectX12::Clear()
{
    // 現在のバックバッファを指定
    auto BackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    // リソースバリア
    CD3DX12_RESOURCE_BARRIER Barrier;
    auto Transition = Barrier.Transition(
        m_pBackBuffers[BackBufferIndex],
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_pCommandList->ResourceBarrier(1, &Transition);

    // レンダーターゲット指定
    auto RenderTargetHandle = m_pRenderTargetHeap->GetCPUDescriptorHandleForHeapStart();
    RenderTargetHandle.ptr += 
        static_cast<ULONG_PTR>(BackBufferIndex) * m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // 深度を指定
    auto DepthHeapHandle = m_pDephtHeap->GetCPUDescriptorHandleForHeapStart();
    m_pCommandList->OMSetRenderTargets(1, &RenderTargetHandle, true, &DepthHeapHandle);
    // 深度値のクリア
    m_pCommandList->ClearDepthStencilView(DepthHeapHandle, D3D12_CLEAR_FLAG_DEPTH, 1, 0, 0, nullptr);

    // 画面をクリア
    float ClearColor[] = { 0.5f,0.5f,0.5f,1.f, };
    m_pCommandList->ClearRenderTargetView(RenderTargetHandle, ClearColor, 0, nullptr);
    m_pCommandList->RSSetViewports(1, &m_ViewPort);
    m_pCommandList->RSSetScissorRects(1, &m_ScissorRect);
}

// フリップ
void DirectX12::Flip()
{
    // 現在のバックバッファを指定
    auto BackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    // リソースバリア
    CD3DX12_RESOURCE_BARRIER Barrier;
    auto Transition = Barrier.Transition(
        m_pBackBuffers[BackBufferIndex],
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);
    m_pCommandList->ResourceBarrier(1, &Transition);

    // 命令のクローズ
    m_pCommandList->Close();

    // コマンドリストの実行
    ComPtr<ID3D12CommandList>CommandLists(m_pCommandList.Get());
    m_pCommandQueue->ExecuteCommandLists(1, CommandLists.GetAddressOf());

    // フリップ
    m_pSwapChain->Present(0, 0);

    // 待ち
    WaitForCommandQueue();

    // キューのクリア
    m_pCommandAllocator->Reset();
    // 再びコマンドリストをためる準備
    m_pCommandList->Reset(m_pCommandAllocator.Get(), nullptr);
}

// コマンドキュー待ち
void DirectX12::WaitForCommandQueue()
{
    // 待ち
    m_pCommandQueue->Signal(m_pFence.Get(), ++m_FenceVal);

    if (m_pFence->GetCompletedValue() != m_FenceVal)
    {
        // イベントハンドルの取得
        auto Event = CreateEvent(nullptr, false, false, nullptr);
        m_pFence->SetEventOnCompletion(m_FenceVal, Event);
        // イベントが発生するまで待ち続ける
        WaitForSingleObjectEx(Event, INFINITE, false);
        // イベントハンドルを閉じる
        CloseHandle(Event);
    }

}

// デバッグレイヤー有効化
void DirectX12::EnableDebugLayer()
{
    ComPtr<ID3D12Debug>Debug = nullptr;

    auto Result = D3D12GetDebugInterface(IID_PPV_ARGS(&Debug));

    // デバッグレイヤーを有効化する
    Debug->EnableDebugLayer();
}




