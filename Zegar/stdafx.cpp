// stdafx.cpp : source file that includes just the standard includes
// Zegar.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// DirectX
D3DXMATRIXA16 g_world;
LPDIRECT3DTEXTURE8 g_Texture = NULL;
LPDIRECT3DTEXTURE8 g_Texture2 = NULL;
LPDIRECT3DTEXTURE8 g_Texture3 = NULL;
D3DLIGHT8 light;
D3DMATERIAL8 mtrl; // Wskazówki: minutowa i godzinowa
D3DMATERIAL8 mtrl_sek;
D3DMATERIAL8 mtrl_tarcz; // Bia³y, neutralny kolor
D3DMATERIAL8 mtrl_mosiadz;
D3DMATERIAL8 mtrl_stal;
LPDIRECT3DVERTEXBUFFER8 m_pBigSquareVB; // CShadowVolume
HWND g_hWndParent = NULL;
// Konfiguracja
WORD g_wGlosnosc = 90, g_wJakoscGr = 1000, g_wWielkosc = SWIAT;
BOOL g_bWskazSek = FALSE;
// Rejestr
TCHAR g_reg [] = REGODNOSNIK;
BOOL g_bUseDepthBuffer;