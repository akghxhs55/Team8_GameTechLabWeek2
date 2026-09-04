struct PS_INPUT
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

PS_INPUT MainVS(uint VertexId : SV_VertexID)
{
    static const float2 Positions[3] =
    {
        float2(-0.6f, -0.6f),
        float2(0.0f, 0.6f),
        float2(0.6f, -0.6f),
    };

    static const float4 Colors[3] =
    {
        float4(1.0f, 0.1f, 0.1f, 1.0f),
        float4(0.1f, 1.0f, 0.1f, 1.0f),
        float4(0.1f, 0.3f, 1.0f, 1.0f),
    };

    PS_INPUT output;
    output.Position = float4(Positions[VertexId], 0.0f, 1.0f);
    output.Color = Colors[VertexId];

    return output;
}