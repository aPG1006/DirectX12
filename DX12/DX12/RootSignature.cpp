#include <d3dcompiler.h>
#include "d3dx12.h"
#include "RootSignature.h"
#include "DirectX12.h"

#pragma comment(lib,"d3dcompiler.lib")

// �R���X�g���N�^�@
RootSignature::RootSignature() :
	m_pRootSignature(nullptr) {}

// ����
void RootSignature::Create()
{
	// �A�v���P�[�V�����̓��̓A�Z���u�����g�p
	auto Flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// �h���C���V�F�[�_�[�̃��[�g�V�O�l�`���ւ̃A�N�Z�X������
	Flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	// �n���V�F�[�_�[�̃��[�g�V�O�l�`���ւ̃A�N�Z�X������
	Flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	// �W�I���g���V�F�[�_�[�̃��[�g�V�O�l�`���ւ̃A�N�Z�X������
	Flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;


	CD3DX12_ROOT_PARAMETER RootParam[2] = {};
	// b0�̒萔�o�b�t�@�ݒ�A�S�ẴV�F�[�_�[���猩���悤�ɂ���
	RootParam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �f�B�X�N���v�^�e�[�u��
	CD3DX12_DESCRIPTOR_RANGE DescriptorRange[1] = {};
	// �V�F�[�_�[���\�[�X�r���[
	DescriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	RootParam[1].InitAsDescriptorTable(std::size(DescriptorRange), DescriptorRange, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	auto Sampler = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
	// ���[�g�p�����[�^�[�̌����ݒ�
	RootSignatureDesc.NumParameters = std::size(RootParam);
	// �T���v���[�̌����ݒ�
	RootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�p�����[�^�[�̃|�C���^�ݒ�
	RootSignatureDesc.pParameters = RootParam;
	// �T���v���[�̃|�C���^�ݒ�
	RootSignatureDesc.pStaticSamplers = &Sampler;
	// �t���O�̐ݒ�
	RootSignatureDesc.Flags = Flag;

	Microsoft::WRL::ComPtr<ID3DBlob>Blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob>ErrorBlob = nullptr;

	// �V���A���C�Y
	HRESULT Result = D3D12SerializeRootSignature(
		&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		Blob.GetAddressOf(),
		ErrorBlob.GetAddressOf());

	// ���[�g�V�O�l�`���V���A���C�Y�Ɏ��s
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "���[�g�V�O�l�`���V���A���C�Y�Ɏ��s���܂���");
	}

	// ���[�g�V�O�l�`������
	Result = DIRECTX().GetDevice()->CreateRootSignature(
		0,
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		IID_PPV_ARGS(m_pRootSignature.GetAddressOf()));

	// ���[�g�V�O�l�`���̐����Ɏ��s
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "���[�g�V�O�l�`���̐����Ɏ��s���܂���");
	}
}

