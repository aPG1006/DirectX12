#include <d3dcompiler.h>
#include "d3dx12.h"
#include "RootSignature.h"
#include "DirectX12.h"

#pragma comment(lib,"d3dcompiler.lib")

// コンストラクタ　
RootSignature::RootSignature() :
	m_pRootSignature(nullptr) {}

// 生成
void RootSignature::Create()
{
	// アプリケーションの入力アセンブラを使用
	auto Flag = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// ドメインシェーダーのルートシグネチャへのアクセスを拒否
	Flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	// ハルシェーダーのルートシグネチャへのアクセスを拒否
	Flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	// ジオメトリシェーダーのルートシグネチャへのアクセスを拒否
	Flag |= D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;


	CD3DX12_ROOT_PARAMETER RootParam[2] = {};
	// b0の定数バッファ設定、全てのシェーダーから見れるようにする
	RootParam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

	// ディスクリプタテーブル
	CD3DX12_DESCRIPTOR_RANGE DescriptorRange[1] = {};
	// シェーダーリソースビュー
	DescriptorRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	RootParam[1].InitAsDescriptorTable(std::size(DescriptorRange), DescriptorRange, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	auto Sampler = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
	// ルートパラメーターの個数分設定
	RootSignatureDesc.NumParameters = std::size(RootParam);
	// サンプラーの個数分設定
	RootSignatureDesc.NumStaticSamplers = 1;
	// ルートパラメーターのポインタ設定
	RootSignatureDesc.pParameters = RootParam;
	// サンプラーのポインタ設定
	RootSignatureDesc.pStaticSamplers = &Sampler;
	// フラグの設定
	RootSignatureDesc.Flags = Flag;

	Microsoft::WRL::ComPtr<ID3DBlob>Blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob>ErrorBlob = nullptr;

	// シリアライズ
	HRESULT Result = D3D12SerializeRootSignature(
		&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		Blob.GetAddressOf(),
		ErrorBlob.GetAddressOf());

	// ルートシグネチャシリアライズに失敗
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "ルートシグネチャシリアライズに失敗しました");
	}

	// ルートシグネチャ生成
	Result = DIRECTX().GetDevice()->CreateRootSignature(
		0,
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		IID_PPV_ARGS(m_pRootSignature.GetAddressOf()));

	// ルートシグネチャの生成に失敗
	if (FAILED(Result))
	{
		assert(SUCCEEDED(Result) && "ルートシグネチャの生成に失敗しました");
	}
}

