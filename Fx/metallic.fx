// Based on Metallic Flakes Shader by Microsoft

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------

#include "struct.fxh"

texture Tex0;
sampler Sampler = sampler_state
{
	Texture = (Tex0);
	MipFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
	MinFilter = ANISOTROPIC;	
};

static const float  MaxByte = 255.0;
static const float3 BrightColor = float3( 254.0 / MaxByte, 252.0 / MaxByte, 254.0 / MaxByte );
static const float3 DarkColor = float3( 52.0 / MaxByte, 28 / MaxByte, 4 / MaxByte );
static const float3 FresnelColor = float3( 80 / MaxByte, 65 / MaxByte, 15 / MaxByte );

struct VS_OUTPUT
{
    float3 Brightness : TEXCOORD0;               
    float4 Position   : SV_Position;
    float4 ProjectedCoord: TEXCOORD1;
};

struct VS_OUTPUT_TEX
{
    float3 Brightness : TEXCOORD0;               
    float2 Texture    : TEXCOORD1;
    float4 Position   : SV_Position;
    float4 ProjectedCoord: TEXCOORD2;
};

// Vertex shader
VS_OUTPUT VS(  
	VS_INPUT Input )
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    const float3 P = mul(Input.Pos, (float4x3)g_mModel);   // position (view space)
    const float3 Lnew = normalize( Light1_Position - P );
    const float3 N = normalize(mul(Input.Normal, (float3x3)g_mModel));     // normal (view space)
    const float3 V = -normalize(P);                                   // view direction (view space)
    const float3 H = normalize(Lnew + V);                                // half vector (view space)
    // Position (projected)
    Out.Position = mul(Input.Pos, g_mModelViewProj);             
    Out.Brightness.x = dot( N, Lnew );
    Out.Brightness.x *= Out.Brightness.x * .175;
    float f = 0.5 - dot(V, N); f = 1 - 4 * f * f;              // fresnel term
    
    float Specular = pow( saturate( dot( H, N ) ), 4.0 );
    Out.Brightness.y = Specular;
    Out.Brightness.z = f / 4;
    
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );
    
    return Out;
}

// Vertex shader
VS_OUTPUT_TEX VS_TEX(    
    VS_INPUT_TEX Input )
{
    VS_OUTPUT_TEX Out = (VS_OUTPUT_TEX)0;
    Out.Texture = Input.Texture;
    const float3 P = mul(Input.Pos, (float4x3)g_mModel);   // position (view space)
    const float3 Lnew = normalize( Light1_Position - P );
    const float3 N = normalize(mul(Input.Normal, (float3x3)g_mModel));     // normal (view space)
    const float3 V = -normalize(P);                                   // view direction (view space)
    const float3 H = normalize(Lnew + V);                                // half vector (view space)
    // Position (projected)
    Out.Position = mul(Input.Pos, g_mModelViewProj);             
    Out.Brightness.x = dot( N, Lnew ) / 4;
    float f = 0.5 - dot(V, N); f = 1 - 4 * f * f;              // fresnel term
    
    float Specular = saturate( dot( H, N ) );
    Specular *= Specular;                        
    Specular *= .3;
    Out.Brightness.y = Specular;
    Out.Brightness.z = f;
       
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );
    
    return Out;
}

/////////// PS ////////////


// Pixel shader
float4 PS(VS_OUTPUT In) : SV_Target
{
    const float shadow = ps_shadow_blur( In.ProjectedCoord );
	float4 Color;
	float Specular = In.Brightness.y;
	float Gloss = In.Brightness.z;
	Specular *= Specular;
	Specular *= Specular;
	Specular *= .86;
	float3 ColorLerp = float3( In.Brightness.x, In.Brightness.x, In.Brightness.x );
	ColorLerp += ( Specular + FresnelColor * Gloss ) * shadow;
    Color.rgb = lerp( DarkColor, BrightColor, ColorLerp );
    Color.rgb *= Light1_Color + Light_Ambient / 8.0;
    
    Color.w   = 1.0;

    return Color;
}  

// Pixel shader
float4 PS_TEX(VS_OUTPUT_TEX In) : COLOR
{
    const float shadow = ps_shadow_blur( In.ProjectedCoord );
	float4 Color;
	float Specular = In.Brightness.y;
	float Gloss = In.Brightness.z;
	Specular *= Specular;
	float3 ColorLerp = float3( In.Brightness.x, In.Brightness.x, In.Brightness.x );
	ColorLerp += Specular;
	ColorLerp += Gloss;
    Color = tex2D( Sampler, In.Texture );
    Color.xyz *= ColorLerp;
    Color.xyz += ColorLerp;
    Color.xyz *= shadow;
    Color.rgb *= Light1_Color * .5 + Light_Ambient * .125;
    Color.w = 1.0;

    return Color;
}  

/////////// Effect ////////////

technique TGoldenMetallic
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        //CullMode         = CW;
        	
        VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}

technique TMetallicTex
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        //CullMode         = CW;
        	
		VertexShader = compile vs_2_0 VS_TEX();
		PixelShader = compile ps_2_0 PS_TEX();
	}
}