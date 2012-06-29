//
// Based on Metallic Flakes Shader by Microsoft
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------

Texture2D Tex0 : register( t0 );
SamplerState Sampler : register( s0 );

// Light direction (view space)
cbuffer PointL : register( b0 )
{
float3 Light1_Position   : packoffset( c0 );
float3 Light1_Color      : packoffset( c1 );
float3 Light_Ambient     : packoffset( c2 );
}

// Transformations
cbuffer Model : register( b1 )
{
float4x4 g_mModelViewProj;
float4x4 g_mModel;
float4   g_Vector1;
}

static const float  MaxByte = 255.0;
static const float3 BrightColor = float3( 254.0 / MaxByte, 252.0 / MaxByte, 254.0 / MaxByte );
static const float3 DarkColor = float3( 64.0 / MaxByte, 33 / MaxByte, 5 / MaxByte );
static const float3 FresnelColor = float3( 80 / MaxByte, 65 / MaxByte, 15 / MaxByte );
struct VS_INPUT
{
    float4 Pos          : POSITION; // vertex position 
    float3 Normal       : NORMAL;   // this normal comes in per-vertex
};

struct VS_INPUT_TEX
{
    float4 Pos          : POSITION; // vertex position 
    float3 Normal       : NORMAL;   // this normal comes in per-vertex
    float2 Texture      : TEXCOORD0;// vertex texture coords 
};

struct VS_OUTPUT
{
    float3 Brightness : TEXCOORD0;               
    float4 Position   : SV_Position;
};

struct VS_OUTPUT_TEX
{
    float3 Brightness : TEXCOORD0;               
    float2 Texture    : TEXCOORD1;
    float4 Position   : SV_Position;
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
    Out.Brightness.x *= Out.Brightness.x * .5;
    float f = 0.5 - dot(V, N); f = 1 - 4 * f * f;              // fresnel term
    
    float Specular = saturate( dot( H, N ) );
    Specular *= Specular;                              
    Specular *= Specular;                                                        
    Specular *= Specular;                                                        
    Out.Brightness.y = Specular;
    Out.Brightness.z = f / 6;
    
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
       
    return Out;
}

/////////// PS ////////////


// Pixel shader
float4 PS(VS_OUTPUT In) : SV_Target
{
	float4 Color;
	float Specular = In.Brightness.y;
	float Gloss = In.Brightness.z;
	Specular *= Specular;
	Specular *= Specular;
	Specular /= 2.0;
	float3 ColorLerp = float3( In.Brightness.x, In.Brightness.x, In.Brightness.x );
	ColorLerp += Specular + FresnelColor * Gloss;
    Color.rgb = lerp( DarkColor, BrightColor, ColorLerp );
    Color.rgb *= Light1_Color + Light_Ambient;
    Color.w   = 1.0;

    return Color;
}  

// Pixel shader
float4 PS_TEX(VS_OUTPUT_TEX In) : SV_Target
{   
	float4 Color;
	float Specular = In.Brightness.y;
	float Gloss = In.Brightness.z;
	Specular *= Specular;
	float3 ColorLerp = float3( In.Brightness.x, In.Brightness.x, In.Brightness.x );
	ColorLerp += Specular;
	ColorLerp += Gloss;
    Color = Tex0.Sample( Sampler, In.Texture );
    Color.xyz *= ColorLerp;
    Color.xyz += ColorLerp;
    Color.xyz /= 2;
    Color.rgb *= Light1_Color + Light_Ambient;
    Color.rgb = saturate( Color.rgb );
    Color.w = 1.0;

    return Color;
}