#include "stdafx.h"

// #############################################
// Nazwa: ZaladujZasob
// Opis: Funkcja ³aduje dowolny zasób do pamiêci
// #############################################
LPCVOID ZaladujZasob(CONST INT pResID, PCTSTR pResTyp, DWORD & pLen)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	HRSRC hRsrc = FindResource(
		hInst,
		MAKEINTRESOURCE(pResID),
		pResTyp);
	if(hRsrc == NULL)
		return NULL;
	DWORD len = SizeofResource(hInst, hRsrc);
	if(len == 0)
		return NULL;
	LPVOID lpLoad = LoadResource(hInst, hRsrc);
	if(lpLoad == NULL)
		return NULL;
	pLen = len;
	return LockResource(lpLoad);
}