#ifndef _SHADOWVOLUME_H_
#define _SHADOWVOLUME_H_

#include "stdafx.h"

//-----------------------------------------------------------------------------
// Name: struct ShadowVolume
// Desc: A shadow volume object, modyfikacja dla celów Zegara
//-----------------------------------------------------------------------------

class CShadowVolume
{
    D3DXVECTOR3 * m_pVertices; // Vertex data for rendering shadow volume
    WORD       m_dwNumVertices;
	D3DXVECTOR3 vLight;

public:
	CShadowVolume()
	{
		Reset(light.Direction);
		m_pVertices = new D3DXVECTOR3 [16000];
	}
    VOID Reset(D3DVECTOR & pLight)
	{ 
		vLight[0] = - pLight.x;
		vLight[1] = - pLight.y;
		vLight[2] = pLight.z;
		m_dwNumVertices = 0L;
	}
	HRESULT AddFromPrimitive(
		class CPrymityw & pPrymityw);
	HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice );

};

#endif