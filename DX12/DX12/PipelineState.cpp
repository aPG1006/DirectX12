#include "PipelineState.h"
#include "d3dx12.h"
#include "DirectX12.h"

// コンストラクタ
PipelineState::PipelineState() :
    pipelineState(nullptr),
    rootSignature(nullptr),
    shader(nullptr) {}

// 生成
void PipelineState::Create()
{
    // 入力レイアウトの設定
    PipelineStateDesc.InputLayout = _InputLayout::InputLayout;
    // ルートシグネチャの設定
    PipelineStateDesc.pRootSignature = rootSignature->Get();
    // 頂点シェーダー設定
    PipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(shader->GetVSBlob());
    // ピクセルシェーダー設定
    PipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(shader->GetPSBlob());
    //
    PipelineStateDesc.SampleMask = UINT_MAX;
    // 三角形を描画
    PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    //　描画対象は1
    PipelineStateDesc.NumRenderTargets = 1;
    PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    PipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    // サンプラーは1
    PipelineStateDesc.SampleDesc.Count = 1;
    PipelineStateDesc.SampleDesc.Quality = 0;

    HRESULT Result = DIRECTX().GetDevice()->CreateGraphicsPipelineState(
        &PipelineStateDesc,
        IID_PPV_ARGS(pipelineState.GetAddressOf()));

    // パイプラインステートの生成に失敗しました
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "パイプラインステートの生成に失敗しました");
    }
}

// ブレンドステート
void PipelineState::BlendState()
{
    D3D12_RENDER_TARGET_BLEND_DESC m_desc = {};
    // 加算、乗算、αなどのブレンドをするかどうか
    m_desc.BlendEnable = FALSE;
    // 論理演算するかどうか
    m_desc.LogicOpEnable = FALSE;
    // 描画元α係数
    m_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
    // 描画先α係数
    m_desc.DestBlendAlpha = D3D12_BLEND_ONE;
    // αをどうブレンドするか
    m_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    // どう論理演算するか
    m_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
    // 書き込む時のマスク値　
    m_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    // どうブレンドするか
    m_desc.BlendOp = D3D12_BLEND_OP_ADD;
    // 描画元係数
    m_desc.SrcBlend = D3D12_BLEND_ONE;
    // 描画先係数
    m_desc.DestBlend = D3D12_BLEND_ONE;

    // ブレンド設定
    PipelineStateDesc.BlendState.RenderTarget[0] = m_desc;
}

// ラスタライザ
void PipelineState::Rasterizer()
{
    D3D12_RASTERIZER_DESC m_desc = {};
    // 塗りつぶし
    m_desc.FillMode = D3D12_FILL_MODE_SOLID;
    // カリング
    m_desc.CullMode = D3D12_CULL_MODE_NONE;
    // 三角形が前面か背面かを決定
    m_desc.FrontCounterClockwise = TRUE;
    // 特定のピクセルに追加された深度値
    m_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    // 特定のピクセルの傾斜上のスカラー
    m_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    // 深度方向のクリッピング有効
    m_desc.DepthClipEnable = TRUE;
    // アンチエイリアス使用しない
    m_desc.MultisampleEnable = FALSE;
    // 回線アンチエイリアスを有効にするかどうかを指定
    m_desc.AntialiasedLineEnable = FALSE;
    // UAVのレンダリングまたはラスタライズ中に強制されるサンプル数
    m_desc.ForcedSampleCount = 0;
    // ラスタライズがオンまたはオフであるかどうか
    m_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // ラスタライザ設定
    PipelineStateDesc.RasterizerState = m_desc;
}

// デプスステンシル
void PipelineState::DepthStencil()
{
    D3D12_DEPTH_STENCIL_DESC m_desc = {};
    m_desc.DepthEnable = true;
    m_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    m_desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    m_desc.StencilEnable = FALSE;
    m_desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    m_desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
    {
        D3D12_STENCIL_OP_KEEP,
        D3D12_STENCIL_OP_KEEP,
        D3D12_STENCIL_OP_KEEP,
        D3D12_COMPARISON_FUNC_ALWAYS
    };
    m_desc.FrontFace = defaultStencilOp;
    m_desc.BackFace = defaultStencilOp;


    // デプスステンシル設定
    PipelineStateDesc.DepthStencilState = m_desc;
}

// 初期化
void PipelineState::Initialize(const char* filename, LPCSTR vsname, LPCSTR psname)
{
    // ルートシグネチャ生成
    rootSignature = std::make_unique<RootSignature>();
    rootSignature->Create();

    // シェーダー生成
    shader = std::make_unique<Shader>();
    shader->SetUpVS(filename, vsname);
    shader->SetUpPS(filename, psname);


    BlendState();
    Rasterizer();
    DepthStencil();
    // 生成
    Create();
}

// 描画設定
void PipelineState::Render()
{
    auto CommandList = DIRECTX().GetCommandList();
    CommandList->SetGraphicsRootSignature(rootSignature->Get());
    CommandList->SetPipelineState(pipelineState.Get());
}



