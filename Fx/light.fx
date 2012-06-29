// Copyright (C) 2011 by www.kubera.info
// All Rights Reserved
// Wszelkie prawa zastrze¿one
// Lighting algorithms for "Old Polish Clock 3.0"

#include "struct.fxh"
#include "light.fxh"

texture Tex0;
sampler Sampler = sampler_state
{
	Texture = (Tex0);
	MipFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
	MinFilter = ANISOTROPIC;
};
texture Tex1;
sampler Sampler2 = sampler_state
{
	Texture = (Tex1);
	MipFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
	MinFilter = ANISOTROPIC;
};

float3 BrightColor;
float3 DarkColor;
float3 FresnelColor;

// Textures morphing factor
float4 fLerp;

float4 g_WoodBright;
float4 g_WoodDark;
static float3 g_GlassColor = float3( .97, .97, 1.0 );
static float4 Mtrl_Color = float4( .12, .12, .12, 1.0 );

struct VS_OUTPUT 
{
	float4 Diffuse:				COLOR0;
	float4 Ambient:				COLOR1;
	float4 VertPos:				POSITION;
    float4 ProjectedCoord:		TEXCOORD0;	
};

struct VS_OUTPUT_SPEC_HAND
{
    float2 Brightness:			TEXCOORD0;  
	float4 Diffuse:				COLOR0;
	float4 Ambient:				COLOR1;
	float4 VertPos:				POSITION;
    float4 ProjectedCoord:		TEXCOORD1;	
};

struct VS_OUTPUT_TEX
{
	float4 Diffuse:				COLOR0;
	float4 Ambient:				COLOR1;
    float2 Texture:				TEXCOORD0;
	float4 VertPos:				POSITION;
    float4 ProjectedCoord:		TEXCOORD1;
};

struct VS_OUTPUT_SPEC
{
    float2 Brightness:			TEXCOORD0;  
	float4 VertPos:				POSITION;
    float4 ProjectedCoord:		TEXCOORD1;
};

struct VS_OUTPUT_SPEC_TEX
{
    float2 Brightness:			TEXCOORD0;  
	float4 Diffuse:				COLOR0;
	float4 Ambient:				COLOR1;
	float2 Texture:				TEXCOORD1;
	float4 VertPos:				POSITION;
    float4 ProjectedCoord:		TEXCOORD2;
};

struct VS_OUTPUT_POS
{
    float4 Position  : POSITION;
};

struct VS_OUTPUT_METAL
{
    float3 Brightness:			TEXCOORD0;               
    float4 Position:			SV_Position;
    float4 ProjectedCoord:		TEXCOORD1;
};

struct VS_OUTPUT_METAL_TEX
{
    float3 Brightness:			TEXCOORD0;               
    float2 Texture:				TEXCOORD1;
    float4 Position:			SV_Position;
    float4 ProjectedCoord:		TEXCOORD2;
};


VS_OUTPUT vs_main( VS_INPUT Input )
{
	VS_OUTPUT Out;

	const float3 worldPos = mul( Input.Pos, g_mModel ).xyz;
	const float3 worldNormal = normalize( mul( Input.Normal, ( float3x3 ) g_mModel ) );
	const float4 projPos = mul( Input.Pos, g_mModelViewProj );

	// Move model to the pixel shader
	Out.VertPos = projPos;
	const float3 PixPos = Light1_Position - worldPos;
	const float Distance = length( PixPos );
	Out.Diffuse = DiffuseColor( PixPos, worldNormal, Distance );
	Out.Ambient = AmbientColor( Distance );
	
    // transform model-space vertex position to light-space:
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );

	return Out;
}

VS_OUTPUT_TEX vs_main_tex( VS_INPUT_TEX Input )
{
	VS_OUTPUT_TEX Out;

	const float3 worldPos = mul( Input.Pos, g_mModel ).xyz;
	const float3 worldNormal = normalize( mul( Input.Normal, ( float3x3 ) g_mModel ) );
	const float4 projPos = mul( Input.Pos, g_mModelViewProj );

	// Move model to the pixel shader
	Out.VertPos = projPos;
	const float3 PixPos = Light1_Position - worldPos;
	Out.Texture = Input.Texture;
	const float Distance = length( PixPos );
	Out.Diffuse = DiffuseColor( PixPos, worldNormal, Distance );
	Out.Ambient = AmbientColor( Distance );

    // transform model-space vertex position to light-space:
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );

	return Out;
}

VS_OUTPUT_TEX vs_wood( VS_INPUT_TEX Input )
{
	VS_OUTPUT_TEX Out;

	const float3 worldPos = mul( Input.Pos, g_mModel ).xyz;
	const float3 worldNormal = normalize( mul( Input.Normal, ( float3x3 ) g_mModel ) );
	const float4 projPos = mul( Input.Pos, g_mModelViewProj );

	// Move model to the pixel shader
	Out.VertPos = projPos;
	const float3 PixPos = Light1_Position - worldPos;
	Out.Texture = Input.Texture;
	const float Distance = length( PixPos );
	Out.Diffuse = DiffuseColor( PixPos, worldNormal, Distance );
	Out.Ambient = AmbientColor( Distance );

    // transform model-space vertex position to light-space:
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );

	return Out;
}

VS_OUTPUT_TEX vs_main_brick_tex( VS_INPUT_TEX Input )
{
	VS_OUTPUT_TEX Out;

	const float3 worldPos = mul( Input.Pos, g_mModel ).xyz;

	// Move model to the pixel shader
	
	const float3 PixPos = Light1_Position - worldPos;
	const float Distance = length( PixPos );
	const float AmbientFactor = AmbientColorMinus( Distance );

	if( AmbientFactor > -.5 ) // Discard invisible
	{
		const float3 worldNormal = normalize( mul( Input.Normal, ( float3x3 ) g_mModel ) );
		const float4 projPos = mul( Input.Pos, g_mModelViewProj );
		Out.VertPos = projPos;
		Out.Ambient = float4( Light_Ambient * AmbientFactor, 1.0 );		
		Out.Diffuse = DiffuseColor( PixPos, worldNormal, Distance );
		Out.Texture = Input.Texture;

		// transform model-space vertex position to light-space:
		Out.ProjectedCoord = mul( Input.Pos, TexTransform );
	}
	else
		Out.VertPos.w = -1;
		
	return Out;
}

VS_OUTPUT_SPEC_TEX vs_main_specular_tex( VS_INPUT_TEX Input )
{
	VS_OUTPUT_SPEC_TEX Out;

	const float3 worldPos = mul( Input.Pos, g_mModel ).xyz;
	const float3 worldNormal = normalize( mul( Input.Normal, ( float3x3 ) g_mModel ) );
	const float4 projPos = mul( Input.Pos, g_mModelViewProj );

	// Move model to the pixel shader
	Out.VertPos = projPos;
	const float3 PixPos = Light1_Position - worldPos;
	Out.Texture = Input.Texture;

	const float Distance = length( PixPos );
	Out.Diffuse = DiffuseColor2( PixPos, worldNormal ) * 0.7;
	Out.Ambient = float4( Light_Ambient * 0.225, 1.0 );

	const float3 V = -normalize(worldPos); 
    const float3 H = normalize(PixPos + V);                                // half vector (view space)	

    float f = 0.5 - dot(V, worldNormal); f = 1 - 4 * f * f;              // fresnel term
    
    const float Specular = pow( saturate( dot( H, worldNormal ) ), 32.0 );
    Out.Brightness.x = Specular * .88;
    Out.Brightness.y = f * .1666;


    // transform model-space vertex position to light-space:
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );

	return Out;
}

VS_OUTPUT_SPEC_HAND vs_main_specular_hand( VS_INPUT Input )
{
	VS_OUTPUT_SPEC_HAND Out;

	const float3 worldPos = mul( Input.Pos, g_mModel ).xyz;
	const float3 worldNormal = normalize( mul( Input.Normal, ( float3x3 ) g_mModel ) );
	const float4 projPos = mul( Input.Pos, g_mModelViewProj );

	// Move model to the pixel shader
	Out.VertPos = projPos;
	const float3 PixPos = Light1_Position - worldPos;

	const float Distance = length( PixPos );
	Out.Diffuse = DiffuseColor2( PixPos, worldNormal );
	Out.Ambient = AmbientColor( Distance );

	const float3 V = -normalize(worldPos); 
    const float3 H = normalize(PixPos + V);                                // half vector (view space)	

    //Out.Brightness.x = dot( worldNormal, PixPos );
    //Out.Brightness.x *= Out.Brightness.x * .5;
    float f = 0.5 - dot(V, worldNormal); f = 1 - 4 * f * f;              // fresnel term
    
    float Specular = pow( saturate( dot( H, worldNormal ) ), 22.0 );
    Out.Brightness.x = Specular;
    Out.Brightness.y = f * 0.0625;

    // transform model-space vertex position to light-space:
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );

	return Out;
}

VS_OUTPUT_SPEC vs_main_specular_glass( VS_INPUT Input )
{
	VS_OUTPUT_SPEC Out;

	const float3 worldPos = mul( Input.Pos, g_mModel ).xyz;
	const float3 worldNormal = normalize( mul( Input.Normal, ( float3x3 ) g_mModel ) );
	const float4 projPos = mul( Input.Pos, g_mModelViewProj );

	// Move model to the pixel shader
	Out.VertPos = projPos;
	const float3 PixPos = Light1_Position - worldPos;

	const float3 V = -normalize(worldPos); 
    const float3 H = normalize(PixPos + V);                                // half vector (view space)	

    //Out.Brightness.x = dot( worldNormal, PixPos );
    //Out.Brightness.x *= Out.Brightness.x * .5;
    float f = 0.5 - dot(V, worldNormal); f = 1 - 4 * f * f;              // fresnel term
    
    const float Specular = pow( saturate( dot( H, worldNormal ) ), 32 );
    Out.Brightness.x = min( Specular * 1.8, .88 );
    Out.Brightness.y = f * .1666;

    // transform model-space vertex position to light-space:
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );

	return Out;
}

VS_OUTPUT_POS vs_ambient_glass( VS_INPUT Input )
{
	VS_OUTPUT_POS Out;

	const float4 projPos = mul( Input.Pos, g_mModelViewProj );

	Out.Position = projPos;
	
	return Out;
}

VS_OUTPUT_SPEC_TEX vs_main_specular_wood( VS_INPUT_TEX Input )
{
	VS_OUTPUT_SPEC_TEX Out;

	const float3 worldPos = mul( Input.Pos, g_mModel ).xyz;
	const float3 worldNormal = normalize( mul( Input.Normal, ( float3x3 ) g_mModel ) );
	const float4 projPos = mul( Input.Pos, g_mModelViewProj );

	// Move model to the pixel shader
	Out.VertPos = projPos;
	const float3 PixPos = Light1_Position - worldPos;
	Out.Texture = Input.Texture;

	const float Distance = length( PixPos );
	Out.Diffuse = DiffuseColor( PixPos, worldNormal, Distance ) * 0.233;
	Out.Ambient = AmbientColor( Distance ) * 0.2;

	const float3 V = -normalize(worldPos); 
    const float3 H = normalize(PixPos + V);                                // half vector (view space)	

    //Out.Brightness.x = dot( worldNormal, PixPos );
    //Out.Brightness.x *= Out.Brightness.x * .5;
    //float f = 0.5 - dot(V, worldNormal); f = 1 - 4 * f * f;              // fresnel term
    
    const float Specular = pow( saturate( dot( H, worldNormal ) ), 4 );
    Out.Brightness.x = Specular;                                                     
    Out.Brightness.y = 0.0;
    //Out.Brightness.y = f / 6;


    // transform model-space vertex position to light-space:
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );

	return Out;
}

// Vertex shader
VS_OUTPUT_METAL VS(  
	VS_INPUT Input )
{
    VS_OUTPUT_METAL Out = (VS_OUTPUT_METAL)0;

    const float3 P = mul(Input.Pos, (float4x3)g_mModel);   // position (view space)
    const float3 Lnew = normalize( Light1_Position - P );
    const float3 N = normalize(mul(Input.Normal, (float3x3)g_mModel));     // normal (view space)
    const float3 V = -normalize(P);                                   // view direction (view space)
    const float3 H = normalize(Lnew + V);                                // half vector (view space)
    // Position (projected)
    Out.Position = mul(Input.Pos, g_mModelViewProj);             
    Out.Brightness.x = dot( N, Lnew );
    Out.Brightness.x *= Out.Brightness.x * .2;
    float f = 0.5 - dot(V, N); f = 1 - 4 * f * f;              // fresnel term
    
    float Specular = pow( saturate( dot( H, N ) ), 7.0 ); // , 4.0 );
    Out.Brightness.y = Specular;
    Out.Brightness.z = f * .3;
    
    Out.ProjectedCoord = mul( Input.Pos, TexTransform );
    
    return Out;
}

// Vertex shader
VS_OUTPUT_METAL_TEX VS_TEX(    
    VS_INPUT_TEX Input )
{
    VS_OUTPUT_METAL_TEX Out = (VS_OUTPUT_METAL_TEX)0;
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
float4 PS(VS_OUTPUT_METAL In) : SV_Target
{
    const float shadow = ps_shadow_blur( In.ProjectedCoord );
	float4 Color;
	const float Gloss = In.Brightness.z;
	const float Specular = pow( In.Brightness.y, 4.0 ) * .86;
	float3 ColorLerp = float3( In.Brightness.x, In.Brightness.x, In.Brightness.x );
	ColorLerp += ( Specular + FresnelColor * Gloss ) * shadow;
    Color.rgb = lerp( DarkColor, BrightColor, ColorLerp );
    Color.rgb *= Light1_Color.rgb;
    
    Color.w   = 1.0;

    return Color;
}  

// Pixel shader
float4 PS_TEX(VS_OUTPUT_METAL_TEX In) : COLOR
{
    const float shadow = ps_shadow_blur( In.ProjectedCoord );
	float4 Color;
	const float Specular = In.Brightness.y * In.Brightness.y;
	const float Gloss = In.Brightness.z;
	float3 ColorLerp = float3( In.Brightness.x, In.Brightness.x, In.Brightness.x );
	ColorLerp += Specular;
	ColorLerp += Gloss;
    Color = tex2D( Sampler, In.Texture );
    Color.xyz *= ColorLerp;
    Color.xyz += ColorLerp;
    Color.xyz *= shadow;
    Color.rgb *= Light1_Color.rgb * .5 + Light_Ambient * .125;
    Color.w = 1.0;

    return Color;
}  

float4 ps_main(
	VS_OUTPUT IN
    ) : COLOR 
{
   const float shadow = ps_shadow_blur( IN.ProjectedCoord );
   return IN.Diffuse * shadow + IN.Ambient;
}

float4 ps_main_tex(
	VS_OUTPUT_TEX IN
	) : COLOR 
{
   const float shadow = ps_shadow_blur( IN.ProjectedCoord );
   return ( IN.Diffuse * shadow + IN.Ambient ) * tex2D( Sampler, IN.Texture );
}

float4 ps_main_tex_wood(
	VS_OUTPUT_TEX IN
	) : COLOR 
{
   const float shadow = ps_shadow_blur( IN.ProjectedCoord );
   const float4 diffuse = IN.Diffuse * shadow;
   return ( diffuse + IN.Ambient ) * lerp( g_WoodDark, g_WoodBright, tex2D( Sampler, IN.Texture ) );
}

float4 ps_main_specular_hand(
	VS_OUTPUT_SPEC_HAND In
    ) : COLOR 
{
    const float shadow = sqrt( ps_shadow_blur( In.ProjectedCoord ) );
	const float Gloss = In.Brightness.y;
	const float Specular = pow( In.Brightness.x, 4.0 );
	const float4 alpha = Light1_Color * ( Specular + Gloss );
   return ( In.Diffuse + float4(Light_Ambient, 1.0 ) + alpha ) * shadow * Mtrl_Color;
}

float4 ps_main_specular_glass(
	VS_OUTPUT_SPEC In
    ) : COLOR 
{
	const float shadow = ps_shadow_blur( In.ProjectedCoord );
	const float Gloss = In.Brightness.y;
	const float Specular = pow( In.Brightness.x, 4.0 );
	const float alpha = ( Specular + Gloss ) * shadow;
    
   clip( alpha - .025 );
   return float4( Light1_Color.rgb * g_GlassColor, alpha );
}

float4 ps_ambient_glass(
	VS_OUTPUT_POS In
    ) : COLOR 
{
	return float4( g_GlassColor, .125 );
}

float4 ps_main_specular_wood(
	VS_OUTPUT_SPEC_TEX In
    ) : COLOR 
{
    const float shadow = ps_shadow_blur( In.ProjectedCoord );
	const float Specular = pow( In.Brightness.x, 8.0 );	
	const float SpecularColor = Specular * shadow;
	const float4 TexCol = tex2D( Sampler, In.Texture );

    return ( In.Diffuse + In.Ambient ) * TexCol + SpecularColor * ( 1.0 - TexCol.r * .25 ) * shadow;
}

float4 ps_main_specular_tex(
	VS_OUTPUT_SPEC_TEX In
    ) : COLOR 
{
    const float shadow = ps_shadow_blur( In.ProjectedCoord );
	const float Gloss = In.Brightness.y;
	const float Specular = pow( In.Brightness.x, 8.0 );
	const float SpecularColor = ( Specular + Gloss );

    return ( ( In.Diffuse + SpecularColor ) * ( shadow * .7 + .3 )
    + In.Ambient ) * tex2D( Sampler, In.Texture );
}

float4 ps_main_specular_tex_2(
	VS_OUTPUT_SPEC_TEX In
    ) : COLOR 
{
	const float4 colorTex0 = tex2D( Sampler, In.Texture );
	const float4 colorTex1 = tex2D( Sampler2, In.Texture );
    const float shadow = ps_shadow_blur( In.ProjectedCoord );
	const float Gloss = In.Brightness.y;
	const float Specular = pow( In.Brightness.x, 8.0 );	
	const float SpecularColor = ( Specular + Gloss );
   	return ( ( In.Diffuse + SpecularColor ) * ( shadow * .7 + .3 )
	+ In.Ambient ) * lerp( colorTex1, colorTex0, fLerp );
}

/////////// Effect ////////////

technique TDiffuse
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        	
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}

technique TDiffuseTex
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        	
		VertexShader = compile vs_3_0 vs_main_tex();
		PixelShader = compile ps_3_0 ps_main_tex();
	}
}

technique TDiffuseBrickTex
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        	
		VertexShader = compile vs_3_0 vs_main_brick_tex();
		PixelShader = compile ps_3_0 ps_main_tex();
	}
}

technique TSpecularTex
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        //CullMode         = CW;
        	
		VertexShader = compile vs_3_0 vs_main_specular_tex();
		PixelShader = compile ps_3_0 ps_main_specular_tex();
	}
}

technique TSpecularTex2
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        //CullMode         = CW;
        	        	
		VertexShader = compile vs_3_0 vs_main_specular_tex();
		PixelShader = compile ps_3_0 ps_main_specular_tex_2();
	}
}

technique THand
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        	        	
		VertexShader = compile vs_3_0 vs_main_specular_hand();
		PixelShader = compile ps_3_0 ps_main_specular_hand();
	}
}

technique TGlass
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = True;
        SrcBlend		 = SrcAlpha;
		DestBlend		 = InvSrcAlpha;		
        Lighting         = False;
        		
		VertexShader = compile vs_3_0 vs_main_specular_glass();
		PixelShader = compile ps_3_0 ps_main_specular_glass();
	}
}

technique TAmbientGlass
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = True;
        SrcBlend		 = SrcAlpha;
		DestBlend		 = InvSrcAlpha;		
        Lighting         = False;
        		
		VertexShader = compile vs_3_0 vs_ambient_glass();
		PixelShader = compile ps_3_0 ps_ambient_glass();
	}
}

technique TBlackWood
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;       
        		
		VertexShader = compile vs_3_0 vs_main_specular_wood();
		PixelShader = compile ps_3_0 ps_main_specular_wood();
	}
}

technique TWood
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        		
		VertexShader = compile vs_3_0 vs_main_tex();
		PixelShader = compile ps_3_0 ps_main_tex_wood();
	}
}

technique TGoldenMetallic
{
	pass P0
	{
        ZEnable          = True;
        AlphaBlendEnable = False;
        Lighting         = False;
        //CullMode         = CW;
        	
        VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
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
        	
		VertexShader = compile vs_3_0 VS_TEX();
		PixelShader = compile ps_3_0 PS_TEX();
	}
}