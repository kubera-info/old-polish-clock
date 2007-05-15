// Zegar.cpp : Defines the entry point for the application.
//

// MAIN
#include "stdafx.h"
#include <Windowsx.h>
#include <time.h>
#include "D3D/dxutil.h"
#include "Konfig.h"
#include "tarcza.h"
#include "wskaz.h"
#include "nakretka.h"
#include "dzwiek.h"
#include "Shadows.h"
#include "Zegar.h"

// ##############################################
// Nazwa: class D3DZegarSaver
// Opis: G³ówna funkcjonalnoœæ wygaszacza ekranu.
// ##############################################
namespace D3DSaver
{
	RECT rc;

	//Direct3D
	D3DLIGHT9 m_tmp_light;
	float m_fSwiatlo;
	bool m_bWaitForInputIdle;
	CKombajnGraf * WszystkieObiekty = NULL;
	CShadowVolume * m_cien = NULL;
	// Inne
	SYSTEMTIME g_aktual_czas;

	// Macierze przekszta³ceñ 3D (skalowania i translacji)
	void BuildProjectionMatrix2( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
	{
		// Pierwszy etap: wspó³czynniki logiczne pikseli
		// FLOAT l,r,t,b,
		FLOAT dx,dy;

		dx = (float) pBackBufferSurfaceDesc->Width;
		dy = (float) pBackBufferSurfaceDesc->Height;

		// Etap drugi: wspó³czynniki monitora (gêstoœæ i proporcja pikseli)
		/*D3DDEVICE_CREATION_PARAMETERS Parameters;
		pd3dDevice->GetCreationParameters(  &Parameters );
		HMONITOR hMonitorAktual = DXUTGetD3DObject()->GetAdapterMonitor(Parameters.AdapterOrdinal);
		MONITORINFOEX OpisMonitora;
		ZeroMemory((void *) &OpisMonitora, sizeof(OpisMonitora));
		OpisMonitora.cbSize = sizeof(OpisMonitora);
		if(GetMonitorInfo(hMonitorAktual, &OpisMonitora))
		{
		HDC Kontekst = CreateDC(L"DISPLAY", OpisMonitora.szDevice, NULL, NULL);
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
		}*/

		// Etap czwarty: macierze charakteryzuj¹ce przestrzeñ.
#define PROJECTION_MATRIX (m_matrix)
#define VIEW_MATRIX (m_matrix)
		D3DXMATRIXA16 m_matrix;
		D3DXVECTOR3 vEyePt( 0.0f, 0.0f, 2.7f );
		//D3DXVECTOR3 vEyePt( 1.0f, 0.0f, 2.7f );
		D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMatrixLookAtLH( &VIEW_MATRIX, &vEyePt, &vLookatPt, &vUpVec );
		pd3dDevice->SetTransform(D3DTS_VIEW, &VIEW_MATRIX);

		// For the projection matrix, we set up a perspective transform (which
		// transforms geometry from 3D view space to 2D viewport space, with
		// a perspective divide making objects smaller in the distance). To build
		// a perpsective transform, we need the field of view (1/4 pi is common),
		// the aspect ratio, and the near and far clipping planes (which define at
		// what distances geometry should be no longer be rendered).
		D3DXMatrixPerspectiveFovLH( &PROJECTION_MATRIX, D3DX_PI/4, dx / dy, 1.0f, ZFAR );
		pd3dDevice->SetTransform(D3DTS_PROJECTION, &PROJECTION_MATRIX);

		D3DXMatrixIdentity(&g_world);

		float skala_wsp = 0.374f;
		D3DXMATRIXA16 translacja, skala;
		D3DXMatrixTranslation(&translacja, -0.028f, -0.57f, 0.0f);
		D3DXMatrixScaling(&skala, skala_wsp, skala_wsp, skala_wsp);
		D3DXMatrixMultiply( &g_small_world, &translacja, &skala);

		//		const static float m_skala = ((float) g_wWielkosc) / 10000;
		//pd3dDevice->SetTransform(D3DTS_WORLD, &g_world); -- by default
	}

	//
	// Funkcje typu CALLBACK
	//

	//--------------------------------------------------------------------------------------
	// Returns true if a particular depth-stencil format can be created and used with
	// an adapter format and backbuffer format combination.
	BOOL IsDepthFormatOk( D3DFORMAT DepthFormat,
		D3DFORMAT AdapterFormat,
		D3DFORMAT BackBufferFormat )
	{
		FUNC_TRACE;

		// Verify that the depth format exists
		HRESULT hr = DXUTGetD3DObject()->CheckDeviceFormat( D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			AdapterFormat,
			D3DUSAGE_DEPTHSTENCIL,
			D3DRTYPE_SURFACE,
			DepthFormat );
		if( FAILED( hr ) ) return FALSE;

		// Verify that the backbuffer format is valid
		hr = DXUTGetD3DObject()->CheckDeviceFormat( D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			AdapterFormat,
			D3DUSAGE_RENDERTARGET,
			D3DRTYPE_SURFACE,
			BackBufferFormat );
		if( FAILED( hr ) ) return FALSE;

		// Verify that the depth format is compatible
		hr = DXUTGetD3DObject()->CheckDepthStencilMatch( D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			AdapterFormat,
			BackBufferFormat,
			DepthFormat );

		return SUCCEEDED(hr);

	}

	//--------------------------------------------------------------------------------------
	// Called during device initialization, this code checks the device for some 
	// minimum set of capabilities, and rejects those that don't pass by returning false.
	//--------------------------------------------------------------------------------------

	bool    CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
	{
		FUNC_TRACE;

		if( pCaps->DeviceType & D3DDEVTYPE_REF )
			return false;

		// Must support stencil buffer
		if( !IsDepthFormatOk( D3DFMT_D24S8,
			AdapterFormat,
			BackBufferFormat ) &&
			!IsDepthFormatOk( D3DFMT_D24X4S4,
			AdapterFormat,
			BackBufferFormat ) &&
			!IsDepthFormatOk( D3DFMT_D15S1,
			AdapterFormat,
			BackBufferFormat ) &&
			!IsDepthFormatOk( D3DFMT_D24FS8,
			AdapterFormat,
			BackBufferFormat ) )
			return false;

		return (
			( pCaps->ZCmpCaps & D3DPCMPCAPS_LESSEQUAL ) && // Wymagania obs³ugi cienia czasu rzeczywistego
			( pCaps->ZCmpCaps & D3DPCMPCAPS_ALWAYS ) &&
			( pCaps->StencilCaps & D3DSTENCILCAPS_DECR ) &&
			( pCaps->StencilCaps & D3DSTENCILCAPS_INCR ) &&
			( pCaps->StencilCaps & D3DSTENCILCAPS_KEEP ) );
	}

	bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
	{

		g_bTwoSided = ( ( pCaps->StencilCaps & D3DSTENCILCAPS_TWOSIDED ) != 0 );

		// This sample requires a stencil buffer.
		if( IsDepthFormatOk( D3DFMT_D24S8,
			pDeviceSettings->AdapterFormat,
			pDeviceSettings->pp.BackBufferFormat ) )
			pDeviceSettings->pp.AutoDepthStencilFormat = D3DFMT_D24S8;
		else
			if( IsDepthFormatOk( D3DFMT_D24X4S4,
				pDeviceSettings->AdapterFormat,
				pDeviceSettings->pp.BackBufferFormat ) )
				pDeviceSettings->pp.AutoDepthStencilFormat = D3DFMT_D24X4S4;
			else
				if( IsDepthFormatOk( D3DFMT_D24FS8,
					pDeviceSettings->AdapterFormat,
					pDeviceSettings->pp.BackBufferFormat ) )
					pDeviceSettings->pp.AutoDepthStencilFormat = D3DFMT_D24FS8;
				else
					if( IsDepthFormatOk( D3DFMT_D15S1,
						pDeviceSettings->AdapterFormat,
						pDeviceSettings->pp.BackBufferFormat ) )
						pDeviceSettings->pp.AutoDepthStencilFormat = D3DFMT_D15S1;
		if( (pDeviceSettings->DeviceType == D3DDEVTYPE_REF))
			return false;

		return true;
	}

	void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
	{
		FUNC_TRACE;

		Sleep(10);
		GetLocalTime(&g_aktual_czas);

#ifndef	LUMINACJA
		m_fSwiatlo =
			(1.0f + cosf(D3DX_PI * 2 * ((float) g_aktual_czas.wSecond + (float) g_aktual_czas.wMilliseconds / 1000) / 60.0f)) / 2.0f;
#else
		m_fSwiatlo = 1.0f;
#endif
		m_tmp_light = light;
		m_tmp_light.Diffuse.r *= m_fSwiatlo;
		m_tmp_light.Diffuse.g *= m_fSwiatlo;
		m_tmp_light.Diffuse.b *= m_fSwiatlo;

		WszystkieObiekty->OnFrameMove( );
	}

	void    CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
	{
		FUNC_TRACE;

		pd3dDevice->Clear( 
			0L,
			NULL, 
			D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL,
			D3DCOLOR_XRGB((UCHAR)(10 * m_fSwiatlo), (UCHAR)(10 * m_fSwiatlo), (UCHAR)(100 * m_fSwiatlo)),
			1.0f, 
			0L );
		// Begin the scene.
		if( SUCCEEDED( pd3dDevice->BeginScene() ) ) // Jeœli ta funkcja wykona poprawn¹ operacjê to w zasadzie ryzyko jest ma³e
		{
			pd3dDevice->SetLight( 0, &m_tmp_light );
			pd3dDevice->LightEnable( 0, TRUE);
			pd3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB((UCHAR)(0x52 * m_fSwiatlo), (UCHAR)(0x52 * m_fSwiatlo), (UCHAR)(0x49 * m_fSwiatlo)));

			WszystkieObiekty->OnFrameRender( pd3dDevice );

			// End the scene.
			pd3dDevice->EndScene();
		}
	}

	HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
	{
		FUNC_TRACE;
		HRESULT hr;

		BuildProjectionMatrix2( pd3dDevice, pBackBufferSurfaceDesc );
		V_RETURN (pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA ));
		V_RETURN (pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA ));
		V_RETURN (pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ));
		V_RETURN (pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ));
		V_RETURN (pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE ));
		V_RETURN (pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ));
		V_RETURN (pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ));
		V_RETURN (pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE ));
		V_RETURN (pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE ) );
		V_RETURN (pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE ) );
		V_RETURN (pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE ) );
		V_RETURN (pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE) );
		V_RETURN (pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ) );

		if(m_cien)
			V_RETURN( m_cien->OnCreateDevice( pd3dDevice, pBackBufferSurfaceDesc ) );

		return S_OK;
	}

	void    CALLBACK OnD3D9LostDevice( void* pUserContext )
	{
		FUNC_TRACE;

		if(m_cien)
			m_cien->OnDestroyDevice();
	}

	void    CALLBACK OnD3D9DestroyDevice( void* pUserContext )
	{
		FUNC_TRACE;

		WszystkieObiekty->OnDestroyDevice();
	}

	//-----------------------------------------------------------------------------
	// Name: OnCreateDevice()
	// Desc: Called during device intialization, this code checks the device
	//       for some minimum set of capabilities
	//-----------------------------------------------------------------------------
	HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
	{
		FUNC_TRACE;
		HRESULT hr;

		WszystkieObiekty = new CKombajnGraf();
		if(NULL == WszystkieObiekty)
			return E_OUTOFMEMORY;

		class CFaktura * TarczaFaktura = new CFaktura(1.0f, 1.0f, 1.0f, IDB_TARCZA, D3DFMT_L8);
		class CFaktura * MosiadzFaktura = new CFaktura(0.7f, 0.7f, 0.6f, IDB_MOSIADZ);
		class CFaktura * StalFaktura = new CFaktura(0.65f, 0.65f, 0.70f, IDB_STAL);
		class CMaterial * WskazowkaFaktura = new CMaterial(0.22f, 0.22f, 0.22f);
		class CMaterial * NiewidocznaFaktura = new CMaterial(0.0f, 0.0f, 0.0f, 0.0f);
		class CFaktura * OzdobaFaktura = new CFaktura(1.0f, 1.0f, 1.0f, IDB_ORNAMENT);

		class COzdoba * m_ozdoba = new COzdoba();
		class CCone * m_cone = new CCone();
		class CTarcza * m_tarcza = new CTarcza();

		class CWskazGodz * m_wsk_godz = new CWskazGodz();
		class CWskazMin * m_wsk_min = new CWskazMin();

		class CMontazMosiadz * m_montazMosiadz = new CMontazMosiadz();
		class CMontazStal * m_montazStal = new CMontazStal();
		class CMacierz * m_macierz = new CMacierz(&g_world);
		class CMacierz * m_macierz_mala = new CMacierz(&g_small_world);

		class CSzklo * m_szklo = new CSzklo();
		m_cien = new CShadowVolume(WszystkieObiekty);

		class CDzwiek * m_pDzwiek = NULL;

		// Direct3D
		// Œwiat³o
		ZeroMemory( &light, sizeof(D3DLIGHT9) );
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = 0.76f;
		light.Diffuse.g = 0.76f;
		light.Diffuse.b = 0.74f;
		light.Direction.x = -0.47095346f;
		light.Direction.y = -0.47095346f;
		light.Direction.z = 0.74592602f;

		// Obliczenia podstawowe, definicje figur
		WszystkieObiekty->AddRenderedObject(m_macierz);
		WszystkieObiekty->AddRenderedObject(OzdobaFaktura);
		WszystkieObiekty->AddRenderedObject(m_ozdoba);
		WszystkieObiekty->AddRenderedObject(NiewidocznaFaktura);
		WszystkieObiekty->AddRenderedObject(m_cone);
		WszystkieObiekty->AddRenderedObject(m_macierz_mala);
		WszystkieObiekty->AddRenderedObject(TarczaFaktura);
		WszystkieObiekty->AddRenderedObject(m_tarcza);
		WszystkieObiekty->AddRenderedObject(WskazowkaFaktura);
		WszystkieObiekty->AddRenderedObject(m_wsk_godz);
		WszystkieObiekty->AddRenderedObject(m_wsk_min);
		if(g_bWskazSek)
		{
			class CMaterial * SekundowaFaktura = new CMaterial(0.36f, 0.30f, 0.82f);
			WszystkieObiekty->AddRenderedObject(SekundowaFaktura);
			class CWskazSec * m_wsk_sec = new CWskazSec();
			WszystkieObiekty->AddRenderedObject(m_wsk_sec);
		}
		WszystkieObiekty->AddRenderedObject(m_macierz_mala);
		WszystkieObiekty->AddRenderedObject(MosiadzFaktura);
		WszystkieObiekty->AddRenderedObject(m_montazMosiadz);
		WszystkieObiekty->AddRenderedObject(StalFaktura);
		WszystkieObiekty->AddRenderedObject(m_montazStal);
		WszystkieObiekty->AddRenderedObject(m_macierz);
		WszystkieObiekty->AddRenderedObject(TarczaFaktura);
		WszystkieObiekty->AddRenderedObject(m_szklo);
		if(g_wGlosnosc)
			WszystkieObiekty->AddRenderedObject(new CDzwiek(DXUTGetHWND()));

		V_RETURN (WszystkieObiekty->OnCreateDevice(pd3dDevice, pBackBufferSurfaceDesc));

		WszystkieObiekty->AddRenderedObject(m_cien);

		// Inne
		return S_OK;
	}

	/* Destruktory, które by³y wo³ane dopiero po dezaktywacji zegara zosta³y usuniête,
	gdy¿ tylko fragmentarycznie zwalania³y pamiêæ */

	// ###################################
	// Nazwa: ReadSettings()
	// Opis: Odczytanie danych z rejestru.
	//       W przypadku b³êdnych wartoœci
	//       dobierane s¹ wsp. domyœlne.
	// ###################################
	HRESULT ReadSettings()
#define POMIEDZY(a, x, y, b) (LOWORD(((a) >= (x) && (a) <= (y)) ? (a) : (b)))
	{
		FUNC_TRACE;
		HRESULT hr;

		HKEY hKey = NULL;
		V (RegOpenKeyEx(HKEY_CURRENT_USER,
			g_strRegPath,
			0,
			KEY_READ,
			&hKey));
		if( FAILED (hr) )
		{
			DXTRACE_ERR(L"Error opening registry handle", hr);
			return S_OK;
		}
		DWORD dw_tmp;

		V (DXUtil_ReadIntRegKey( hKey, REGGLOS, &dw_tmp, g_wGlosnosc ));
		if( FAILED( hr ) )
			DXTRACE_ERR(L"Error reading registry", hr);
		g_wGlosnosc = POMIEDZY(dw_tmp, MINGLOS, MAXGLOS, g_wGlosnosc);

		V (DXUtil_ReadBoolRegKey( hKey, REGSEKUNDA, &g_bWskazSek, g_bWskazSek ))
			if( FAILED( hr ) )
				DXTRACE_ERR(L"Error reading registry", hr);

		V (DXUtil_ReadBoolRegKey( hKey, REGOPADANIE, &g_bOpadanie, g_bOpadanie ))
			if( FAILED( hr ) )
				DXTRACE_ERR(L"Error reading registry", hr);		

		V (RegCloseKey(hKey));

		if ( FAILED( hr ) )
			DXTRACE_ERR(L"Error closing registry handle", hr);

		return S_OK;
	}

	// ###################################
	// Nazwa: WriteSettings()
	// Opis: Zapisanie danych w rejestrze.
	// ###################################
	HRESULT WriteSettings()
	{
		FUNC_TRACE;
		HRESULT hr;
		HKEY hKey = NULL;

		V_RETURN(DXTRACE_ERR(L"Error opening registry handle", RegCreateKeyEx(HKEY_CURRENT_USER, g_strRegPath, 0, L"", 0,
			KEY_WRITE, NULL, &hKey, NULL)));
		DXTRACE_ERR(L"Error modifing registry", DXUtil_WriteIntRegKey( hKey, REGGLOS, g_wGlosnosc ));
		DXTRACE_ERR(L"Error modifing registry", DXUtil_WriteBoolRegKey( hKey, REGSEKUNDA, g_bWskazSek ));
		DXTRACE_ERR(L"Error modifing registry", DXUtil_WriteBoolRegKey( hKey, REGOPADANIE, g_bOpadanie ));
		V_RETURN(DXTRACE_ERR(L"Error reading registry", RegCloseKey(hKey)));

		return S_OK;
	}

	//--------------------------------------------------------------------------------------
	void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
	{
		DXUTShutdown();
	}
	void    CALLBACK MouseProc( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext )
	{
		DXUTShutdown();
	}

	LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
	{
		if(uMsg == WM_MOUSEMOVE)
		{
			if(g_SaverMode != sm_preview && g_SaverMode != sm_test)
			{
				static INT xPrev = -1;
				static INT yPrev = -1;
				static int m_dwSaverMouseMoveCount = 0;
				INT xCur = GET_X_LPARAM(lParam);
				INT yCur = GET_Y_LPARAM(lParam);

				if( xCur != xPrev || yCur != yPrev )
				{
					xPrev = xCur;
					yPrev = yCur;
					m_dwSaverMouseMoveCount++;
					if ( m_dwSaverMouseMoveCount > 5 )
						DXUTShutdown();
				}
				*pbNoFurtherProcessing = true;
			}
		}
		else if((uMsg == WM_ACTIVATEAPP && wParam == FALSE)
			||(uMsg == WM_POWERBROADCAST && wParam == PBT_APMSUSPEND))
		{
			DXUTShutdown();
			*pbNoFurtherProcessing = true;
		}
		else if(uMsg == WM_SYSCOMMAND &&
			g_SaverMode != sm_test &&
			(wParam == SC_NEXTWINDOW
			||wParam == SC_PREVWINDOW
			||wParam == SC_SCREENSAVE
			||wParam == SC_CLOSE)
			/* || uMsg == WM_PAINT */)
			*pbNoFurtherProcessing = true;
		else if(uMsg == WM_USER)
		{
			// All initialization messages have gone through.  Allow
			// 500ms of idle time, then proceed with initialization.
			SetTimer( hWnd, 1, 500, NULL );
			*pbNoFurtherProcessing = true;
		}
		else if(uMsg == WM_TIMER)
		{
			// Initial idle time is done, proceed with initialization.
			m_bWaitForInputIdle = FALSE;
			KillTimer( hWnd, 1 );
			*pbNoFurtherProcessing = true;
		}

		return 0;
	}

	HRESULT DoConfig( const HINSTANCE hInstance )
	{
		FUNC_TRACE;
		HRESULT hr;

		if(IDOK == MyDoConfig(hInstance, g_hWndParent))
			V_RETURN(WriteSettings());

		return S_OK;
	}

	void SetCallbacks( void )
	{
		DXUTSetCallbackDeviceCreated( OnD3D9CreateDevice );
		DXUTSetCallbackDeviceReset( OnD3D9ResetDevice );
		DXUTSetCallbackDeviceLost( OnD3D9LostDevice );
		DXUTSetCallbackDeviceDestroyed( OnD3D9DestroyDevice );
		DXUTSetCallbackFrameRender( OnD3D9FrameRender );

		DXUTSetCallbackFrameMove( OnFrameMove );
		DXUTSetCallbackMsgProc( MsgProc );
		DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
	}

	HRESULT DoSaver( const HINSTANCE hInstance, const HWND DefWnd = 0)
	{
		FUNC_TRACE;
		HRESULT hr;

		// Initialize the sample framework and create the desired Win32 window and Direct3D 
		// device for the application. Calling each of these functions is optional, but they
		// allow you to set several options which control the behavior of the framework.

#ifdef _DEBUG
		DXUTInit( false, false, NULL ); // Parse the command line, handle the default hotkeys, and show msgboxes
#else
		DXUTInit( false, false, NULL ); // Parse the command line, handle the default hotkeys, and show msgboxes
#endif
		if(DefWnd != 0)
		{
			GetClientRect(DefWnd, &rc);
			// In preview mode, "pause" (enter a limited message loop) briefly 
			// before proceeding, so the display control panel knows to update itself.
			m_bWaitForInputIdle = TRUE;

			// Post a message to mark the end of the initial group of window messages
			PostMessage( DefWnd, WM_USER, 0, 0 );
			MSG msg;
			while( m_bWaitForInputIdle )
			{
				// If GetMessage returns FALSE, it's quitting time.
				if( !GetMessage( &msg, DefWnd, 0, 0 ) )
				{
					// Post the quit message to handle it later
					PostQuitMessage(0);
					break;
				}

				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}

			V_RETURN(DXUTSetWindow(DefWnd, DefWnd, DefWnd, false));
		}
		else
		{
			GetClientRect(GetDesktopWindow(), &rc);
			V_RETURN (DXUTCreateWindow(MiniWinApi::LoadStringI18N(I18N_TYTUL), hInstance, LoadIcon(hInstance, MAKEINTRESOURCE( IDI_SMALL ) ) ) );
		}
#if defined(_DEBUG)
		V_RETURN( 
			DXUTCreateDevice( 
			0,
			true, 
			rc.right - rc.left,
			rc.bottom - rc.top,
			IsD3D9DeviceAcceptable,
			ModifyDeviceSettings));
#else
		V_RETURN( 
			DXUTCreateDevice(0,
			DefWnd != 0, 
			rc.right - rc.left,
			rc.bottom - rc.top,
			IsD3D9DeviceAcceptable,
			ModifyDeviceSettings));
#endif
		// Pass control to the sample framework for handling the message pump and 
		// dispatching render calls. The sample framework will call your FrameMove 
		// and FrameRender callback when there is idle time between handling window messages.
		DXUTMainLoop();

		// Perform any application-level cleanup here. Direct3D device resources are released within the
		// appropriate callback functions and therefore don't require any cleanup code here.

		return DXUTGetExitCode();
	}

} // namespace D3DSaver

// ######################################
// Nazwa: _tWinMain
// Opis: Uruchominie wygaszacza nastêpuje
//       przez tê funkcje.
// ######################################
int APIENTRY InternalWinMain(	HINSTANCE hInstance,
							 HINSTANCE hPrevInstance,
							 LPSTR     lpCmdLine,
							 int       nCmdShow)
{
	HRESULT hr;
	BOOL bUnused;

#ifndef _DEBUG
	__try
	{		
#endif
		SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_IDLE );
		g_SaverMode = ParseCommandLine( GetCommandLine() );
		V_RETURN(D3DSaver::ReadSettings());
		D3DSaver::SetCallbacks();

		switch(g_SaverMode)
		{
		case sm_config :
			return D3DSaver::DoConfig( hInstance );
			break;
		case sm_full :
			// Flag as screensaver running if in full on mode
			SystemParametersInfo( SPI_SCREENSAVERRUNNING, TRUE, &bUnused, 0 );
			DXUTSetCallbackKeyboard( D3DSaver::KeyboardProc );
			DXUTSetCallbackMouse( D3DSaver::MouseProc );
			hr = D3DSaver::DoSaver( hInstance );
			if(!SystemParametersInfo( SPI_SCREENSAVERRUNNING, FALSE, &bUnused, 0 ))
				DXTRACE_ERR( TEXT("SystemParametersInfo failed"), GetLastError() );
			return hr;
			break;
		case sm_preview :
			{
				WNDCLASS    cls;
				cls.hCursor        = LoadCursor( NULL, IDC_ARROW );
				cls.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL ) ); 
				cls.lpszMenuName   = NULL;
				cls.lpszClassName  = SAVERWNDCLASS;
				cls.hbrBackground  = (HBRUSH) GetStockObject(BLACK_BRUSH);
				cls.hInstance      = hInstance; 
				cls.style          = CS_VREDRAW|CS_HREDRAW;
				cls.lpfnWndProc    = DXUTStaticWndProc;
				cls.cbWndExtra     = 0; 
				cls.cbClsExtra     = 0; 
				RegisterClass( &cls );

				const DWORD dwStyle = WS_VISIBLE | WS_CHILD;

				if(GetClientRect( g_hWndParent, &D3DSaver::rc ))
				{
					AdjustWindowRect( &D3DSaver::rc, dwStyle, FALSE );
					const HWND m_hWnd = CreateWindow( SAVERWNDCLASS, MiniWinApi::LoadStringI18N(I18N_TYTUL), dwStyle, 
						D3DSaver::rc.left, D3DSaver::rc.top, 
						D3DSaver::rc.right-D3DSaver::rc.left, D3DSaver::rc.bottom-D3DSaver::rc.top, 
						g_hWndParent, NULL, hInstance, NULL );
					if(m_hWnd)
						return D3DSaver::DoSaver( hInstance, m_hWnd );
					else
						return E_INVALIDARG;
				}
				else
					return E_INVALIDARG;
			}
		case sm_test :
			DXUTSetCallbackKeyboard( D3DSaver::KeyboardProc );
			DXUTSetCallbackMouse( D3DSaver::MouseProc );
			hr = D3DSaver::DoSaver( hInstance );
			return hr;
			break;
		default :
			return E_INVALIDARG;
		}
#ifndef _DEBUG
	}
	__except(1)
	{
		if(g_hWndParent)
			RedrawWindow(g_hWndParent, 
			NULL, NULL,
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
		//		HRESULT hr;
		//		HKEY hKey;
		if(g_SaverMode == sm_full)
			if(!SystemParametersInfo( SPI_SCREENSAVERRUNNING, FALSE, &bUnused, 0 ))
				DXTRACE_ERR( TEXT("SystemParametersInfo failed"), GetLastError() );
		/*		if(SUCCEEDED(RegCreateKeyEx(HKEY_CURRENT_USER, g_reg, 0, L"", 0, 
		KEY_WRITE, NULL, &hKey, NULL)))
		{
		time_t ltime;
		struct tm *thetime;
		TCHAR str[100];

		time (&ltime);
		thetime = gmtime(&ltime);

		if (wcsftime((TCHAR *)str, 100, L"; %Y-%m-%d %H:%M, %a",
		(const struct tm *)thetime))
		StringCbCat(g_blad, sizeof(g_blad) * sizeof(TCHAR), str);
		else
		StringCbCat(g_blad, sizeof(g_blad) * sizeof(TCHAR), TEXT("?"));
		hr = DXUtil_WriteStringRegKey( hKey, REGBLAD, g_blad );
		hr = RegCloseKey(hKey);
		}*/
		return EXIT_FAILURE;
	}
#endif
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	HRESULT hr;

#if defined (ENCRYPT_PLIKI)
	/*	WCHAR p [MAX_PATH];
	GetCurrentDirectory(
	MAX_PATH,
	p
	);
	ExitProcess(0);
	*/
	class ResourceCrypt ResCr1x("..\\Zasoby\\face.x", "..\\Zasoby\\Zaszyfrowane\\face.x", IDX_FACE);
	class ResourceCrypt ResCr2x("..\\Zasoby\\steel.x", "..\\Zasoby\\Zaszyfrowane\\steel.x", IDX_STEEL);
	class ResourceCrypt ResCr3x("..\\Zasoby\\brass.x", "..\\Zasoby\\Zaszyfrowane\\brass.x", IDX_BRASS);
	class ResourceCrypt ResCr4x("..\\Zasoby\\hour.x", "..\\Zasoby\\Zaszyfrowane\\hour.x", IDX_HOUR_HAND);
	class ResourceCrypt ResCr5x("..\\Zasoby\\min.x", "..\\Zasoby\\Zaszyfrowane\\min.x", IDX_MIN_HAND);
	class ResourceCrypt ResCr6x("..\\Zasoby\\sec.x", "..\\Zasoby\\Zaszyfrowane\\sec.x", IDX_SEC_HAND);
	class ResourceCrypt ResCr7x("..\\Zasoby\\ornament.x", "..\\Zasoby\\Zaszyfrowane\\ornament.x", IDX_ORNAMENT);
	class ResourceCrypt ResCr8x("..\\Zasoby\\glass.x", "..\\Zasoby\\Zaszyfrowane\\glass.x", IDX_GLASS);
	class ResourceCrypt ResCr9x("..\\Zasoby\\cone.x", "..\\Zasoby\\Zaszyfrowane\\cone.x", IDX_CONE);

	class ResourceCrypt ResCr1png("..\\Zasoby\\mosiadz.png", "..\\Zasoby\\Zaszyfrowane\\mosiadz.png", IDB_MOSIADZ);
	class ResourceCrypt ResCr2png("..\\Zasoby\\stal.png", "..\\Zasoby\\Zaszyfrowane\\stal.png", IDB_STAL);
	class ResourceCrypt ResCr3png("..\\Zasoby\\tarcza.png", "..\\Zasoby\\Zaszyfrowane\\tarcza.png", IDB_TARCZA);
	class ResourceCrypt ResCr4png("..\\Zasoby\\ozdoba_nowa.png", "..\\Zasoby\\Zaszyfrowane\\ozdoba_nowa.png", IDB_ORNAMENT);

	//class ResourceCrypt ResCr3wav("..\\Zasoby\\gong.wav", "..\\Zasoby\\Zaszyfrowane", IDR_GONG);
	//class ResourceCrypt ResCr4wav("..\\Zasoby\\tik.wav", "..\\Zasoby\\Zaszyfrowane", IDR_TIK);

	::MessageBox(0, TEXT("Szyfrowanie udane, zabierz pliki"), TEXT("*"), 0);
	return 0;
#endif;
	hr = InternalWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	/*for(int i = 0; i < CreatedTemps.GetSize(); i ++)
	hr = SUCCEEDED(hr) ? DeleteFile(CreatedTemps.GetAt(i)) : hr;*/
	return hr;
}