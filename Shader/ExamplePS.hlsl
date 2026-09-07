#include "Constants.hlsli"

struct PS_INPUT
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

float4 MainPS(PS_INPUT Input) : SV_Target
{
	if (length(Input.Color) > 0.0001f)
	{
        return Input.Color;
    }
    else
    {
        return float4(Color, 1.0f);
    }
}
