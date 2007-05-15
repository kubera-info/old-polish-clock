// Wygaszacz Ekranu "Stary Polski Zegar".
// Aplikacja ilustruje artyku³ w Software Developer Juornal.
// Copyright © 2007 by W³odzimierz O. Kubera
// All rights reseved
// Wszelkie prawa zastrze¿one
// Aplikacja jest wykonana w technologii 32-bitowej.
// Wymagany system operacyjny Windows XP lub nowszy.
// Aplikacja jest licencjonowana zgodnie z dokumentem GNU GPL 2.1.
// Licencja znajduje siê w katalogu "Licenses".
// You could find a license for this project in the folder 'Licenses'.

#pragma once

#include "D3DSaver.h"

namespace D3DSaver
{
	extern SYSTEMTIME g_aktual_czas;
	extern SaverMode m_SaverMode;

	//--------------------------------------------------------------------------------------
	// Forward declarations 
	//--------------------------------------------------------------------------------------
	bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
	void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
	void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
	void    CALLBACK MouseProc( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );
	//void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

	bool    CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
	HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	void    CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	void    CALLBACK OnD3D9LostDevice( void* pUserContext );
	void    CALLBACK OnD3D9DestroyDevice( void* pUserContext );

	//bool    CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
	//HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	//HRESULT CALLBACK OnD3D10SwapChainResized( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	//void    CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	//void    CALLBACK OnD3D10SwapChainReleasing( void* pUserContext );
	//void    CALLBACK OnD3D10DestroyDevice( void* pUserContext );

}