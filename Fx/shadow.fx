#include "input.fxh"

float4x4 g_mShadowModelViewProj;

struct VS_OUTPUT {
    float4 Position  : POSITION;
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

float4 WhitePS(VS_OUTPUT IN) : COLOR 
{
    return float4( 0.0, 1.0, 1.0, 1.0 );
}

//-----------------------------------------------------------------------------

technique GenHardwareShadowMap
{
    pass P0
    {
        VertexShader = compile vs_3_0 MainVS();
        PixelShader = compile ps_3_0 WhitePS();

        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        CullMode         = None;  // note: not quite optimal
        
        ColorWriteEnable = 0;     // no need to render to color, we only need z
    }
}

technique GenHardwareShadowMapTex
{
    pass P0
    {
        VertexShader = compile vs_3_0 MainVS_TEX();
        PixelShader = compile ps_3_0 WhitePS();

        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        CullMode         = None;  // note: not quite optimal
        
        ColorWriteEnable = 0;     // no need to render to color, we only need z
    }
}