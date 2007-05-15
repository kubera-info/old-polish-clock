// stdafx.cpp : source file that includes just the standard includes
// Zegar.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// DirectX
D3DXMATRIXA16 g_world, g_small_world;
D3DLIGHT9 light;
// Konfiguracja
DWORD g_wGlosnosc = 800; 
#if defined(CPP_RAW_PRIMITIVE)
DWORD g_wJakoscGr = /*1000*/ 800, g_wWielkosc = SWIAT;
#endif

BOOL g_bWskazSek = FALSE;
BOOL g_bOpadanie = TRUE;
bool g_bTwoSided = false;

// Rejestr
TCHAR g_reg [] = REGODNOSNIK;

const TCHAR URL [] = L"http://screensaver.kubera.info/";

const TCHAR g_strRegPath[] = REGODNOSNIK;

SaverMode g_SaverMode;

// HWND g_hWnd;