// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "MonadParameter.h"

#include "MonadSet.h"

/*BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	lpReserved;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}*/

INT WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPWSTR, int )
{
	g_hModule = hInstance;
	if( IsParameter( L"/addclock" ) )
		return AddClock();
	else if( IsParameter( L"/removeclock" ) )
		return RemoveClock();
	else
		return EXIT_FAILURE;
}
