#include "Constants.hlsli"

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

PS_INPUT MainVS(VS_INPUT Input)
{
    PS_INPUT Output;

    Output.Position = float4(Input.Position, 1.0f);
    //Output.Position = mul(Output.Position, Model);
    //Output.Position = mul(Output.Position, ViewProjection);

    Output.Color = float4(Input.Color, 1.0f);

    return Output;
}