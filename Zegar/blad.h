#ifndef _BLAD_H_
#define _BLAD_H_

#include <stdio.h>

HRESULT wypisz_blad(const HRESULT = GetLastError());

#define W   "B³¹d obliczeñ: \n" \
"%8d :" \
"%s\n" \
"Plik: %s"*/


extern TCHAR g_blad [256];

#endif