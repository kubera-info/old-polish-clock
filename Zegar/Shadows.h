// #############################################
// ogolne.h - pakiet ogólny, implemetacja cieni.
// #############################################

#ifndef _SHADOWS_H_
#define _SHADOWS_H_

#include "stdafx.h"

// ###########################################################
// Nazwa: class CShadowVolume
// Opis: Implementacja rysowania cienia w czasie rzeczywistym.
//       Zastosowany algorytm pochodzi z DirectX SDK (9a).
// ###########################################################
class CShadowVolume : public CObiektGraf
{
public:
	CShadowVolume(CKombajnGraf * pShadowCasters) : MyShadowCasters(pShadowCasters)
	{
		m_VB = NULL;
	}
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice);
	virtual HRESULT OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
	virtual void OnDestroyDevice()
	{
		SAFE_RELEASE(m_VB);
	}
private:
	void RenderShadow( IDirect3DDevice9* pd3dDevice );
	void DrawShadow( IDirect3DDevice9* pd3dDevice );
	void CShadowVolume::OnDesignModel(
		LPDIRECT3DDEVICE9 pd3dDevice,
		const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
		SHADOWVERTEX p_model []);

	// Lista obiektów do odrysowania.
	// Wœród nich wystêpuj¹ przystosowane do rysowania cienia.
	CKombajnGraf * MyShadowCasters;
	LPDIRECT3DVERTEXBUFFER9 m_VB;
};

#endif