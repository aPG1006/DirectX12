#pragma once
#include<wrl.h>
#include<d3d12.h>
#include"RootSignature.h"
#include <cassert>

// ���[�g�V�O�l�`���N���X
class RootSignature
{
private:
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature>m_pRootSignature;
public:
	// �R���X�g���N�^
	RootSignature();
	// �f�X�g���N�^
	~RootSignature() = default;
	// ����
	void Create();

	// �擾
	ID3D12RootSignature* Get() const { return m_pRootSignature.Get(); };
};

