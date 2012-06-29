#include "input.fxh"

// Transformations
cbuffer cWorldViewProj : register( b0 )
{
    float4x4 g_mShadowModelViewProj;
}

struct VS_OUTPUT {
    float4 Position  : SV_Position;
};

VS_OUTPUT MainVS(VS_INPUT IN)
{
    VS_OUTPUT OUT;

    OUT.Position = mul( IN.Pos, g_mShadowModelViewProj );

    return OUT;
}

VS_OUTPUT MainVS_TEX(VS_INPUT_TEX IN)
{
    VS_OUTPUT OUT;

    OUT.Position = mul( IN.Pos, g_mShadowModelViewProj );

    return OUT;
}

//-----------------------------------------------------------------------------

float4 WhitePS(VS_OUTPUT IN) : SV_Target
{
    return float4( 0.0, 1.0, 1.0, 1.0 );
}