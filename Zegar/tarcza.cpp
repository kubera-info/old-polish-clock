#include "stdafx.h"
#include "tarcza.h"

HRESULT CTarcza::Render( LPDIRECT3DDEVICE8 pd3dDevice )
{
	ASERT( pd3dDevice->SetMaterial(&mtrl_tarcz) );
	ASERT( pd3dDevice->SetTexture( 0, g_Texture ) );
	CKombajnGraf::Render( pd3dDevice );
	pd3dDevice->SetTexture(0, NULL);

	return S_OK;
}
