// Zegar.cpp : Defines the entry point for the application.
//
#ifndef STRICT
#define STRICT
#endif
#include <Windows.h>
#include <time.h>
#include "stdafx.h"
#include "D3DSaver.h"
#include "Konfig.h"
#include "tarcza.h"
#include "wskaz.h"
#include "muterka.h"
#include "ShadowVolume.h"
#include "dzwiek.h"
#include "Zegar.h"
#include "D3DUtil.h"

// #############################################
// Nazwa: class CD3DZegarSaver
// Opis: G³ówna funkcjonalnoœæ wygaszacz ekranu
// #############################################
class CD3DZegarSaver * Zegar3D;

HRESULT CD3DZegarSaver::Render()
{
	FUNC_TRACE;

#ifdef OBROT
	D3DXMATRIXA16 mat;
	m_pd3dDevice->GetTransform(D3DTS_PROJECTION, &mat);
	D3DXMatrixRotationY(&mat, D3DX_PI * 0.2f);
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &mat);
#endif
	try
	{
		ASERT(m_pd3dDevice->SetLight( 0, &m_tmp_light ));
		ASERT(m_pd3dDevice->LightEnable( 0, TRUE));
		m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB((UCHAR)(0x52 * m_fSwiatlo), (UCHAR)(0x52 * m_fSwiatlo), (UCHAR)(0x49 * m_fSwiatlo)));

		ASERT(m_pd3dDevice->Clear( 0L, NULL, 
			(m_bUseDepthBuffer ?
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL :
			D3DCLEAR_TARGET), D3DCOLOR_XRGB((UCHAR)(10 * m_fSwiatlo), (UCHAR)(10 * m_fSwiatlo), (UCHAR)(100 * m_fSwiatlo)), 1.0f, 0L ));
		// Begin the scene.
		ASERT( m_pd3dDevice->BeginScene() ); // Jeœli ta funkcja wykona poprawn¹ operacjê to w zasadzie ryzyko jest ma³e
		if(m_bUseDepthBuffer)
			m_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
		m_tarcza->Render(m_pd3dDevice);
		if(m_bUseDepthBuffer)
			m_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_GREATEREQUAL );

		m_wsk_godz->Render(m_pd3dDevice);
		m_wsk_min->Render(m_pd3dDevice);
		if(g_bWskazSek)
			m_wsk_sec->Render(m_pd3dDevice);

		ASERT(m_pd3dDevice->SetTransform(D3DTS_WORLD, &g_world));
		m_montaz->Render(m_pd3dDevice);
		

		//m_pShadowVolume->Render( m_pd3dDevice );

		if(m_bUseDepthBuffer)
		{			
			RenderShadow();
			DrawShadow();
		}
		
		// End the scene.
		m_pd3dDevice->EndScene();
	}
	catch(CBlad & e)
	{
		e.MsgBox();
		return E_FAIL;
	}		
	return S_OK;
}
HRESULT CD3DZegarSaver::FrameMove()
{
	FUNC_TRACE;

	static LONG_PTR m_il_gong = -4;
	SYSTEMTIME m_aktual_czas;
	GetLocalTime(&m_aktual_czas);
	if(g_pDzwiek != NULL)
	{
		if(m_aktual_czas.wHour != m_poprzedz_czas.wHour)
		{
			m_il_gong = m_aktual_czas.wHour % 12;
			if(m_il_gong == 0)
				m_il_gong = 12;
		}
		else if(m_aktual_czas.wMinute == 30 && m_aktual_czas.wMinute > m_poprzedz_czas.wMinute)
			m_il_gong = 1;
		
#ifdef _DEBUG
		if(m_aktual_czas.wMinute > m_poprzedz_czas.wMinute)
			m_il_gong = 4;
#endif

		if(m_aktual_czas.wSecond != m_poprzedz_czas.wSecond)
		{			
			if(m_il_gong > -4 )
			{
				if(m_il_gong > 0)
					g_pDzwiek->GrajGong();
				m_il_gong --;
			}
			else
				g_pDzwiek->GrajTik();
		}
	} // g_pDzwiek != NULL

#ifndef	LUMINACJA
	m_fSwiatlo =
		(1.0f + cosf(D3DX_PI * 2 * ((float) m_aktual_czas.wSecond + (float) m_aktual_czas.wMilliseconds / 1000) / 60.0f)) / 2.0f;
#else
	m_fSwiatlo = 1.0f;
#endif
	m_tmp_light = light;
	m_tmp_light.Diffuse.r *= m_fSwiatlo;
	m_tmp_light.Diffuse.g *= m_fSwiatlo;
	m_tmp_light.Diffuse.b *= m_fSwiatlo;

	m_wsk_min->Oblicz(m_aktual_czas);
	m_wsk_godz->Oblicz(m_aktual_czas);
	if(g_bWskazSek)
		m_wsk_sec->Oblicz(m_aktual_czas);

	/*	if(m_bUseDepthBuffer)
	{
		bool m_bObliczCien = m_wsk_min->Oblicz(m_aktual_czas);
			m_bObliczCien |= m_wsk_godz->Oblicz(m_aktual_czas);
		if(g_bWskazSek)
			m_bObliczCien |= m_wsk_sec->Oblicz(m_aktual_czas);
		if(m_bObliczCien)
		{
			//m_tarcza->DajDefinicje( );
			m_wsk_min->DajDefinicje( );
			m_wsk_godz->DajDefinicje( );
			if(g_bWskazSek)
				m_wsk_sec->DajDefinicje( );
			m_montaz->DajDefinicje( );
		}
	}*/

	m_poprzedz_czas = m_aktual_czas;

	return S_OK;
}
// #######
// RenderAllShadows - funkcja rysuje wszystkie woluminy cienia
// #######
void CD3DZegarSaver::RenderAllShadows()
{
	m_wsk_min->RenderShadow( m_pd3dDevice );
	m_wsk_godz->RenderShadow( m_pd3dDevice );
	if(g_bWskazSek)
		m_wsk_sec->RenderShadow( m_pd3dDevice );
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &g_world);
	m_montaz->RenderShadow( m_pd3dDevice );
}

//-----------------------------------------------------------------------------
// Name: RenderShadow()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DZegarSaver::RenderShadow()
{
	FUNC_TRACE;

    // Disable z-buffer writes (note: z-testing still occurs), and enable the
    // stencil-buffer
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );

    // Dont bother with interpolating color
    m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE,     D3DSHADE_FLAT );

    // Set up stencil compare fuction, reference value, and masks.
    // Stencil test passes if ((ref & mask) cmpfn (stencil & mask)) is true.
    // Note: since we set up the stencil-test to always pass, the STENCILFAIL
    // renderstate is really not needed.
    m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_ALWAYS );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILFAIL,  D3DSTENCILOP_KEEP );

    // If ztest passes, inc/decrement stencil buffer value
    m_pd3dDevice->SetRenderState( D3DRS_STENCILREF,       0x1 );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILMASK,      0xffffffff );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0xffffffff );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS,      D3DSTENCILOP_INCR );

    // Make sure that no pixels get drawn to the frame buffer
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

/*    if( m_bTwoSided )
    {
        // With 2-sided stencil, we can avoid rendering twice:
        m_pd3dDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_CCW_STENCILFUNC,  D3DCMP_ALWAYS );
        m_pd3dDevice->SetRenderState( D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP );
        m_pd3dDevice->SetRenderState( D3DRS_CCW_STENCILFAIL,  D3DSTENCILOP_KEEP );
        m_pd3dDevice->SetRenderState( D3DRS_CCW_STENCILPASS, D3DSTENCILOP_DECR );

        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE );

        // Draw both sides of shadow volume in stencil/z only
//        m_pShadowVolume->Render( m_pd3dDevice );
		RenderAllShadows();

        m_pd3dDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, FALSE );
    }
    else*/
    {
        // Draw front-side of shadow volume in stencil/z only
		
//        m_pShadowVolume->Render( m_pd3dDevice );
		RenderAllShadows();

        // Now reverse cull order so back sides of shadow volume are written.
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CW );

        // Decrement stencil buffer value
        m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_DECR );

        // Draw back-side of shadow volume in stencil/z only
//        m_pShadowVolume->Render( m_pd3dDevice );
		RenderAllShadows();
    }

    // Restore render states
    m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,     TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

    return S_OK;
}
//-----------------------------------------------------------------------------
// Name: DrawShadow()
// Desc: Draws a big gray polygon over scene according to the mask in the
//       stencil buffer. (Any pixel with stencil==1 is in the shadow.)
//-----------------------------------------------------------------------------
HRESULT CD3DZegarSaver::DrawShadow()
{
	FUNC_TRACE;

    // Set renderstates (disable z-buffering, enable stencil, disable fog, and
    // turn on alphablending)
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

    // Only write where stencil val >= 1 (count indicates # of shadows that
    // overlap that pixel)
    m_pd3dDevice->SetRenderState( D3DRS_STENCILREF,  0x1 );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );

    // Draw a big, gray square
    m_pd3dDevice->SetVertexShader( SHADOWVERTEX::FVF );
    m_pd3dDevice->SetStreamSource( 0, m_pBigSquareVB, sizeof(SHADOWVERTEX) );
    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

    // Restore render states
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

    return S_OK;
}
void CD3DZegarSaver::BuildProjectionMatrix2( )
{
#ifndef OBROT
	// Pierwszy etap: wspó³czynniki logiczne pikseli
	float dx, dy;
	FLOAT l,r,t,b;

	if( m_bAllScreensSame )
	{
		dx = (float) (m_rcRenderCurDevice.right - m_rcRenderCurDevice.left);
		dy = (float) (m_rcRenderCurDevice.bottom - m_rcRenderCurDevice.top);
	}
	else
	{
		dx = (float) (m_rcRenderTotal.right - m_rcRenderTotal.left);
		dy = (float) (m_rcRenderTotal.bottom - m_rcRenderTotal.top);
	}

	// Etap drugi: wspó³czynniki monitora (gêstoœæ i proporcja pikseli)
	D3DDEVICE_CREATION_PARAMETERS Parameters;
	m_pd3dDevice->GetCreationParameters(  &Parameters );
	HMONITOR hMonitorAktual = m_pD3D->GetAdapterMonitor(Parameters.AdapterOrdinal);
	for(DWORD m_ind_mon = 0; m_ind_mon < m_dwNumMonitors; m_ind_mon ++)
		if(m_Monitors[m_ind_mon].hMonitor == hMonitorAktual)
		{
			HDC Kontekst = CreateDC("DISPLAY", m_Monitors[m_ind_mon].strMonitorName, NULL, NULL);
			if(Kontekst)
			{
				float wsp1 = ((float) GetDeviceCaps(Kontekst, HORZSIZE))
							/ ((float) GetDeviceCaps(Kontekst, VERTSIZE));
				float wsp2 = ((float) GetDeviceCaps(Kontekst, HORZRES))
							/ ((float) GetDeviceCaps(Kontekst, VERTRES));
				float wsp = wsp1 / wsp2;
				DeleteDC(Kontekst);
				dx *= wsp;
			}
			break;
		}

	// Etap trzeci: proporcje dla macierzy
	if(dx > dy)
	{
		l = (FLOAT) (- dx / dy) * 1.1f;
		r = (FLOAT) (  dx / dy) * 1.1f;
		t = (FLOAT) (- 1.0f) * 1.1f;
		b = (FLOAT) (  1.0f) * 1.1f;
	}
	else
	{
		l = (FLOAT) (- 1.0f) * 1.1f;
		r = (FLOAT) (  1.0f) * 1.1f;
		t = (FLOAT) (- dy / dx) * 1.1f;
		b = (FLOAT) (  dy / dx) * 1.1f;
	}		
	
	// Etap czwarty: macierze charakteryzuj¹ce przestrzeñ
	D3DXMatrixIdentity(&m_proj);
	D3DXMatrixIdentity(&m_view);
	D3DXMatrixIdentity(&g_world);
	const static float m_skala = ((float) g_wWielkosc) / 10000;
	if(dx > dy)
		D3DXMatrixScaling( &g_world, m_skala * dy / dx, m_skala, m_skala );
	else
		D3DXMatrixScaling( &g_world, m_skala, m_skala * dx / dy, m_skala );
	ASERT(m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_view));
	ASERT(m_pd3dDevice->SetTransform(D3DTS_WORLD, &g_world));
	ASERT(m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_proj));
#endif
}
void CD3DZegarSaver::LoadTexture(INT p_IDB, LPDIRECT3DTEXTURE8 & p_texture)
{	
	FUNC_TRACE;

	DWORD len;
	LPCVOID lpRsrc = ZaladujZasob(p_IDB, "PNG", len);
	ASERTNZ(lpRsrc);
	ASERT(D3DXCreateTextureFromFileInMemory (
		m_pd3dDevice, 
		lpRsrc, 
		len,
		&p_texture));
}
HRESULT CD3DZegarSaver::RestoreDeviceObjects()
{

	FUNC_TRACE;

	if( m_pd3dDevice == NULL )
		return S_OK;

	try
	{		
		LoadTexture(IDB_TARCZA, g_Texture);
		LoadTexture(IDB_MOSIADZ, g_Texture2);
		LoadTexture(IDB_STAL, g_Texture3);

		m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
		m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
		m_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
		m_pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);

		BuildProjectionMatrix2();

		ASERT(m_wsk_godz->RestoreDeviceObjects(m_pd3dDevice));
		ASERT(m_wsk_min->RestoreDeviceObjects(m_pd3dDevice));
		if(g_bWskazSek)
			ASERT(m_wsk_sec->RestoreDeviceObjects(m_pd3dDevice));
		ASERT(m_tarcza->RestoreDeviceObjects(m_pd3dDevice));
		ASERT(m_montaz->RestoreDeviceObjects(m_pd3dDevice));

		if(m_bUseDepthBuffer)
		{
			/*ASERT(m_pd3dDevice->CreateVertexBuffer( 4*sizeof(SHADOWVERTEX),
													D3DUSAGE_WRITEONLY, 
													SHADOWVERTEX::FVF,
													D3DPOOL_MANAGED, 
													&m_pBigSquareVB,
													NULL));*/
			ASERT(m_pd3dDevice->CreateVertexBuffer( 4*sizeof(SHADOWVERTEX),
													D3DUSAGE_WRITEONLY, 
													SHADOWVERTEX::FVF,
													D3DPOOL_MANAGED, 
													&m_pBigSquareVB));
			// Set the size of the big square shadow
			SHADOWVERTEX* v;
			FLOAT sx = (FLOAT)m_d3dsdBackBuffer.Width;
			FLOAT sy = (FLOAT)m_d3dsdBackBuffer.Height;
			ASERT(m_pBigSquareVB->Lock( 0, 0, (BYTE **) &v, 0 ));
			v[0].p = D3DXVECTOR4(  0, sy, 0.0f, 1.0f );
			v[1].p = D3DXVECTOR4(  0,  0, 0.0f, 1.0f );
			v[2].p = D3DXVECTOR4( sx, sy, 0.0f, 1.0f );
			v[3].p = D3DXVECTOR4( sx,  0, 0.0f, 1.0f );
			v[0].color = 0x40000000;
			v[1].color = 0x40000000;
			v[2].color = 0x40000000;
			v[3].color = 0x40000000;
			ASERT(m_pBigSquareVB->Unlock());
		}
	}
	catch(CBlad & e)
	{
		e.MsgBox();
		return E_FAIL;
	}

	return S_OK;
}
HRESULT CD3DZegarSaver::InvalidateDeviceObjects() 
{
	FUNC_TRACE;

	if(g_pDzwiek)
		g_pDzwiek->StopAll();
	m_wsk_godz->InvalidateDeviceObjects();
	m_wsk_min->InvalidateDeviceObjects();
	if(g_bWskazSek)
		m_wsk_sec->InvalidateDeviceObjects();
	m_tarcza->InvalidateDeviceObjects();
	m_montaz->InvalidateDeviceObjects();
	SAFE_RELEASE(g_Texture);		
	SAFE_RELEASE(g_Texture2);
	SAFE_RELEASE(g_Texture3);		
	SAFE_RELEASE(m_pBigSquareVB);

	return S_OK;
}
/*	HRESULT InitDeviceObjects()
{
	return S_OK;
}*/
//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CD3DZegarSaver::ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
										D3DFORMAT Format )
{
	// Make sure device supports directional lights
	if( (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING ) ||
		(dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING ) )
	{
		if(! (pCaps->VertexProcessingCaps & D3DVTXPCAPS_DIRECTIONALLIGHTS))
			return E_FAIL;
	}
	if(m_bUseDepthBuffer)
		if(	(pCaps->ZCmpCaps & D3DPCMPCAPS_LESSEQUAL) && // Wymagania obs³ugi cienia czasu rzeczywistego
			(pCaps->ZCmpCaps & D3DPCMPCAPS_GREATEREQUAL) &&
			(pCaps->ZCmpCaps & D3DPCMPCAPS_ALWAYS) &&
			(pCaps->StencilCaps & D3DSTENCILCAPS_DECR) &&
			(pCaps->StencilCaps & D3DSTENCILCAPS_INCR) &&
			(pCaps->StencilCaps & D3DSTENCILCAPS_KEEP))
			m_bTwoSided = true;//((pCaps->StencilCaps & D3DSTENCILCAPS_TWOSIDED) != 0);
		else
			return E_FAIL;
	return S_OK;
}

HRESULT CD3DZegarSaver::OneTimeSceneInit()                       
{
	FUNC_TRACE;

	if(m_SaverMode == sm_preview)
		g_wJakoscGr = MINJAKOSC;
	m_hWndParent = g_hWndParent;

// Direct3D
	// Œwiat³o g³ówne
	ZeroMemory( &light, sizeof(D3DLIGHT8) );
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.76f;
	light.Diffuse.g = 0.76f;
	light.Diffuse.b = 0.71f;
	light.Direction.x = 0.47095346f;
	light.Direction.y = -0.47095346f;
	light.Direction.z = 0.74592602f;
	// Materia³ wskazówek
	D3DUtil_InitMaterial( mtrl, .22f, .22f, .22f );
	D3DUtil_InitMaterial( mtrl_sek, .36f, .30f, .82f );
	// Materia³ - masi¹dz
	D3DUtil_InitMaterial( mtrl_mosiadz, .70f, .70f, .60f );
	// Materia³ - stal
	D3DUtil_InitMaterial( mtrl_stal, .65f, .65f, .70f );
	// Materia³ - tarcza
	D3DUtil_InitMaterial( mtrl_tarcz, 1.0f, 1.0f, 1.0f );

	// Obliczenia podstawowe, definicje figur
	m_tarcza = new CTarcza(
			D3DXVECTOR3(1.0f, 1.0f, WYCOFANIE),
			D3DXVECTOR3(0.0f, 0.0f, TARCZA_Z));

	proporcje_wsk m_prop_godz;
	m_prop_godz.dlugosc_wsk = 0.70f;
	m_prop_godz.dlugosc_krzyza = 0.3f;
	m_prop_godz.szer_krzyza = 0.052f;
	m_prop_godz.szer_prostak = 0.026f;
	m_prop_godz.szer_wsk = 0.132f;
	m_prop_godz.wsk_mtrl = &mtrl;
	m_wsk_godz = new CWskazGodz(m_prop_godz, D3DXVECTOR3(0.0f, 0.0f, GODZ_Z));

	proporcje_wsk m_prop_min;
	m_prop_min.dlugosc_wsk = 0.966f;
	m_prop_min.dlugosc_krzyza = 0.476f;
	m_prop_min.szer_krzyza = 0.045f;
	m_prop_min.szer_prostak = 0.023f;
	m_prop_min.szer_wsk = 0.118f;
	m_prop_min.wsk_mtrl = &mtrl;
	m_wsk_min = new CWskazMin(m_prop_min, D3DXVECTOR3(0.0f, 0.0f, g_bWskazSek ? (GODZ_Z + MIN_Z) / 2 : MIN_Z));

	if(g_bWskazSek)
	{
		proporcje_wsk m_prop_sek;
		m_prop_sek.dlugosc_wsk = 0.966f;
		m_prop_sek.dlugosc_krzyza = 0.477f;
		m_prop_sek.szer_krzyza = 0.040f;
		m_prop_sek.szer_prostak = 0.020f;
		m_prop_sek.szer_wsk = 0.106f;
		m_prop_sek.wsk_mtrl = &mtrl_sek;
		m_wsk_sec = new CWskazSec(m_prop_sek, D3DXVECTOR3(0.0f, 0.0f, MIN_Z));
		g_bWskazSek = m_wsk_sec != NULL;
	}

	proporcje_mutry m_prop_mutr;
	m_prop_mutr.duzy_r = 0.08f;
	m_prop_mutr.wiekszy_r = 0.075f;
	m_prop_mutr.maly_r = 0.05f;
	m_prop_mutr.gwint_r = 0.027f;
	m_prop_mutr.koniec_r = 0.0139f;
	m_montaz = new CMontaz(m_prop_mutr, D3DXVECTOR3(0.0f, 0.0f, STOZEK_Z));
	if(m_bUseDepthBuffer)
	{
		m_montaz->AddPrimitive( );
	}

	// DirectMusic
	if(g_wGlosnosc)
	{
		g_pDzwiek = new CDzwiek();
		if(g_pDzwiek)
			if(FAILED(g_pDzwiek->Create()))
				SAFE_DELETE(g_pDzwiek);
	}

	/*/ Tekstury
	g_Texture = NULL;
	g_Texture2 = NULL;
	g_Texture3 = NULL;*/

	// Inne
	GetLocalTime(&m_poprzedz_czas);

	return S_OK;
}

void CD3DZegarSaver::Zeruj()
{
	// Direct3D
	m_pBigSquareVB = NULL;
	m_tarcza = NULL;
	m_wsk_godz = NULL;
	m_wsk_min = NULL;
	m_wsk_sec = NULL;
	m_montaz = NULL;
	g_Texture = NULL;
	g_Texture2 = NULL;
	g_Texture3 = NULL;		

	// DirectMusic
	g_pDzwiek = NULL;
}
	/*
	SAFE_DELETE(m_tarcza);
	SAFE_DELETE(m_wsk_godz);
	SAFE_DELETE(m_wsk_min);
	SAFE_DELETE(m_montaz);
	SAFE_DELETE(m_pShadowVolume);
 /* Destruktory, które by³y wo³ane dopiero po dezaktywacji zegara zosta³y usuniête,
	    gdy¿ tylko fragmentarycznie zwalania³y pamiêæ */


//-----------------------------------------------------------------------------
// Name: DoConfig()
// Desc: 
//-----------------------------------------------------------------------------
VOID CD3DZegarSaver::DoConfig()
{
	FUNC_TRACE;
	if(SUCCEEDED(MyDoConfig(m_hInstance, m_hWndParent)))
		WriteSettings();
}

// #############################################
// Nazwa: CD3DZegarSaver::Create()
// Opis: Metoda rozszerza dzia³anie wygaszacza,
//       nie przerywa dzia³ania przy braku 
//       DirectX 9
// #############################################
HRESULT CD3DZegarSaver::Create( HINSTANCE hInstance )
{
	FUNC_TRACE;

	HRESULT hr;

	// Tylko do testu na obecnoœæ Bibliotek DirectX 9

	if( DirectX9OK() )
		// Jeœli nie znaleziono DirectX 9 to nale¿y odpowiednie API pomin¹æ.
		// Biblioteka jest zadeklarowana jako "Delayed loaded DLLs".
	{
		
		// Ten kawa³ek tylko dla DirectX8
		TCHAR* pstrCmdLine = GetCommandLine();
		m_SaverMode = ParseCommandLine( pstrCmdLine );
		if(m_SaverMode == sm_config)
		{
			m_dwMinStencilBits = 0;
			m_bUseDepthBuffer = FALSE;
		}
		// Koniec

		hr = CD3DScreensaver::Create(hInstance);
	}
	else
	{

		SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_IDLE );

		m_hInstance = hInstance;

		// Parse the command line and do the appropriate thing
		TCHAR* pstrCmdLine = GetCommandLine();
		m_SaverMode = ParseCommandLine( pstrCmdLine );

		if(m_SaverMode = sm_config)
			m_bUseDepthBuffer = FALSE;

		EnumMonitors();

		// Create the screen saver window(s)
		if( m_SaverMode == sm_preview || 
			m_SaverMode == sm_test    || 
			m_SaverMode == sm_full )
		{
			if( FAILED( hr = CreateSaverWindow() ) )
			{
				m_bErrorMode = TRUE;
				m_hrError = hr;
			}
		}

		if( m_SaverMode == sm_preview )
		{
			// In preview mode, "pause" (enter a limited message loop) briefly 
			// before proceeding, so the display control panel knows to update itself.
			m_bWaitForInputIdle = TRUE;

			// Post a message to mark the end of the initial group of window messages
			PostMessage( m_hWnd, WM_USER, 0, 0 );

			MSG msg;
			while( m_bWaitForInputIdle )
			{
				// If GetMessage returns FALSE, its quitting time.
				if( !GetMessage( &msg, m_hWnd, 0, 0 ) )
				{
					// Post the quit message to handle it later
					PostQuitMessage(0);
					break;
				}

				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}

		m_bErrorMode = TRUE;
		m_hrError = D3DAPPERR_NOCOMPATIBLEDEVICES;

		/*if(m_SaverMode == sm_config)
			hr = D3DAPPERR_NOCOMPATIBLEDEVICES; // Ostrze¿e, ¿e brak odpowiedniego oprogramowania
		else*/
		hr = S_OK; // Musi zadzia³aæ, poniewa¿ mo¿e chroniæ dostêpu has³em
	}

	return hr;
}

// #############################################
// Nazwa: CD3DZegarSaver::ReadSettings()
// Opis: Odczytanie danych z rejestru
// #############################################
VOID CD3DZegarSaver::ReadSettings()
#define POMIEDZY(a, x, y, b) (LOWORD(((a) >= (x) && (a) <= (y)) ? (a) : (b)))
{
	HKEY hKey = NULL;
	if(SUCCEEDED(RegOpenKeyEx(HKEY_CURRENT_USER,
					m_strRegPath,
					0,
					KEY_READ,
					&hKey)))
	{
		DWORD dw_tmp;
		DXUtil_ReadIntRegKey( hKey, REGGLOS, &dw_tmp, g_wGlosnosc );
		g_wGlosnosc = POMIEDZY(dw_tmp, MINGLOS, MAXGLOS, g_wGlosnosc);
		DXUtil_ReadIntRegKey( hKey, REGJAKOSC, &dw_tmp, g_wJakoscGr );
		g_wJakoscGr = POMIEDZY(dw_tmp, MINJAKOSC, MAXJAKOSC, g_wJakoscGr);
		DXUtil_ReadIntRegKey( hKey, REGWIELKOSC, &dw_tmp, g_wWielkosc );
		g_wWielkosc = POMIEDZY(dw_tmp, MINWIELKOSC, MAXWIELKOSC, g_wWielkosc);
		DXUtil_ReadBoolRegKey( hKey, REGSEKUNDA, &g_bWskazSek, g_bWskazSek );

		RegCloseKey(hKey);
	}
}

// #############################################
// Nazwa: CD3DZegarSaver::WriteSettings()
// Opis: Zapisanie danych w rejestrze
// #############################################
VOID CD3DZegarSaver::WriteSettings()
{
	HRESULT hr;
	HKEY hKey = NULL;
	if(SUCCEEDED(RegCreateKeyEx(HKEY_CURRENT_USER, m_strRegPath, 0, "", 0, 
        KEY_WRITE, NULL, &hKey, NULL)))
	{
		hr = DXUtil_WriteIntRegKey( hKey, REGGLOS, g_wGlosnosc );
		hr = DXUtil_WriteIntRegKey( hKey, REGJAKOSC, g_wJakoscGr );
		hr = DXUtil_WriteIntRegKey( hKey, REGWIELKOSC, g_wWielkosc );
		hr = DXUtil_WriteBoolRegKey( hKey, REGSEKUNDA, g_bWskazSek );

		hr = RegCloseKey(hKey);
	}
}

//-----------------------------------------------------------------------------
// Name: GetTextForError()
// Desc: Translate an HRESULT error code into a string that can be displayed
//       to explain the error.  A class derived from CD3DScreensaver can 
//       provide its own version of this function that provides app-specific
//       error translation instead of or in addition to calling this function.
//       This function returns TRUE if a specific error was translated, or
//       FALSE if no specific translation for the HRESULT was found (though
//       it still puts a generic string into pszError).
//-----------------------------------------------------------------------------
BOOL CD3DZegarSaver::GetTextForError( HRESULT hr, TCHAR* pszError, 
                                       DWORD dwNumChars )
{
    const DWORD dwErrorMap[][2] = 
    {
    //  HRESULT, stringID
        (DWORD)E_FAIL, IDS_ERR_GENERIC,
        D3DAPPERR_NODIRECT3D, IDS_ERR_NODIRECT3D,
        D3DAPPERR_NOWINDOWEDHAL, IDS_ERR_NOWINDOWEDHAL,
        D3DAPPERR_CREATEDEVICEFAILED, IDS_ERR_CREATEDEVICEFAILED,
        D3DAPPERR_NOCOMPATIBLEDEVICES, IDS_ERR_NOCOMPATIBLEDEVICES,
        D3DAPPERR_NOHARDWAREDEVICE, IDS_ERR_NOHARDWAREDEVICE,
        D3DAPPERR_HALNOTCOMPATIBLE, IDS_ERR_HALNOTCOMPATIBLE,
        D3DAPPERR_NOHALTHISMODE, IDS_ERR_NOHALTHISMODE,   
        D3DAPPERR_MEDIANOTFOUND, IDS_ERR_MEDIANOTFOUND,   
        D3DAPPERR_RESIZEFAILED, IDS_ERR_RESIZEFAILED,    
        (DWORD)E_OUTOFMEMORY, IDS_ERR_OUTOFMEMORY,     
        (DWORD)D3DERR_OUTOFVIDEOMEMORY, IDS_ERR_OUTOFVIDEOMEMORY,
        D3DAPPERR_NOPREVIEW, IDS_ERR_NOPREVIEW
    };
    const DWORD dwErrorMapSize = sizeof(dwErrorMap) / sizeof(DWORD[2]);

    DWORD iError;
    DWORD resid = 0;

    for( iError = 0; iError < dwErrorMapSize; iError++ )
    {
        if( hr == (HRESULT)dwErrorMap[iError][0] )
        {
            resid = dwErrorMap[iError][1];
        }
    }
    if( resid == 0 )
    {
        resid = IDS_ERR_GENERIC;
    }

    LoadString( NULL, resid + Jezyk(), pszError, dwNumChars );

    if( resid == IDS_ERR_GENERIC )
        return FALSE;
    else
        return TRUE;
}


bool CreateScrnSvr(bool pBool)
{
	Zegar3D = new CD3DZegarSaver(pBool);
	return Zegar3D != NULL;
}
// #############################################
// Nazwa: _tWinMain
// Opis: Uruchominie wygaszacza nastêpuje
//       przez tê funkcje
// #############################################
int APIENTRY _tWinMain(	HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPTSTR    lpCmdLine,
						int       nCmdShow)
{
	HRESULT hr;

	if(!CreateScrnSvr(true))
		return E_FAIL;
#ifndef _DEBUG
	__try
	{		
#endif
		if( FAILED( hr = Zegar3D->Create( hInstance ) ) )
		{
			Zegar3D->DisplayErrorMsg( hr );
			return 0;
		}
		if(Zegar3D->GetConfigErrorMode())
		{
			Zegar3D->DisplayErrorMsg( Zegar3D->Get_hrError() );
			return 0;
		}
		hr = Zegar3D->Run();
		if(	FAILED(Zegar3D->Get_hrError()) )
		{
			SAFE_DELETE(Zegar3D);
			if(!CreateScrnSvr(false))
				return E_FAIL;

			if( FAILED( hr = Zegar3D->Create( hInstance ) ) )
			{
				Zegar3D->DisplayErrorMsg( hr );
				return 0;
			}
			return Zegar3D->Run();
		}
		else
			return hr;
#ifndef _DEBUG
	}
	__except(1)
	{
		if(g_hWndParent)
			RedrawWindow(g_hWndParent, 
				NULL, NULL,
				RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
		HKEY hKey;
		if(SUCCEEDED(RegCreateKeyEx(HKEY_CURRENT_USER, g_reg, 0, "", 0, 
			KEY_WRITE, NULL, &hKey, NULL)))
		{
			time_t ltime;
			struct tm *thetime;
			TCHAR str[100];

			time (&ltime);
			thetime = gmtime(&ltime);

			if (_tcsftime((char *)str, 100, "; %Y-%m-%d %H:%M, %a", 
							(const struct tm *)thetime))
				_tcscat(g_blad, str);
			else
				_tcscat(g_blad, _T("?"));
			hr = DXUtil_WriteStringRegKey( hKey, REGBLAD, g_blad );
			hr = RegCloseKey(hKey);
		}
	}
#endif
	return 0;
}
