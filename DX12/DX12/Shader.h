#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <cassert>

// 入力レイアウト構造体
struct _InputLayout
{
private:
	static const int InputElementCount = 3;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
public:
	static const D3D12_INPUT_LAYOUT_DESC InputLayout;
};

// シェーダークラス
class Shader
{
private:
	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3DBlob>m_pVSBlob;
	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3DBlob>m_pPSBlob;

	// シェーダー単体コンパイル
	HRESULT Compile(const char* filename, LPCSTR method, LPCSTR shaderModel, ID3DBlob** ppBlobOut);
public:
	// コンストラクタ
	Shader();
	// デストラクタ
	~Shader() = default;

	// 頂点シェーダーの取得
	ID3DBlob* GetVSBlob() const { return m_pVSBlob.Get(); }
	// ピクセルシェーダーの取得
	ID3DBlob* GetPSBlob() const { return m_pPSBlob.Get(); }

	// 頂点シェーダーの設定
	void SetUpVS(const char* pFilename, LPCSTR vsname);
	// ピクセルシェーダーの設定
	void SetUpPS(const char* pFilename, LPCSTR psname);
};

