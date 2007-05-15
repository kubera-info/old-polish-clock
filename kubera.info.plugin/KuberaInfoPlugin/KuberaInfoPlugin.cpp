// UnicodePlugin.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "KuberaInfoPlugin.h"

#define IDS_SIGN						16

static HANDLE MyMutex;

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: ;
	case DLL_THREAD_ATTACH: ;
	case DLL_THREAD_DETACH: ;
	case DLL_PROCESS_DETACH: ;
	}
	// Biblioteka nie wykonuje ¿adnych czynnoœci inicjalizacyjnych lub destrukcyjnych.
	return TRUE;
}

// Funkcja umo¿liwia wykrycie i ostrze¿enie przed zainstalowaniem Zegara w serwerze.
// Mog³oby to w niektórych przypadkach wp³yn¹æ negatywnie na wydajnoœci pracy komputera.
VOID IsWindowsServer ( NSIS_ARGS )
{
	EXDLL_INIT(); // Niezbêdna inicjalizacja w NSIS

	OSVERSIONINFOEX m_VersionBuffer;
	ZeroMemory(&m_VersionBuffer, sizeof(m_VersionBuffer));
	m_VersionBuffer.dwOSVersionInfoSize = sizeof(m_VersionBuffer);
	if (GetVersionEx((LPOSVERSIONINFO) &m_VersionBuffer))
	{
		if ( (VER_NT_DOMAIN_CONTROLLER == m_VersionBuffer.wProductType) || (VER_NT_SERVER == m_VersionBuffer.wProductType) )
			pushstring ( "server" );
		else
			pushstring ( "desktop" );
	}
	else
		pushstring ( "desktop" );
}

// W wygaszaczu Zegar jest zaszyty GUID.
// Na tej podstawie mo¿na stwierdziæ z du¿ym prawdopodobieñstwem, czy plik mo¿na zamazaæ.
VOID VerifyBinary ( NSIS_ARGS )
{
	EXDLL_INIT(); // Niezbêdna inicjalizacja w NSIS

	const int MaxSize = 64;
	char mFile [1024], mGUID [1024];
	popstring ( mFile );
	popstring ( mGUID );

	if(NULL == mFile)
	{
		pushstring ( "file error" );
		return;
	}
	else if(NULL == mGUID)
	{
		pushstring ( "GUID error" );
		return;
	}
	HMODULE hm = LoadLibraryEx(mFile, NULL, LOAD_LIBRARY_AS_DATAFILE);
	if(NULL == hm)
	{
		pushstring ( "friend" );
		return;
	}
	char buf [MaxSize] = "";
	LoadString(hm, IDS_SIGN, buf, MaxSize);
	FreeLibrary(hm);
	if(0 == buf[0])
		pushstring ( "alien" );
	else if(0 == StrNCmp(buf, mGUID, MaxSize))
		pushstring ( "friend" );
	else
		pushstring ( "alien" );
}

// Global Mutex chroni¹cy przed uruchomieniem dwóch sesji instalatora.
VOID UniqueInstallation ( NSIS_ARGS )
{
	EXDLL_INIT(); // Niezbêdna inicjalizacja w NSIS

	HANDLE MyMutex = CreateMutex(NULL, FALSE, "WWWKUBERAINFOINSTALLER");
	switch(GetLastError())
	{
	case ERROR_ALREADY_EXISTS:
		pushstring ( "dup" );
		break;
	default:
		pushstring ( "unique" );
	}
}

// Jawne zwolnienie biblioteki.
// U¿ycie tej funkcji koñczy siê skasowaniem jej z katalogu tymczasowego
// przez skrypt NSIS.
VOID Unload ( NSIS_ARGS )
{
	EXDLL_INIT(); // Niezbêdna inicjalizacji w NSIS

	if(MyMutex)
		CloseHandle(MyMutex);
}