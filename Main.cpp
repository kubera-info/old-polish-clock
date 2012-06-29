//--------------------------------------------------------------------------------------
// File: Main.cpp
//
// Copyright (c) 2009-2012 by kubera.info. All rights reserved.
//--------------------------------------------------------------------------------------

#include "DXUT.h"
#include <WindowsX.h>
#include <dwmapi.h>
#include <Dbt.h>
#include "MetaDefine.h"
#include "MonadException.h"
#include "MonadTime.h"
#include "Singleton.h"
#include "System.h"
#include "MonadAudio.h"
#include "FileManager.h"
#include "Path.h"
#include "World.h"
#include "OldPolishClock.h"
#include "Light.h"
#include "d3dsaver.h"
#include "DictOPC.h"
#include "MonadCatch.h"
#include "MonadFile.h"
#include "MonadUniqueInstance.h"
#include "OPC-ConfigDialog.h"
#include "MonadMonitor.h"
#include "MonadWindow.h"
#include "MonadLibrary.h"

#ifdef DEBUG
// Model Building
//#include "BirdCore.h"
//#include "CasingCore.h"
//#include "WallCore.h"
//#include "PaintingCore.h"
//#include "NutCore.h"
//#include "OrnamentCore.h"
//#include "HingeCore.h"
//#include "HookCore.h"
//#include "BackCasingCore.h"
//#include "MetalCore.h"
//#include "BoxCasingCore.h"
//#include "WeightCore.h"
//#include "StringCore.h"
//#include "BackHookCore.h"
//#include "NailCore.h"
//#include "HRCore.h"
//#include "MiddleHookCore.h"
//#include "MountCore.h"
//#include "PendulumCore.h"
#endif

static CSealedSingleCall bStaticShadow;

struct CPackageManager
{
	CPackageManager()
		// : mUnique( SAVERWNDCLASS )
	{		
		if( ( 0 < g_wGlosnosc ) || GetSaverParams().m_SaverMode == sm_config )
		{
			MonadAudio::InitAudio( 3 );
			SetVolumeFromRegistry();
			if( GetSaverParams().m_SaverMode != sm_config )
				MonadAudio::SetVoiceVolume( 0.0f, 1 );
		}
	}
	~CPackageManager()
	{
		// This order is significant
		SAFE_DELETE( Streaming );
		MonadAudio::ShutdownAudio();
	}
	// CAmIUnique mUnique;
};

struct CComposition
{
	typedef HRESULT ( WINAPI *typeDwmEnableComposition )( UINT ); 
	typeDwmEnableComposition fnDwmEnableComposition;

	CComposition() : fnDwmEnableComposition( NULL ),
		dllDWMAPI( L"dwmapi.dll" )
	{	// Windows XP does not implement this function
#ifndef DEBUG
		if( dllDWMAPI )
		{
			fnDwmEnableComposition = (typeDwmEnableComposition) dllDWMAPI.GetProcAddress( "DwmEnableComposition" );
			if( fnDwmEnableComposition )
				if( FAILED( fnDwmEnableComposition( DWM_EC_DISABLECOMPOSITION ) ) )
					fnDwmEnableComposition = NULL;
		}
#endif
	}
#ifndef DEBUG
	~CComposition()
	{
		if( fnDwmEnableComposition )
			fnDwmEnableComposition( DWM_EC_ENABLECOMPOSITION );
	}
private:
	CSystemLibrary dllDWMAPI;
#endif
};

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
	DXUTTRACE( L"IsDepthFormatOk\n" );
	// Verify that the depth format exists
	HRESULT hr = DXUTGetD3D9Object()->CheckDeviceFormat( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		AdapterFormat,
		D3DUSAGE_DEPTHSTENCIL,
		D3DRTYPE_SURFACE,
		DepthFormat );
	if( FAILED( hr ) ) return FALSE;

	// Verify that the backbuffer format is valid
	hr = DXUTGetD3D9Object()->CheckDeviceFormat( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		AdapterFormat,
		D3DUSAGE_RENDERTARGET,
		D3DRTYPE_SURFACE,
		BackBufferFormat );
	if( FAILED( hr ) ) return FALSE;

	// Verify that the depth format is compatible
	hr = DXUTGetD3D9Object()->CheckDepthStencilMatch( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		AdapterFormat,
		BackBufferFormat,
		DepthFormat );

	return SUCCEEDED( hr );
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
void HardwareTooOld()
{
	THROW_EXC_ONERROR( MonadException::CapsNotValid, ERROR_NOT_SUPPORTED, L"Start" );
}
//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
									 D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{	
	static ULONG_PTR nErrors = 0;     // use this to only show the very first error messagebox
	//int nPrevErrors = nErrors;
	DXUTTRACE( L"IsD3D9DeviceAcceptable\n" );

	try
	{
		// check vertex shading support
		if(pCaps->VertexShaderVersion < D3DVS_VERSION(3,0))
			if (!nErrors++) 
				HardwareTooOld();

		// check pixel shader support
		if(pCaps->PixelShaderVersion < D3DPS_VERSION(3,0))
			if (!nErrors++) 
				HardwareTooOld();

		if (!(pCaps->TextureCaps & D3DPTEXTURECAPS_MIPMAP))
			if (!nErrors++) 
				HardwareTooOld();

		if(pCaps->MaxSimultaneousTextures < 4)
			if (!nErrors++) 
				HardwareTooOld();

		if(!(pCaps->RasterCaps & D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS))
			if (!nErrors++) 
				HardwareTooOld();

	}
	CATCH;
	return true;
	//return (nErrors > nPrevErrors) ? false : true;
}

//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
									  DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	DXUTTRACE( L"IsD3D11DeviceAcceptable\n" );

	return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	DXUTTRACE( L"ModifyDeviceSettings\n" );

	pDeviceSettings->d3d9.pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	pDeviceSettings->d3d11.SyncInterval = 1;

	return true;
}

void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	if( MonadException::ExceptionInProgress() )
		return;

	MonadTime::fTime = fTime;
	static CSealedSingleCall Tim;
	if( !Tim )
		Singleton->mTiming.Start();
	if( sm_config == GetSaverParams().m_SaverMode )
		Sleep( 20 );
	CalculateFallingVolume();

	try
	{
		g_poprz_czas = g_aktual_czas;
		GETTIME;

		MonadEvent::COnEvent::FireEvent( MonadEvent::EventFrameMoveStub );
		if( g_poprz_czas.wSecond != g_aktual_czas.wSecond )
		{
			MonadEvent::COnEvent::FireEvent( MonadEvent::EventNewSecond );
		}
		CPointLightShader::ViewOnce().ResetCalledFlag();
	}
	CATCH;
}

HRESULT OnCreateDeviceGeneric( const DXUTDeviceVersion pDeviceLevel )
{
	DXUTTRACE( L"Create Device\n" );
	if( MonadException::ExceptionInProgress() )
		return E_FAIL;

	try
	{
		bFactoryIsReady = true;
		Singleton.Attach( new StructSingleton() );

		CPaths mSearchPaths;
		mSearchPaths.add_path( L"Auto" );
		if( DXUT_D3D9_DEVICE == DeviceLevel )
			mSearchPaths.add_path( L"DX9" );
		else
			mSearchPaths.add_path( L"DX11" );
		static class CSealedSingleCall bOnceToLoad;
		if( !bOnceToLoad )
		{
			//mSearchPaths.add_path( L"Once" );
			if( MonadAudio::IsAudioOn() )
			{
				mSearchPaths.add_path( L"Stream" );
				mSearchPaths.add_path( L"Clock" );
			}
		}			
		mSearchPaths.add_path( gPicturesFactory.GetLeft() );
		mSearchPaths.add_path( gPicturesFactory.GetRight() );

		g_pFileManager.Attach( new CFileSearcher() );
		g_pFileManager->Run( mSearchPaths );

		// Use PeekMessage() so we can use idle time to render the scene. 
		// Now we're ready to receive and process Windows messages.
		MSG msg;
		// msg.message = WM_NULL;

		do
		{			
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE ) != 0 )
			{
				if( WM_QUIT != msg.message && WM_CLOSE != msg.message )
				{
					PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
				else
					break; // Deferred
			}
			MonadThread::OnDequeue();
		}
		while( !g_pFileManager->IsFinished() );
		PostMessage( DXUTGetHWND(), WM_APP_SET_DEV, DBT_DEVNODES_CHANGED, 0 );
		while( !g_pFileManager->IsFinished() )
			MonadThread::OnDequeue();
		g_pFileManager.Free();		

		if( DXUT_D3D9_DEVICE == DeviceLevel )
		{
			gD3D9Handles.SetHandles(
				Singleton->MonadFactory.GetShader( L"light" ),
				Singleton->MonadFactory.GetShader( L"shadow" ) );
		}
		Singleton->OnCreateDeviceAndShadow();
		GetLocalTime( &g_poprz_czas );	
	}
	CATCH;

	return S_OK;
}

HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
#ifdef DEBUG
	static class CSealedSingleCall bGenerate;

	try
	{
		if( !bGenerate )
		{
			DXUTTRACE( L"Model Building\n" );
			// COvalCreator Oval;
		}
	}
	CATCH;
#endif

	DXUTTRACE( L"OnD3D9CreateDevice\n" );

	return OnCreateDeviceGeneric( DXUT_D3D9_DEVICE );
}
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
									 void* pUserContext )
{
	DXUTTRACE( L"OnD3D11CreateDevice\n" );

	return OnCreateDeviceGeneric( DXUT_D3D11_DEVICE );
}

void BuildProjectionMatrix()
{ 
	if( DXUT_D3D9_DEVICE == DeviceLevel )
	{
		dx = (float) DXUTGetD3D9BackBufferSurfaceDesc()->Width;
		dy = (float) DXUTGetD3D9BackBufferSurfaceDesc()->Height;
	}
	else
	{
		dx = (float) DXUTGetDXGIBackBufferSurfaceDesc()->Width;
		dy = (float) DXUTGetDXGIBackBufferSurfaceDesc()->Height;
	}

	PROJECTION_MATRIX = XMMatrixPerspectiveFovLH( XM_PIDIV4, dx / dy, 1.0f, ZFAR() );
}

HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	DXUTTRACE( L"OnD3D9ResetDevice\n" );
	if( MonadException::ExceptionInProgress() || NULL == Singleton )
		return S_OK;

	HRESULT hr( E_FAIL );

	try
	{		
		BuildProjectionMatrix();

		StructSingleton::CallResetDevice();
		StructSingleton::CallResetLogic();
		StructSingleton::CallResetLogicDX9();

		hr = S_OK;
	}
	CATCH;

	return hr;
}
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
										 const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	DXUTTRACE( L"OnD3D11ResizedSwapChain\n" );
	if( MonadException::ExceptionInProgress() || NULL == Singleton )
		return S_OK;

	HRESULT hr( E_FAIL );

	try
	{
		BuildProjectionMatrix();

		hr = S_OK;
	}
	CATCH;

	return hr;
}

void    CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	// Begin the scene.
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		if( NULL != Singleton )
		{
			if( !MonadException::ExceptionInProgress() )
			{
				try
				{
					CShadowHelper::SaveD3D9World();

					if( !bStaticShadow )
					{
						CMonadShaderGeneric::IsShadowRendered() = MONAD_STATIC_SHADOW;
						Singleton->mShadowHelper.BeforeD3D9Rendering();
						Singleton->OnFrameRender();
					}
					CMonadShaderGeneric::IsShadowRendered() = MONAD_DYNAMIC_SHADOW;
					Singleton->mShadowHelperDyn.BeforeD3D9Rendering();
					Singleton->OnFrameRender();

					CMonadShaderGeneric::IsShadowRendered() = MONAD_WORLD;
					CShadowHelper::AfterD3D9Rendering();
					Singleton->OnFrameRender();
				}
				CATCH;
			}
		}
		// End the scene.
		pd3dDevice->EndScene();
	}
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
								 float fElapsedTime, void* pUserContext )
{
	if( MonadException::ExceptionInProgress() || NULL == Singleton )
		return;

	try
	{		
		CShadowHelper::SaveD3D11World();

		ClearTextures();
		if( !bStaticShadow )
		{
			CMonadShaderGeneric::IsShadowRendered() = MONAD_STATIC_SHADOW;
			for( ULONG_PTR iLevel = 0; iLevel < CShadowHelper::s_ulShadowCnt; iLevel ++ )
			{				
				Singleton->mShadowHelper.BeforeD3D11Rendering( iLevel );
				Singleton->OnFrameRender();
			}
		}
		CMonadShaderGeneric::IsShadowRendered() = MONAD_DYNAMIC_SHADOW;
		for( ULONG_PTR iLevel = 0; iLevel < CShadowHelper::s_ulShadowCnt; iLevel ++ )
		{
			Singleton->mShadowHelperDyn.BeforeD3D11Rendering( iLevel );
			Singleton->OnFrameRender();
		}

		CMonadShaderGeneric::IsShadowRendered() = MONAD_WORLD;
		CShadowHelper::AfterD3D11Rendering();
		Singleton->OnFrameRender();
	}
	CATCH;
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
	DXUTTRACE( L"OnD3D11ReleasingSwapChain\n" );
}

void    CALLBACK OnD3D9LostDevice( void* pUserContext )
{
	DXUTTRACE( L"OnD3D9LostDevice\n" );
	bStaticShadow.ResetCalledFlag();
	StructSingleton::CallLostDevice();
}

void    CALLBACK OnDestroyDevice()
{
	DXUTTRACE( L"OnDestroyDevice\n" );

	bFactoryIsReady = false;
	if( g_pFileManager )
	{
		while( !g_pFileManager->IsFinished() )
			MonadThread::OnDequeue();
		g_pFileManager.Free();
	}
	Singleton.Free();	
	SetAllVolumes( 1.0f );
}

void CALLBACK OnD3D9DestroyDevice( void* pUserContext )
{
	DXUTTRACE( L"OnD3D9DestroyDevice\n" );
	OnDestroyDevice();
}

void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
	DXUTTRACE( L"OnD3D11DestroyDevice\n" );
	bStaticShadow.ResetCalledFlag();
	OnDestroyDevice();
}

//--------------------------------------------------------------------------------------
// Call if device was removed.  Return true to find a new device, false to quit
//--------------------------------------------------------------------------------------
bool CALLBACK OnDeviceRemoved( void* pUserContext )
{
	DXUTTRACE( L"OnDeviceRemoved\n" );
	return true;
}

void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
#ifndef DEBUG
#ifdef _MONAD_SCREENSAVER_
	DXUTShutdown();
#endif
#endif // DEBUG
};
void    CALLBACK MouseProc( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext )
{
#ifndef DEBUG
#ifdef _MONAD_SCREENSAVER_
	DXUTShutdown();
#endif
#endif // DEBUG
}


LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
						 void* pUserContext )
{
	switch( uMsg )
	{
	case WM_CLOSE:
		{
			MonadException::SetWindowParent( NULL );
			break;
		}
	case WM_DESTROY :
		{
			MonadException::ShowRegisteredException();
			break;
		}
#ifndef DEBUG
#ifdef _MONAD_SCREENSAVER_
	case WM_MOUSEMOVE :
		{
			if( GetSaverParams().IsFullScreen() )
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
					if( m_dwSaverMouseMoveCount > 8 )
						DXUTShutdown();
				}
				*pbNoFurtherProcessing = true;
			}
			break;
		}
#endif // _MONAD_SCREENSAVER_
#endif // DEBUG
	case WM_SETCURSOR:
		{
			if( GetSaverParams().IsFullScreen() )
			{ 
				SetCursor( NULL ); 
				*pbNoFurtherProcessing = true;
			}
			break;
		}
	case WM_APP_EXIT:
		{
			DXUTShutdown( ( int ) wParam );
			*pbNoFurtherProcessing = true;
			break;
		}		
	case WM_POWERBROADCAST:
		{
			if( wParam != PBT_APMSUSPEND )
				break;
		}
	case WM_ACTIVATEAPP:
		{
			if( GetSaverParams().IsFullScreen() )
			{
				if( wParam == FALSE )
					DXUTShutdown();
				*pbNoFurtherProcessing = true;
			}
			break;
		}
	case WM_SYSCOMMAND:
		{
			if( GetSaverParams().IsFullScreen() )
				if(wParam == SC_NEXTWINDOW
					||wParam == SC_PREVWINDOW
					||wParam == SC_SCREENSAVE
					||wParam == SC_CLOSE)
					*pbNoFurtherProcessing = true;
		}
	}
	return 0;
}

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
void DoSaver( const HINSTANCE hInstance, const HWND DefWnd = 0)
{
	// Pass control to the sample framework for handling the message pump and 
	// dispatching render calls. The sample framework will call your FrameMove 
	// and FrameRender callback when there is idle time between handling window messages.

	if( NULL != DefWnd )
	{
		RECT rc;
		GetClientRect( DefWnd, &rc );
		// In preview mode, "pause" (enter a limited message loop) briefly 
		// before proceeding, so the display control panel knows to update itself.
		m_bWaitForInputIdle = true;

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

		THROW_EXC_D3D( DXUTSetWindow( DefWnd, DefWnd, DefWnd, true ), L"Set Preview Window" );
		THROW_EXC_D3D( DXUTCreateDevice( 
			D3D_FEATURE_LEVEL_11_0,
			true, 
			rc.right - rc.left,
			rc.bottom - rc.top), L"Can not create Preview Device" );
	}
	else
	{
		const RECT rc( GetMonitorRect() );
		THROW_EXC_D3D( DXUTCreateDevice(
			D3D_FEATURE_LEVEL_11_0,
			DXUT_D3D9_DEVICE == DeviceLevel,
			rc.right, 
			rc.bottom ), L"Can not create Device" );
	}
	// Start the render loop
	THROW_EXC_IFFAILED( MonadException::Unknown, DXUTMainLoop(), L"Loop" );
	DXUTShutdown();		
}

INT wInternalWinMain( HINSTANCE hInstance )
{
	// SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL );
	// Disable gamma correction in DXUT
	DXUTSetIsInGammaCorrectMode( false );

	// Set the callback functions
	DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
	DXUTSetCallbackDeviceRemoved( OnDeviceRemoved );

	g_lightPosition = XMVectorSet( 1.1f * g_xDir, .755f, 2.734f, 1.0f );
	const XMFLOAT3A vUpVecInternal( 0.0f, 1.0f, 0.0f );
	vUpVec = XMLoadFloat3A( &vUpVecInternal );
	const XMFLOAT3A vEyePtInternal( XMVectorGetX( g_lightPosition ), XMVectorGetY( g_lightPosition ), XMVectorGetZ( g_lightPosition ) );
	const XMFLOAT3A vLookatPtInternal( XMVectorGetX( g_lightPosition ) / 2.0f, 0.0f, 0.3f );
	const XMVECTOR vEyePt = XMLoadFloat3( &vEyePtInternal );
	const XMVECTOR vLookatPt = XMLoadFloat3( &vLookatPtInternal );

#ifndef MONAD_DIRECT3D9_MODE
	CMonadWindowsVersion WinVer;
	if( !WinVer.IsWindows_Vista_SP2() )
	{
#endif
		DeviceLevel = DXUT_D3D9_DEVICE;
		DXUTSetCallbackD3D9DeviceAcceptable( IsD3D9DeviceAcceptable );
		DXUTSetCallbackD3D9DeviceCreated( OnD3D9CreateDevice );
		DXUTSetCallbackD3D9DeviceReset( OnD3D9ResetDevice );
		DXUTSetCallbackD3D9DeviceLost( OnD3D9LostDevice );
		DXUTSetCallbackD3D9DeviceDestroyed( OnD3D9DestroyDevice );
		DXUTSetCallbackD3D9FrameRender( OnD3D9FrameRender );

		//set special texture matrix for shadow mapping
		const float fOffsetX = 0.5f + (0.5f / (float)CShadowHelper::TEXDEPTH_WIDTH);
		const float fOffsetY = 0.5f + (0.5f / (float)CShadowHelper::TEXDEPTH_HEIGHT);
		const unsigned int range = 1;            //note different scale in DX9!
		float fBias    = 0.0f;
		const XMFLOAT4X4A texScaleBiasMatInternal( 0.5f,     0.0f,     0.0f,         0.0f,
			0.0f,    -0.5f,     0.0f,         0.0f,
			0.0f,     0.0f,     (float)range, 0.0f,
			fOffsetX, fOffsetY, fBias,        1.0f );
		texScaleBiasMat = XMLoadFloat4x4A( &texScaleBiasMatInternal );
		XMMATRIX SHADOW_PROJECTION_MATRIX = XMMatrixPerspectiveFovLH( D3DXToRadian( 75.0f ), 1.0f, 1.0f, 12.0f );
		XMMATRIX SHADOW_VIEW_MATRIX = XMMatrixLookAtLH( vEyePt, vLookatPt, vUpVec );
		SHADOW_TEX_MATRIX = XMMatrixMultiply( SHADOW_VIEW_MATRIX, SHADOW_PROJECTION_MATRIX );
#ifndef MONAD_DIRECT3D9_MODE
	}
	else
		// Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
	{
		DeviceLevel = DXUT_D3D11_DEVICE;
		DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
		DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
		DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
		DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
		DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
		DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );
	}
#endif

	// Initialize DXUT and create the desired Win32 window and Direct3D device for the application
	DXUTInit( false, false, NULL, false ); // Parse the command line and show msgboxes

	struct CWallBitmap
	{
		CWallBitmap( HINSTANCE hInstance )
		{
			hbmp = LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_BACKGROUND ) );
			THROW_EXC_IFZ( hbmp, MonadException::OutOfMemory, L"Bitmap" );
		}
		~CWallBitmap()
		{
			DeleteObject( hbmp );
			hbmp = NULL;
		}			
	};

	try
	{
		if( !SetDllDirectory( ( GetRootPath() + L"Bin" ).c_str() ) )
			THROW_EXC( MonadException::NotInit, E_FAIL, L"DLL" );
		struct CScreenSaverParam
		{
			CScreenSaverParam()
			{
				SystemParametersInfo( SPI_SCREENSAVERRUNNING, TRUE, &m_bUnused, 0 );
			}
			~CScreenSaverParam()
			{
				SystemParametersInfo( SPI_SCREENSAVERRUNNING, FALSE, &m_bUnused, 0 );
			}

		private:
			BOOL m_bUnused;
		};
		CAutoPtr<CScreenSaverParam> ScrnParam;

		CPackageManager mPackageManager;

		switch( GetSaverParams().m_SaverMode )
		{
		case sm_config :
			return MyDoConfig( hInstance );
		case sm_full :
			// Flag as screensaver running if in full on mode
			ScrnParam.Attach( new CScreenSaverParam() );
		case sm_test :
			{
				DXUTSetCallbackKeyboard( KeyboardProc );
				DXUTSetCallbackMouse( MouseProc );
				CWallBitmap WallBitmap( hInstance );
				CSatelliteWindowClass mSatelliteClass;
				CFullWindowClass mFullClass;
				mSatelliteClass.RegisterClass( hInstance );
				mFullClass.RegisterClass( hInstance );
				CComposition mComposition;
				THROW_EXC_IFFAILED( MonadException::InvalidCall, 
					DXUTCreateWindow( GetOPC().c_str() ), L"CreateWin" );
				MonadException::SetWindowParent( DXUTGetHWND() );
				DoSaver( hInstance );
				break;
			}
		case sm_preview :
			{
				CWallBitmap WallBitmap( hInstance );
				RECT rc;
				THROW_EXC_IFZ( GetClientRect( GetSaverParams().m_hWndParent, &rc ), MonadException::InvalidClass, L"Parent Window" );				
				CPreviewWindowClass	mPreviewClass;
				mPreviewClass.RegisterClass( hInstance );
				const DWORD dwStyle = WS_VISIBLE | WS_CHILD;
				AdjustWindowRect( &rc, dwStyle, FALSE );
				const HWND m_hWnd = CreateWindow( mPreviewClass, GetOPC().c_str(), dwStyle, 
					rc.left, rc.top, 
					rc.right-rc.left, rc.bottom-rc.top, 
					GetSaverParams().m_hWndParent, NULL, hInstance, NULL );
				THROW_EXC_IFZ( m_hWnd, MonadException::NotInit, L"Can not create a child window" );
				DoSaver( hInstance, m_hWnd );
				break;
			}
		default :
			THROW_EXC( MonadException::InvalidCall, E_FAIL, L"Invalid parameters have been passed to the executable" );
		}

		return ERROR_SUCCESS;
	}
	CATCH;

	MonadException::SetWindowParent( NULL );
	MonadException::ShowRegisteredException();

	return EXIT_FAILURE;
}

INT WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPWSTR, int )
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

#ifndef _DEBUG
#ifdef HARDWARE_EXCEPTIONS
	__try
	{
#endif
#endif
		return wInternalWinMain( hInstance );
#ifndef _DEBUG
#ifdef HARDWARE_EXCEPTIONS
	}
	__except(1)
	{
		ExitProcess( EXIT_FAILURE );
	}
#endif
#endif
}
