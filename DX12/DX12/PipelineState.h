#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>
#include "RootSignature.h"
#include "Shader.h"

// 描画設定クラス
class PipelineState
{
private:
	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
	// ルートシグネチャ
	std::unique_ptr<RootSignature>rootSignature;
	// シェーダー
	std::unique_ptr<Shader>shader;
	// パイプラインステートの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};

	// 生成
	void Create();
	// ブレンドステート
	void BlendState();
	// ラスタライザ
	void Rasterizer();
	// デプスステンシル
	void DepthStencil();
public:
	// コンストラクタ
	PipelineState();
	// デストラクタ
	~PipelineState() = default;
	// 初期化
	void Initialize(const char*filename, LPCSTR vsname, LPCSTR psname);
	// 描画
	void Render();

	// 取得
	ID3D12PipelineState* Get() { return pipelineState.Get(); }
};

