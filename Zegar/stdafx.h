// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of featuspecific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif


#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

/*// Windows Header Files:
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>

// C RunTime Header Files
#include <tchar.h>*/

// Math
#define _USE_MATH_DEFINES

#include <math.h>
//#include <D3dx9math.h>

// DirectX
#include "dxstdafx.h"

// Extension
#include "Resource.h"
#include "Version.h"
#include "D3D\d3dsaver.h"
#include "define.h"
#include "blad.h"
#include "MyEncrypt.h"
#include "ogolne.h"
#include "kubera.math.h"

// DirectX
extern D3DXMATRIXA16 g_world, g_small_world;
extern D3DLIGHT9 light;

// Konfiguracja
extern DWORD g_wGlosnosc;
#if defined(CPP_RAW_PRIMITIVE)
extern DWORD g_wJakoscGr;
extern DWORD g_wWielkosc;
#endif
extern BOOL g_bWskazSek;
extern BOOL g_bOpadanie;
extern bool g_bTwoSided;
extern const TCHAR URL [];
extern const TCHAR g_strRegPath[];

extern TCHAR g_reg [];

extern SaverMode g_SaverMode;
