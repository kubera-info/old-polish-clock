// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of featuspecific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0410		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif


#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

// Windows Header Files:
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>

// C RunTime Header Files
#include <tchar.h>

// Math
#define _USE_MATH_DEFINES
#include <math.h>
#include <D3dx8math.h>

// DirectX
#include <dxerr8.h>
#include <D3d8types.h>
#include <D3d8.h>
#include <D3dx8tex.h>
#include "dxutil.h"
#include "dmutil.h"

#include "Resource.h"
#include "define.h"
#include "ogolne.h"
#include "blad.h"

// DirectX
extern D3DXMATRIXA16 g_world;
extern LPDIRECT3DTEXTURE8 g_Texture, g_Texture2, g_Texture3;
extern D3DLIGHT8 light;
extern D3DMATERIAL8 mtrl, mtrl_sek, mtrl_tarcz;
extern D3DMATERIAL8 mtrl_tarcz, mtrl_mosiadz, mtrl_stal;
extern LPDIRECT3DVERTEXBUFFER8 m_pBigSquareVB;

extern HWND g_hWndParent;

// Konfiguracja
extern WORD g_wGlosnosc;
extern WORD g_wJakoscGr;
extern WORD g_wWielkosc;
extern BOOL g_bWskazSek;

extern TCHAR g_reg [];

extern BOOL g_bUseDepthBuffer;