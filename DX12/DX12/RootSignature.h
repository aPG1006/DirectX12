#pragma once
#include<wrl.h>
#include<d3d12.h>
#include"RootSignature.h"
#include <cassert>

// ルートシグネチャクラス
class RootSignature
{
private:
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature>m_pRootSignature;
public:
	// コンストラクタ
	RootSignature();
	// デストラクタ
	~RootSignature() = default;
	// 生成
	void Create();

	// 取得
	ID3D12RootSignature* Get() const { return m_pRootSignature.Get(); };
};

