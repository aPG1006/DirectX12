#include "Shader.h"
#include <d3dcompiler.h>
#include "DirectX12.h"

#pragma comment(lib,"d3dcompiler.lib")

const D3D12_INPUT_ELEMENT_DESC _InputLayout::InputElements[] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};


const D3D12_INPUT_LAYOUT_DESC _InputLayout::InputLayout =
{
    _InputLayout::InputElements,
    _InputLayout::InputElementCount,
};

// �R���X�g���N�^
Shader::Shader():
    m_pVSBlob(nullptr),
    m_pPSBlob(nullptr) {}

// �V�F�[�_�[�P�̃R���p�C��
HRESULT Shader::Compile(const char* pFilename, LPCSTR method, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
{
    WCHAR Path[256];
    size_t Len = 0;
    mbstowcs_s(&Len, Path, 256, pFilename, _TRUNCATE);

    Microsoft::WRL::ComPtr<ID3DBlob>ErrorBlob = nullptr;

    HRESULT	Result = D3DCompileFromFile(
        Path,                                             // �V�F�[�_�[��            
        nullptr,                                          // define�͂Ȃ�
        D3D_COMPILE_STANDARD_FILE_INCLUDE,                // �C���N���[�h�̓f�t�H���g
        method,
        shaderModel,                                      // �ΏۃV�F�[�_�[vs_5_0
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,  // �f�o�b�O�p����эœK���Ȃ�
        0,
        ppBlobOut,
        ErrorBlob.ReleaseAndGetAddressOf());              // �G���[���ɂ�ErrorBlob�Ƀ��b�Z�[�W������

    // �R���p�C���̓ǂݍ��݂Ɏ��s���܂���
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "�R���p�C���̓ǂݍ��݂Ɏ��s���܂���");
    }

    return S_OK;
}

// ���_�V�F�[�_�[�̐ݒ�
void Shader::SetUpVS(const char* pFilename, LPCSTR vsname)
{
    HRESULT Result = Result = Compile(pFilename, vsname, "vs_5_0", m_pVSBlob.GetAddressOf());

    // ���_�V�F�[�_�[�̓ǂݍ��݂Ɏ��s
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "���_�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂���");
    }
}

// �s�N�Z���V�F�[�_�[�̐ݒ�
void Shader::SetUpPS(const char* filename, LPCSTR psname)
{
    HRESULT Result = Result = Compile(filename, psname, "ps_5_0", m_pPSBlob.GetAddressOf());

    // �s�N�Z���V�F�[�_�[�̓ǂݍ��݂Ɏ��s
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "�s�N�Z���V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂���");
    }
}
