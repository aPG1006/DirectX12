#include <string>
#include<d3dcompiler.h>
#include "DirectX12.h"
#include"Window.h"
#include "d3dx12.h"

// ���C�u�����̃C���N���[�h
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

// �R���X�g���N�^
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

// ������
void DirectX12::Initialize(HWND hwnd)
{
    // �f�o�b�O���C���[�I��
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

// Direct3D������
HRESULT DirectX12::InitializeDXGIDevice()
{
    UINT FlagsDXGI = 0;

    FlagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;

    D3D_FEATURE_LEVEL FeatureLevel;

    auto Result = CreateDXGIFactory2(FlagsDXGI, IID_PPV_ARGS(&m_pDxgiFactory));

    //�ǂݍ��ݎ��s
    if (FAILED(Result))
    {
        ::OutputDebugStringA("�ǂݍ��ݎ��s");
        return Result;
    }

    // �A�_�v�^�[�̗񋓗p
    std::vector<IDXGIAdapter*>Adapters;

    // �A�_�v�^�[�I�u�W�F�N�g�}��
    IDXGIAdapter* TmpAdapter = nullptr;

    for (int i = 0; m_pDxgiFactory->EnumAdapters(i, &TmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        Adapters.push_back(TmpAdapter);
    }

    // �A�_�v�^�[�����ʂ��邽�߂̏������[�v�Ŏ擾
    for (auto adapt : Adapters)
    {
        DXGI_ADAPTER_DESC AdapterDesc = {};
        //�A�_�v�^�[�̐����I�u�W�F�N�g�擾
        adapt->GetDesc(&AdapterDesc);

        std::wstring StrDesc = AdapterDesc.Description;

        //�T�������A�_�v�^�[�̖��O���m�F
        if (StrDesc.find(L"NVIDIA") != std::string::npos)
        {
            TmpAdapter = adapt;
            break;
        }
    }

    // �f�o�C�X�����\�ȃt���[�`���[���x���T���p�z��
    D3D_FEATURE_LEVEL FeatureLevels[4] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    Result = S_FALSE;
    // Direct3D�̏�����
    for (auto level : FeatureLevels)
    {
        if (SUCCEEDED(D3D12CreateDevice(TmpAdapter, level, IID_PPV_ARGS(m_pDevice.ReleaseAndGetAddressOf()))))
        {
            //�����\�ȃo�[�W���������������烋�[�v�𔲂���
            FeatureLevel = level;
            Result = S_OK;
            break;
        }
    }

    return Result;
}

// �R�}���h�L���[����
HRESULT DirectX12::CreateCommandQueue()
{
    // �R�}���h�A���P�[�^�[�̐���
    HRESULT Result = m_pDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(m_pCommandAllocator.ReleaseAndGetAddressOf()));

    // �R�}���h���X�g�̐���
    Result = m_pDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_pCommandAllocator.Get(),
        nullptr,
        IID_PPV_ARGS(&m_pCommandList));

    // �R�}���h�L���[�쐬
    D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
    // �^�C���A�E�g�Ȃ�
    CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    // �A�_�v�^��������g��Ȃ��ꍇ��0
    CommandQueueDesc.NodeMask = 0;
    // �v���C�I���e�B�͓��ɂȂ�
    CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    // �R�}���h���X�g�ƍ��킹��
    CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    // �L���[����
    Result = m_pDevice->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&m_pCommandQueue));

    return Result;
}

// �X���b�v�`�F�C������
HRESULT DirectX12::CreateSwapChain(HWND hwnd)
{
    // �X���b�v�`�F�C���쐬
    DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
    SwapChainDesc.Width  = WindowWidth;
    SwapChainDesc.Height = WindowHeight;
    SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.Stereo = false;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    SwapChainDesc.BufferCount = 2;
    // �o�b�N�o�b�t�@�[�͐L�яk�݉\
    SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    // �t���b�v��͑��₩�ɔj��
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    // �w��Ȃ�
    SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    // �E�B���h�E�̃t���X�N���[���؂�ւ��\
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

// �f�B�X�N���v�^�q�[�v����
HRESULT DirectX12::CreateDescriptorHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc = {};
    DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    // ������GPU������ꍇ�Ɏ��ʂ��s���r�b�g�t���O
    DescriptorHeapDesc.NodeMask = 0;
    // �\����2��
    DescriptorHeapDesc.NumDescriptors = 2;
    // �w��Ȃ�
    DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT Result = m_pDevice->CreateDescriptorHeap(
        &DescriptorHeapDesc,
        IID_PPV_ARGS(m_pRenderTargetHeap.ReleaseAndGetAddressOf()));

    return Result;
}

// �����_�[�^�[�Q�b�g�r���[����
HRESULT DirectX12::CreateRenderTargetView()
{

    DXGI_SWAP_CHAIN_DESC SwcDesc = {};
    auto  Result = m_pSwapChain->GetDesc(&SwcDesc);
    m_pBackBuffers.resize(SwcDesc.BufferCount);
    // 1�Ԗڈȍ~�̃f�B�X�N���v�^�q�[�v���擾���邽�߂�1�����ɂ��炷
    D3D12_CPU_DESCRIPTOR_HANDLE Handle = m_pRenderTargetHeap->GetCPUDescriptorHandleForHeapStart();

    // SRGBA�p�̃����_�[�^�[�Q�b�g�r���[�ݒ�
    D3D12_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc = {};
    // 
    RenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    RenderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

    for (size_t i = 0; i < SwcDesc.BufferCount; ++i)
    {
        // BackBuffers�̒��ɃX���b�v�`�F�[����̃o�b�N�o�b�t�@���擾
        Result = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pBackBuffers[i]));
        RenderTargetViewDesc.Format = m_pBackBuffers[i]->GetDesc().Format;
        // �����_�[�^�[�Q�b�g�r���[����
        m_pDevice->CreateRenderTargetView(m_pBackBuffers[i], &RenderTargetViewDesc, Handle);
        // �f�B�X�N���v�^1������̃T�C�Y�����Z
        Handle.ptr += m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    return Result;
}

// �[�x�o�b�t�@����
HRESULT DirectX12::CreateDepthBuffer()
{
    // �[�x�o�b�t�@�쐬
    D3D12_RESOURCE_DESC DepthResourceDesc = {};
    // 2�����e�N�X�`���[�f�[�^
    DepthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    // ���ƍ����̓����_�[�^�[�Q�b�g�Ɠ���
    DepthResourceDesc.Width  = WindowWidth;
    DepthResourceDesc.Height = WindowHeight;
    // �e�N�X�`���z��ł�3D�z��ł��Ȃ�
    DepthResourceDesc.DepthOrArraySize = 1;
    // �[�x�l�������ݗp�t�H�[�}�b�g
    DepthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
    // �T���v����1�s�N�Z��������1��
    DepthResourceDesc.SampleDesc.Count = 1;
    // �f�v�X�X�e���V���Ƃ��Ďg�p
    DepthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    // �[�x�l�p�q�[�v�v���p�e�B
    D3D12_HEAP_PROPERTIES DepthHeapProperties = {};
    // DEFAULT�Ȃ̂ł��Ƃ�UNKNOWN�ł悢
    DepthHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    DepthHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    DepthHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    // �N���A�o�����[
    D3D12_CLEAR_VALUE DepthClearValue = {};
    // �[��1.f(�ő�l)�ŃN���A
    DepthClearValue.DepthStencil.Depth = 1.f;
    // 32�r�b�gflaot�l�Ƃ��ăN���A
    DepthClearValue.Format = DXGI_FORMAT_D32_FLOAT;

    // �[�x�o�b�t�@
    return m_pDevice->CreateCommittedResource(
        &DepthHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &DepthResourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,// �[�x�l�������ݎg�p
        &DepthClearValue,
        IID_PPV_ARGS(&m_pDepthBuffer));
}

// �[�x�q�[�v����
HRESULT DirectX12::CreateDepthHeap()
{
    // �[�x�l�̂��߂̃f�B�X�N���v�^�q�[�v�쐬
    D3D12_DESCRIPTOR_HEAP_DESC DepthHeapDesc = {};
    // �[�x�r���[1�� 
    DepthHeapDesc.NumDescriptors = 1;
    // �f�v�X�X�e���V���r���[�Ƃ��Ďg�p
    DepthHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    // �q�[�v����
    HRESULT Result = m_pDevice->CreateDescriptorHeap(
        &DepthHeapDesc,
        IID_PPV_ARGS(m_pDephtHeap.ReleaseAndGetAddressOf()));

    // �[�x�q�[�v�̐����Ɏ��s
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "�[�x�q�[�v�̐����Ɏ��s���܂���");
    }

    return Result;
}

// �[�x�r���[�̐���
void DirectX12::CreateDepthStencilView()
{
    // �[�x�r���[�쐬
    D3D12_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc = {};
    // �[�x�l��32�r�b�g�g�p
    DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    // 2D�e�N�X�`��
    DepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    // �t���O�͓��ɂȂ�
    DepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;

    m_pDevice->CreateDepthStencilView(
        m_pDepthBuffer.Get(),
        &DepthStencilViewDesc,
        m_pDephtHeap->GetCPUDescriptorHandleForHeapStart());
}

// �r���[�|�[�g����
void DirectX12::CreateViewPort()
{
    // �o�͐�̕�
    m_ViewPort.Width  = static_cast<FLOAT>(WindowWidth);
    // �o�͐�̍��� 
    m_ViewPort.Height = static_cast<FLOAT>(WindowHeight);
    // �o�͐�̍�����WX
    m_ViewPort.TopLeftX = 0.f;
    // �o�͐�̍�����WY
    m_ViewPort.TopLeftY = 0.f;
    // �[�x�ő�l
    m_ViewPort.MaxDepth = 1.f;
    // �[�x�ŏ��l
    m_ViewPort.MinDepth = 0.f;
}

// �V�U�[�Z�`�̐���
void DirectX12::CreateScissorRect()
{
    // �؂蔲������W
    m_ScissorRect.top = 0;
    // �؂蔲�������W
    m_ScissorRect.left = 0;
    // �؂蔲������W
    m_ScissorRect.right = m_ScissorRect.left + WindowWidth;
    // �؂蔲�������W
    m_ScissorRect.bottom = m_ScissorRect.top + WindowHeight;
}


// �N���A
void DirectX12::Clear()
{
    // ���݂̃o�b�N�o�b�t�@���w��
    auto BackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    // ���\�[�X�o���A
    CD3DX12_RESOURCE_BARRIER Barrier;
    auto Transition = Barrier.Transition(
        m_pBackBuffers[BackBufferIndex],
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_pCommandList->ResourceBarrier(1, &Transition);

    // �����_�[�^�[�Q�b�g�w��
    auto RenderTargetHandle = m_pRenderTargetHeap->GetCPUDescriptorHandleForHeapStart();
    RenderTargetHandle.ptr += 
        static_cast<ULONG_PTR>(BackBufferIndex) * m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // �[�x���w��
    auto DepthHeapHandle = m_pDephtHeap->GetCPUDescriptorHandleForHeapStart();
    m_pCommandList->OMSetRenderTargets(1, &RenderTargetHandle, true, &DepthHeapHandle);
    // �[�x�l�̃N���A
    m_pCommandList->ClearDepthStencilView(DepthHeapHandle, D3D12_CLEAR_FLAG_DEPTH, 1, 0, 0, nullptr);

    // ��ʂ��N���A
    float ClearColor[] = { 0.5f,0.5f,0.5f,1.f, };
    m_pCommandList->ClearRenderTargetView(RenderTargetHandle, ClearColor, 0, nullptr);
    m_pCommandList->RSSetViewports(1, &m_ViewPort);
    m_pCommandList->RSSetScissorRects(1, &m_ScissorRect);
}

// �t���b�v
void DirectX12::Flip()
{
    // ���݂̃o�b�N�o�b�t�@���w��
    auto BackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    // ���\�[�X�o���A
    CD3DX12_RESOURCE_BARRIER Barrier;
    auto Transition = Barrier.Transition(
        m_pBackBuffers[BackBufferIndex],
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);
    m_pCommandList->ResourceBarrier(1, &Transition);

    // ���߂̃N���[�Y
    m_pCommandList->Close();

    // �R�}���h���X�g�̎��s
    ComPtr<ID3D12CommandList>CommandLists(m_pCommandList.Get());
    m_pCommandQueue->ExecuteCommandLists(1, CommandLists.GetAddressOf());

    // �t���b�v
    m_pSwapChain->Present(0, 0);

    // �҂�
    WaitForCommandQueue();

    // �L���[�̃N���A
    m_pCommandAllocator->Reset();
    // �ĂуR�}���h���X�g�����߂鏀��
    m_pCommandList->Reset(m_pCommandAllocator.Get(), nullptr);
}

// �R�}���h�L���[�҂�
void DirectX12::WaitForCommandQueue()
{
    // �҂�
    m_pCommandQueue->Signal(m_pFence.Get(), ++m_FenceVal);

    if (m_pFence->GetCompletedValue() != m_FenceVal)
    {
        // �C�x���g�n���h���̎擾
        auto Event = CreateEvent(nullptr, false, false, nullptr);
        m_pFence->SetEventOnCompletion(m_FenceVal, Event);
        // �C�x���g����������܂ő҂�������
        WaitForSingleObjectEx(Event, INFINITE, false);
        // �C�x���g�n���h�������
        CloseHandle(Event);
    }

}

// �f�o�b�O���C���[�L����
void DirectX12::EnableDebugLayer()
{
    ComPtr<ID3D12Debug>Debug = nullptr;

    auto Result = D3D12GetDebugInterface(IID_PPV_ARGS(&Debug));

    // �f�o�b�O���C���[��L��������
    Debug->EnableDebugLayer();
}




