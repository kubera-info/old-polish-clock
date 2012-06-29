#pragma once
#ifndef _OPC_STRUCT_H_
#define _OPC_STRUCT_H_

#define CB_LIGHT_POS ( 1 )
#define CB_MODEL ( 2 )
#define CB_METAL ( 3 )
#define CB_LERP ( 4 )
#define CB_WOOD ( 5 )

struct StructHandlesD3D9
{
	void SetHandles( const LPD3DXEFFECT pEffect, const LPD3DXEFFECT pShadowEffect );

	D3DXHANDLE hLight1_Position;
	D3DXHANDLE hg_mModelViewProj;
	D3DXHANDLE hg_mModel;
	D3DXHANDLE hg_WoodBright;
	D3DXHANDLE hg_WoodDark;
	D3DXHANDLE hBrightColor;
	D3DXHANDLE hDarkColor;
	D3DXHANDLE hFresnelColor;
	D3DXHANDLE hg_mShadowModelViewProj;
	D3DXHANDLE hTexTransform;
	D3DXHANDLE hShadowMap;
	D3DXHANDLE hShadowMapDyn;
	D3DXHANDLE hTex0;
	D3DXHANDLE hTex1;
	D3DXHANDLE hfLerp;

private:	
	D3DXHANDLE GetHandle( const LPD3DXEFFECT pCurrentEffect, const char * pParamName ) const;
} extern gD3D9Handles;

#endif // _OPC_STRUCT_H_
