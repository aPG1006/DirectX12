#pragma once
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "Singleton.h"

// DirectX�̃V�X�e���N���X
class DirectX12 :public Singleton<DirectX12>
{
	friend Singleton<DirectX12>;
private:
	// �R���X�g���N�^
	DirectX12();
	// �f�X�g���N�^
	~DirectX12() = default;
	// Direct3D������
	HRESULT InitializeDXGIDevice();
	// �R�}���h�L���[����
	HRESULT CreateCommandQueue();
	// �X���b�v�`�F�C������
	HRESULT CreateSwapChain(HWND hwnd);
	// �f�B�X�N���v�^�q�[�v����
	HRESULT CreateDescriptorHeap();
	// �����_�[�^�[�Q�b�g�r���[����
	HRESULT CreateRenderTargetView();
	// �[�x�o�b�t�@����
	HRESULT CreateDepthBuffer();
	// �[�x�q�[�v����
	HRESULT CreateDepthHeap();
	// �[�x�r���[����
	void CreateDepthStencilView();
	// �r���[�|�[�g����
	void CreateViewPort();
	// �V�U�[���N�g�̐���
	void CreateScissorRect();

	// �f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device>m_pDevice;
	// DXGI�t�@�N�g��
	Microsoft::WRL::ComPtr<IDXGIFactory6>m_pDxgiFactory;
	// �X���b�v�`�F�C��
	Microsoft::WRL::ComPtr<IDXGISwapChain4>m_pSwapChain;
	// �R�}���h�A���P�[�^�[
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>m_pCommandAllocator;
	// �R�}���h���X�g
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>m_pCommandList;
	// �R�}���h�L���[
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>m_pCommandQueue;
	// �����_�[�^�[�Q�b�g�p�f�B�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>m_pRenderTargetHeap;
	// �t�F���X
	Microsoft::WRL::ComPtr<ID3D12Fence>m_pFence;
	// �[�x�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>m_pDepthBuffer;
	// �[�x�p�f�B�X�N���v�^�[�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>m_pDephtHeap;
	// �o�b�N�o�b�t�@
	std::vector<ID3D12Resource*>m_pBackBuffers;
	// �t�F���X�l
	UINT64 m_FenceVal;
	// �V�U�[�Z�`
	D3D12_RECT m_ScissorRect;
	// �r���[�|�[�g
	D3D12_VIEWPORT m_ViewPort;
public:
	// ������
	void Initialize(HWND hwnd);
	// �N���A
	void Clear();
	// �t���b�v
	void Flip();
	// �R�}���h�L���[�҂�
	void WaitForCommandQueue();
	// �f�o�b�O���C���[�L����
	void EnableDebugLayer();

	// �f�o�C�X�̎擾
	ID3D12Device* GetDevice() const { return m_pDevice.Get(); };
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* GetCommandList() const { return m_pCommandList.Get(); };
};

#define DIRECTX() DirectX12::Instance()