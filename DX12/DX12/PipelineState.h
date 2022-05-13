#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>
#include "RootSignature.h"
#include "Shader.h"

// �`��ݒ�N���X
class PipelineState
{
private:
	// �p�C�v���C���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	// ���[�g�V�O�l�`��
	std::unique_ptr<RootSignature>rootSignature;
	// �V�F�[�_�[
	std::unique_ptr<Shader>shader;
	// �p�C�v���C���X�e�[�g�̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};

	// ����
	void Create();
	// �u�����h�X�e�[�g
	void BlendState();
	// ���X�^���C�U
	void Rasterizer();
	// �f�v�X�X�e���V��
	void DepthStencil();
public:
	// �R���X�g���N�^
	PipelineState();
	// �f�X�g���N�^
	~PipelineState() = default;
	// ������
	void Initialize(const char*filename, LPCSTR vsname, LPCSTR psname);
	// �`��
	void Render();

	// �擾
	ID3D12PipelineState* Get() { return pipelineState.Get(); }
};

