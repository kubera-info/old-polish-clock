#include "input.fxh"

// Lights
cbuffer cLightPos : register( b1 )
{
	float3 Light1_Position;
}

static const float4 Light1_Color = float4( 0.89f, 0.86f, 0.79f, 1.0f );
static const float3 Light_Ambient = float3( .46f, .46f, .36f );

cbuffer cModel : register( b2 )
{
// Transformations
	float4x4 g_mModelViewProj;
	float4x4 g_mModel;
	float4x4 TexTransform;
	float4x4 TexTransform1;
	float4x4 TexTransform2;
}

//-----------------------------------------------------------------------------

Texture2D ShadowMap0 : register( t0 );
Texture2D ShadowMap1 : register( t1 );
Texture2D ShadowMap2 : register( t2 );
Texture2D ShadowMapDyn0 : register( t3 );
Texture2D ShadowMapDyn1 : register( t4 );
Texture2D ShadowMapDyn2 : register( t5 );

//-----------------------------------------------------------------------------

SamplerState ShadowSampler : register( s0 );

//-----------------------------------------------------------------------------

static const float SHADOW_EPSILON = .001;
                                    
// Routine to blur monochrome image
float ps_shadow_blur( float4 tc0, float4 tc1, float4 tc2 )
{
	tc0.xyz /= tc0.w;
	float test0;
	float depth0 = tc0.z;
	if( tc0.x < -1.0 || tc0.x > 1.0 || tc0.y < -1.0 || tc0.y > 1.0 )
		test0 = 1.0;
	else
	{
		tc0.x = +.5 * tc0.x + .5;
		tc0.y = -.5 * tc0.y + .5;
		test0 = depth0 <= ShadowMap0.Sample( ShadowSampler, tc0.xy ).r + SHADOW_EPSILON 
					&& depth0 <= ShadowMapDyn0.Sample( ShadowSampler, tc0.xy ).r + SHADOW_EPSILON;
	}

	tc1.xyz /= tc1.w;
	float test1;
	float depth1 = tc1.z;
	if( tc1.x < -1.0 || tc1.x > 1.0 || tc1.y < -1.0 || tc1.y > 1.0 )
		test1 = 1.0;
	else
	{
		tc1.x = +.5 * tc1.x + .5;
		tc1.y = -.5 * tc1.y + .5;
		test1 = depth1 <= ShadowMap1.Sample( ShadowSampler, tc1.xy ).r + SHADOW_EPSILON 
					&& depth1 <= ShadowMapDyn1.Sample( ShadowSampler, tc1.xy ).r + SHADOW_EPSILON;
	}

	tc2.xyz /= tc2.w;
	float test2;
	float depth2 = tc2.z;
	if( tc2.x < -1.0 || tc2.x > 1.0 || tc2.y < -1.0 || tc2.y > 1.0 )
		test2 = 1.0;
	else
	{
		tc2.x = +.5 * tc2.x + .5;
		tc2.y = -.5 * tc2.y + .5;
		test2 = depth2 <= ShadowMap2.Sample( ShadowSampler, tc2.xy ).r + SHADOW_EPSILON 
					&& depth2 <= ShadowMapDyn2.Sample( ShadowSampler, tc2.xy ).r + SHADOW_EPSILON;
	}
	
				
	return ( test0 + test1 + test2 ) * ( 1.0 / 3.0 );
}