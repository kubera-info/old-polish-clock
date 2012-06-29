// Copyright (C) 2011 by www.kubera.info
// All Rights Reserved
// Wszelkie prawa zastrze¿one
// Lighting algorithms for "Old Polish Clock 3.0"

float Attn(
	float Distance,
	float AttnDist,
	float AttnDiv )
{
	return ( AttnDist - Distance + 3.125 ) * AttnDiv;
}

float4 DiffuseColor(
	float3 inPixPos,
	float3 inNormal,
	float Distance)
{
	// Compute the lighting contribution for this single light
	const float3 LightVec = inPixPos;
	const float3 LightDir = normalize( LightVec );
	const float LightAttn = max( Attn( Distance, 1.28, 1 / 1.28 ), 0.0 );
	return max( float4( 0.0, 0.0, 0.0, 0.0 ), dot( LightDir, inNormal ) * Light1_Color * LightAttn );
}

float4 DiffuseColor2(
	float3 inPixPos,
	float3 inNormal)
{
	// Compute the lighting contribution for this single light
	const float3 LightVec = inPixPos;
	const float3 LightDir = normalize( LightVec );
	const float LightAttn = .925;
	const float4 Normal = float4( inNormal, 1.0f );
	return max( float4( 0.0, 0.0, 0.0, 0.0 ), dot( LightDir, inNormal ) * Light1_Color * LightAttn );
}

float4 AmbientColor(
	float Distance)
{
	float LightAttnAmbient = max( Attn( Distance, 5.3, 1 / 5.3 ), 0.0 );
	return float4( Light_Ambient * LightAttnAmbient, 1.0 );
}

float4 AmbientColor2(
	float Distance)
{
	float LightAttnAmbient = max( Attn( Distance, 5.3, 1 / 5.3 ), 0.0 );
	return float4( Light_Ambient * LightAttnAmbient, 1.0 );
}

float AmbientColorMinus(
	float Distance)
{
	return Attn( Distance, 5.3, 1 / 5.3 );	
}