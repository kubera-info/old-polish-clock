#include "input.fxh"

// Lights
float3 Light1_Position;
static const float4 Light1_Color = float4( 0.89f, 0.86f, 0.79f, 1.0f );
static const float3 Light_Ambient = float3( .46f, .46f, .36f );
// Transformations
float4x4 g_mModelViewProj;
float4x4 g_mModel;
float4x4 TexTransform;

//-----------------------------------------------------------------------------

texture ShadowMap;
texture ShadowMapDyn;

//-----------------------------------------------------------------------------

sampler ShadowMapSampler = sampler_state
{
    Texture = <ShadowMap>;
    MinFilter = Linear;  
    MagFilter = Linear;
    MipFilter = None;
    AddressU  = Clamp;
    AddressV  = Clamp;
};

sampler ShadowMapDynSampler = sampler_state
{
    Texture = <ShadowMapDyn>;
    MinFilter = Linear;  
    MagFilter = Linear;
    MipFilter = None;
    AddressU  = Clamp;
    AddressV  = Clamp;
};

//-----------------------------------------------------------------------------

#define NUM_BLUR_TAPS 12

float2 filterTaps[NUM_BLUR_TAPS] = {{-0.326212f, -0.405805f},
                                    {-0.840144f, -0.07358f},
                                    {-0.695914f,  0.457137f},
                                    {-0.203345f,  0.620716f},
                                    { 0.96234f,  -0.194983f},
                                    { 0.473434f, -0.480026f},
                                    { 0.519456f,  0.767022f},
                                    { 0.185461f, -0.893124f},
                                    { 0.507431f,  0.064425f},
                                    { 0.89642f,   0.412458f},
                                    {-0.32194f,  -0.932615f},
                                    {-0.791559f, -0.597705f}};

// Routine to blur monochrome image
float ps_shadow_blur( float4 tc0 )
{
   float colorSum = 0.0f;
   const float fScale = 0.0041f;

	// Run through all taps in the Poisson Disc
	for (int i = 0; i < NUM_BLUR_TAPS; i++)
	{
		// Compute tap coordinates
		float4 tapCoord = float4( tc0.xy + filterTaps[i] * fScale * (1.0f-tc0.y), tc0.z, tc0.w );

		// Fetch tap sample
		const float tapColor = tex2Dproj( ShadowMapSampler, tapCoord ).r;
		const float tapColor2 = tex2Dproj( ShadowMapDynSampler, tapCoord ).r;
		
		float tapColorAll = min( tapColor, tapColor2 );

		// Accumulate color and contribution
		colorSum += tapColorAll;
	}

	// Divide down the accumulated color
	float finalColor = colorSum / ( NUM_BLUR_TAPS );

	return finalColor;
}