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

// コンストラクタ
Shader::Shader():
    m_pVSBlob(nullptr),
    m_pPSBlob(nullptr) {}

// シェーダー単体コンパイル
HRESULT Shader::Compile(const char* pFilename, LPCSTR method, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
{
    WCHAR Path[256];
    size_t Len = 0;
    mbstowcs_s(&Len, Path, 256, pFilename, _TRUNCATE);

    Microsoft::WRL::ComPtr<ID3DBlob>ErrorBlob = nullptr;

    HRESULT	Result = D3DCompileFromFile(
        Path,                                             // シェーダー名            
        nullptr,                                          // defineはなし
        D3D_COMPILE_STANDARD_FILE_INCLUDE,                // インクルードはデフォルト
        method,
        shaderModel,                                      // 対象シェーダーvs_5_0
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,  // デバッグ用および最適化なし
        0,
        ppBlobOut,
        ErrorBlob.ReleaseAndGetAddressOf());              // エラー時にはErrorBlobにメッセージが入る

    // コンパイルの読み込みに失敗しました
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "コンパイルの読み込みに失敗しました");
    }

    return S_OK;
}

// 頂点シェーダーの設定
void Shader::SetUpVS(const char* pFilename, LPCSTR vsname)
{
    HRESULT Result = Result = Compile(pFilename, vsname, "vs_5_0", m_pVSBlob.GetAddressOf());

    // 頂点シェーダーの読み込みに失敗
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "頂点シェーダーの読み込みに失敗しました");
    }
}

// ピクセルシェーダーの設定
void Shader::SetUpPS(const char* filename, LPCSTR psname)
{
    HRESULT Result = Result = Compile(filename, psname, "ps_5_0", m_pPSBlob.GetAddressOf());

    // ピクセルシェーダーの読み込みに失敗
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "ピクセルシェーダーの読み込みに失敗しました");
    }
}
