// �ʏ�e�N�X�`��(0��)
Texture2D DiffuseTexture : register(t0);

// �T���v���[(0��)
SamplerState DecalSampler : register(s0);

// ���_�V�F�[�_�[�ւ̓���
struct VSInput
{
    float3 pos : POSITION; // ���_���W
    float2 uv : TEXCOORD; // UV
    float4 color : COLOR; // ���_�F
};

// ���_�V�F�[�_�[����̏o��
struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float4 screen : TEXCOORD2;
};



// ���_�V�F�[�_�[
VSOutput VSMain(VSInput input)
{
    // �o�͍\����
    VSOutput output = (VSOutput) 0;

    // �s�N�Z���V�F�[�_�[�ɏ��n��
    output.svpos  = float4(input.pos, 1.f);
    output.color  = input.color;
    output.uv     = input.uv;
   
    return output;
}

// �s�N�Z���V�F�[�_�[
float4 PSMain(VSOutput input) : SV_TARGET
{
    return input.color;
}

