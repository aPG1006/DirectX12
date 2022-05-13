#include "PipelineState.h"
#include "d3dx12.h"
#include "DirectX12.h"

// �R���X�g���N�^
PipelineState::PipelineState() :
    pipelineState(nullptr),
    rootSignature(nullptr),
    shader(nullptr) {}

// ����
void PipelineState::Create()
{
    // ���̓��C�A�E�g�̐ݒ�
    PipelineStateDesc.InputLayout = _InputLayout::InputLayout;
    // ���[�g�V�O�l�`���̐ݒ�
    PipelineStateDesc.pRootSignature = rootSignature->Get();
    // ���_�V�F�[�_�[�ݒ�
    PipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(shader->GetVSBlob());
    // �s�N�Z���V�F�[�_�[�ݒ�
    PipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(shader->GetPSBlob());
    //
    PipelineStateDesc.SampleMask = UINT_MAX;
    // �O�p�`��`��
    PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    //�@�`��Ώۂ�1
    PipelineStateDesc.NumRenderTargets = 1;
    PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    PipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    // �T���v���[��1
    PipelineStateDesc.SampleDesc.Count = 1;
    PipelineStateDesc.SampleDesc.Quality = 0;

    HRESULT Result = DIRECTX().GetDevice()->CreateGraphicsPipelineState(
        &PipelineStateDesc,
        IID_PPV_ARGS(pipelineState.GetAddressOf()));

    // �p�C�v���C���X�e�[�g�̐����Ɏ��s���܂���
    if (FAILED(Result))
    {
        assert(SUCCEEDED(Result) && "�p�C�v���C���X�e�[�g�̐����Ɏ��s���܂���");
    }
}

// �u�����h�X�e�[�g
void PipelineState::BlendState()
{
    D3D12_RENDER_TARGET_BLEND_DESC m_desc = {};
    // ���Z�A��Z�A���Ȃǂ̃u�����h�����邩�ǂ���
    m_desc.BlendEnable = FALSE;
    // �_�����Z���邩�ǂ���
    m_desc.LogicOpEnable = FALSE;
    // �`�挳���W��
    m_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
    // �`��惿�W��
    m_desc.DestBlendAlpha = D3D12_BLEND_ONE;
    // �����ǂ��u�����h���邩
    m_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    // �ǂ��_�����Z���邩
    m_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
    // �������ގ��̃}�X�N�l�@
    m_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    // �ǂ��u�����h���邩
    m_desc.BlendOp = D3D12_BLEND_OP_ADD;
    // �`�挳�W��
    m_desc.SrcBlend = D3D12_BLEND_ONE;
    // �`���W��
    m_desc.DestBlend = D3D12_BLEND_ONE;

    // �u�����h�ݒ�
    PipelineStateDesc.BlendState.RenderTarget[0] = m_desc;
}

// ���X�^���C�U
void PipelineState::Rasterizer()
{
    D3D12_RASTERIZER_DESC m_desc = {};
    // �h��Ԃ�
    m_desc.FillMode = D3D12_FILL_MODE_SOLID;
    // �J�����O
    m_desc.CullMode = D3D12_CULL_MODE_NONE;
    // �O�p�`���O�ʂ��w�ʂ�������
    m_desc.FrontCounterClockwise = TRUE;
    // ����̃s�N�Z���ɒǉ����ꂽ�[�x�l
    m_desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    // ����̃s�N�Z���̌X�Ώ�̃X�J���[
    m_desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    // �[�x�����̃N���b�s���O�L��
    m_desc.DepthClipEnable = TRUE;
    // �A���`�G�C���A�X�g�p���Ȃ�
    m_desc.MultisampleEnable = FALSE;
    // ����A���`�G�C���A�X��L���ɂ��邩�ǂ������w��
    m_desc.AntialiasedLineEnable = FALSE;
    // UAV�̃����_�����O�܂��̓��X�^���C�Y���ɋ��������T���v����
    m_desc.ForcedSampleCount = 0;
    // ���X�^���C�Y���I���܂��̓I�t�ł��邩�ǂ���
    m_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // ���X�^���C�U�ݒ�
    PipelineStateDesc.RasterizerState = m_desc;
}

// �f�v�X�X�e���V��
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


    // �f�v�X�X�e���V���ݒ�
    PipelineStateDesc.DepthStencilState = m_desc;
}

// ������
void PipelineState::Initialize(const char* filename, LPCSTR vsname, LPCSTR psname)
{
    // ���[�g�V�O�l�`������
    rootSignature = std::make_unique<RootSignature>();
    rootSignature->Create();

    // �V�F�[�_�[����
    shader = std::make_unique<Shader>();
    shader->SetUpVS(filename, vsname);
    shader->SetUpPS(filename, psname);


    BlendState();
    Rasterizer();
    DepthStencil();
    // ����
    Create();
}

// �`��ݒ�
void PipelineState::Render()
{
    auto CommandList = DIRECTX().GetCommandList();
    CommandList->SetGraphicsRootSignature(rootSignature->Get());
    CommandList->SetPipelineState(pipelineState.Get());
}



