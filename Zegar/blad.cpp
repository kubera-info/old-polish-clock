#include "stdafx.h"

TCHAR g_blad [256] = L"";

// #############################################
// Nazwa: wypisz_blad
// Opis: Zapisanie danych w rejestrze
// #############################################
HRESULT wypisz_blad(const HRESULT e)
{
	if(e != ERROR_SUCCESS)
	{
		LPVOID lpMsgBuf;
		FormatMessageA( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			e,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPSTR) &lpMsgBuf, // Zmiana!
			0,
			NULL 
		);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		MessageBoxA( NULL, (PCHAR)lpMsgBuf, "B³¹d", MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}
	return e;
}
