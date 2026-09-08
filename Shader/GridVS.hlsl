cbuffer GridConstant : register(b1)
{
    row_major float4x4 mvp;
    row_major float4x4 World;
    float CellSize;
    float3 _Pad;
}

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float3 WorldPos : TEXCOORD0;
};

PS_INPUT MainVS(VS_INPUT Input)
{
    PS_INPUT Output;
    float4 Local = float4(Input.Position, 1.0f);

    Output.Position = mul(Local, mvp);
    Output.WorldPos = mul(Local, World).xyz; // ← .xyz
    return Output;
}