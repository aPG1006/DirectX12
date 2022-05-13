// 通常テクスチャ(0番)
Texture2D DiffuseTexture : register(t0);

// サンプラー(0番)
SamplerState DecalSampler : register(s0);

// 頂点シェーダーへの入力
struct VSInput
{
    float3 pos : POSITION; // 頂点座標
    float2 uv : TEXCOORD; // UV
    float4 color : COLOR; // 頂点色
};

// 頂点シェーダーからの出力
struct VSOutput
{
    float4 svpos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float4 screen : TEXCOORD2;
};



// 頂点シェーダー
VSOutput VSMain(VSInput input)
{
    // 出力構造体
    VSOutput output = (VSOutput) 0;

    // ピクセルシェーダーに情報渡す
    output.svpos  = float4(input.pos, 1.f);
    output.color  = input.color;
    output.uv     = input.uv;
   
    return output;
}

// ピクセルシェーダー
float4 PSMain(VSOutput input) : SV_TARGET
{
    return input.color;
}

