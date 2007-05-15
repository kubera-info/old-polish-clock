#include "Shadows.h"

//-----------------------------------------------------------------------------
// Name: RenderShadow()
// Desc:
//-----------------------------------------------------------------------------
void CShadowVolume::RenderShadow( IDirect3DDevice9* pd3dDevice )
{
	FUNC_TRACE;

	// Disable z-buffer writes (note: z-testing still occurs), and enable the
	// stencil-buffer
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
	pd3dDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );

	// Dont bother with interpolating color
	pd3dDevice->SetRenderState( D3DRS_SHADEMODE,     D3DSHADE_FLAT );

	// Set up stencil compare fuction, reference value, and masks.
	// Stencil test passes if ((ref & mask) cmpfn (stencil & mask)) is true.
	// Note: since we set up the stencil-test to always pass, the STENCILFAIL
	// renderstate is really not needed.
	pd3dDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_ALWAYS );
	pd3dDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
	pd3dDevice->SetRenderState( D3DRS_STENCILFAIL,  D3DSTENCILOP_KEEP );

	// If ztest passes, inc/decrement stencil buffer value
	pd3dDevice->SetRenderState( D3DRS_STENCILREF,       0x1 );
	pd3dDevice->SetRenderState( D3DRS_STENCILMASK,      0xffffffff );
	pd3dDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0xffffffff );
	pd3dDevice->SetRenderState( D3DRS_STENCILPASS,      D3DSTENCILOP_INCR );

	// Make sure that no pixels get drawn to the frame buffer
	// W.Kubera, pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	if( g_bTwoSided )
	{
		// With 2-sided stencil, we can avoid rendering twice:
		pd3dDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, TRUE );
		pd3dDevice->SetRenderState( D3DRS_CCW_STENCILFUNC,  D3DCMP_ALWAYS );
		pd3dDevice->SetRenderState( D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP );
		pd3dDevice->SetRenderState( D3DRS_CCW_STENCILFAIL,  D3DSTENCILOP_KEEP );
		pd3dDevice->SetRenderState( D3DRS_CCW_STENCILPASS, D3DSTENCILOP_DECR );

		pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE );

		// Draw both sides of shadow volume in stencil/z only
		if(MyShadowCasters)
			MyShadowCasters->OnCastShadow( pd3dDevice );

		pd3dDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, FALSE );
	}
	else
	{
		// Draw front-side of shadow volume in stencil/z only
		if(MyShadowCasters)
			MyShadowCasters->OnCastShadow( pd3dDevice );

		// Now reverse cull order so back sides of shadow volume are written.
		pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CW );

		// Decrement stencil buffer value
		pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_DECR );

		// Draw back-side of shadow volume in stencil/z only
		if(MyShadowCasters)
			MyShadowCasters->OnCastShadow( pd3dDevice );
	}

	// Restore render states
	pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,     TRUE );
	pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
	// W. Kubera, pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

}
//-----------------------------------------------------------------------------
// Name: DrawShadow()
// Desc: Draws a big gray polygon over scene according to the mask in the
//       stencil buffer. (Any pixel with stencil==1 is in the shadow.)
//-----------------------------------------------------------------------------
void CShadowVolume::DrawShadow( IDirect3DDevice9* pd3dDevice )
{
	FUNC_TRACE;

	// Set renderstates (disable z-buffering, enable stencil, disable fog, and
	// turn on alphablending)
	pd3dDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );
	pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    TRUE );
	// W. Kubera, pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	/*		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );*/

	// Only write where stencil val >= 1 (count indicates # of shadows that
	// overlap that pixel)
	pd3dDevice->SetRenderState( D3DRS_STENCILREF,  0x1 );
	pd3dDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL );
	pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );

	// Draw a big, gray square
	if(NULL != m_VB)
	{
		pd3dDevice->SetStreamSource( 0, m_VB, NULL, sizeof(SHADOWVERTEX));
		pd3dDevice->SetFVF( SHADOWVERTEX::FVF );
		pd3dDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP, 
			0, 
			2);
	}

	// Restore render states
	pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
	pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
	// W. Kubera, pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

void CShadowVolume::OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice)
{
	RenderShadow( pd3dDevice );
	DrawShadow( pd3dDevice );
}

HRESULT CShadowVolume::OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	HRESULT hr;

	if(NULL != m_VB)
		SAFE_RELEASE(m_VB);

	SHADOWVERTEX * m_model = NULL;

	V_RETURN(pd3dDevice->CreateVertexBuffer( 4 * sizeof(SHADOWVERTEX),
		D3DUSAGE_WRITEONLY, GetFVF() ,
		D3DPOOL_MANAGED, &m_VB, NULL ) );
	V_RETURN((m_VB->Lock( 0, 0, (VOID **) &m_model, 0 ) ) );
	if( m_model )
	{
		OnDesignModel(pd3dDevice, pBackBufferSurfaceDesc, m_model);
	}
	return m_VB->Unlock();
}

void CShadowVolume::OnDesignModel(
								  LPDIRECT3DDEVICE9 pd3dDevice,
								  const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
								  SHADOWVERTEX p_model [])
{
	FLOAT sx = (FLOAT)pBackBufferSurfaceDesc->Width;
	FLOAT sy = (FLOAT)pBackBufferSurfaceDesc->Height;

	p_model[0].pos = D3DXVECTOR3(  0, sy, 0.0f);
	p_model[1].pos = D3DXVECTOR3(  0,  0, 0.0f);
	p_model[2].pos = D3DXVECTOR3( sx, sy, 0.0f);
	p_model[3].pos = D3DXVECTOR3( sx,  0, 0.0f);
	p_model[0].rhw = 1.0f;
	p_model[1].rhw = 1.0f;
	p_model[2].rhw = 1.0f;
	p_model[3].rhw = 1.0f;
	p_model[0].color = 0x46000000;
	p_model[1].color = 0x46000000;
	p_model[2].color = 0x46000000;
	p_model[3].color = 0x46000000;
}
