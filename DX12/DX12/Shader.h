#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cassert>

// ���̓��C�A�E�g�\����
struct _InputLayout
{
private:
	static const int InputElementCount = 3;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
public:
	static const D3D12_INPUT_LAYOUT_DESC InputLayout;
};

// �V�F�[�_�[�N���X
class Shader
{
private:
	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3DBlob>m_pVSBlob;
	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3DBlob>m_pPSBlob;

	// �V�F�[�_�[�P�̃R���p�C��
	HRESULT Compile(const char* filename, LPCSTR method, LPCSTR shaderModel, ID3DBlob** ppBlobOut);
public:
	// �R���X�g���N�^
	Shader();
	// �f�X�g���N�^
	~Shader() = default;

	// ���_�V�F�[�_�[�̎擾
	ID3DBlob* GetVSBlob() const { return m_pVSBlob.Get(); }
	// �s�N�Z���V�F�[�_�[�̎擾
	ID3DBlob* GetPSBlob() const { return m_pPSBlob.Get(); }

	// ���_�V�F�[�_�[�̐ݒ�
	void SetUpVS(const char* pFilename, LPCSTR vsname);
	// �s�N�Z���V�F�[�_�[�̐ݒ�
	void SetUpPS(const char* pFilename, LPCSTR psname);
};

