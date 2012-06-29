#include "DXUT.h"
#include "MonadException.h"
#include "Singleton.h"
#include "OPC-Struct.h"

struct StructHandlesD3D9 gD3D9Handles;

void StructHandlesD3D9::SetHandles( const LPD3DXEFFECT pEffect, const LPD3DXEFFECT pShadowEffect )
{
	hLight1_Position = GetHandle( pEffect, "Light1_Position" );
	hg_mModelViewProj = GetHandle( pEffect, "g_mModelViewProj" );
	hg_mModel = GetHandle( pEffect, "g_mModel" );
	hg_WoodBright = GetHandle( pEffect, "g_WoodBright" );
	hg_WoodDark = GetHandle( pEffect, "g_WoodDark" );
	hBrightColor = GetHandle( pEffect, "BrightColor" );
	hDarkColor = GetHandle( pEffect, "DarkColor" );
	hFresnelColor = GetHandle( pEffect, "FresnelColor" );
	hg_mShadowModelViewProj = GetHandle( pShadowEffect, "g_mShadowModelViewProj" );
	hTexTransform = GetHandle( pEffect, "TexTransform" );
	hShadowMap = GetHandle( pEffect, "ShadowMap" );
	hShadowMapDyn = GetHandle( pEffect, "ShadowMapDyn" );
	hTex0 = GetHandle( pEffect, "Tex0" );
	hTex1 = GetHandle( pEffect, "Tex1" );
	hfLerp = GetHandle( pEffect, "fLerp" );
}
D3DXHANDLE StructHandlesD3D9::GetHandle( const LPD3DXEFFECT pCurrentEffect, const char * pParamName ) const
{
	assert( pCurrentEffect );
	D3DXHANDLE dummy( pCurrentEffect->GetParameterByName( NULL, pParamName ) );
	assert( dummy );
	THROW_EXC_IFZ( dummy, MonadException::NoDataFound, L"Technique GetParam" );
	return dummy;
}
