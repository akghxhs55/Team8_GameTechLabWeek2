cbuffer FrameConstants : register(b0)
{
    // row_major float4x4 ViewProjection;
}

cbuffer ObjectConstants : register(b1)
{
    row_major float4x4 MVP;
    // row_major float4x4 Model;
    float3 Color;

    float Padding;
}
