#ifndef _KONFIG_H_
#define _KONFIG_H_

// #############################################
// Nazwa:LoadStringI18N
// Opis: Wczytuje ³añcuch w jednym z 3 jêzyków
// #############################################
namespace MiniWinApi
{
	// Poni¿sze funkcje ³aduj¹ ³añcuchy z zasobów
	// w celu dalszego ich u¿ycia
	// U¿ycie jest zbli¿one do LoadString oraz SetWindowText
	TCHAR *LoadStringI18N(const UINT pStrId);
	BOOL SetWindowText( 
			HWND hWnd, 
			const UINT pStrId);
}

// #############################################
// Nazwa: funkcja MyDoConfig
// Opis: Otwiera okienko konfiguracyjne
//       Zwracane wartoœci to: S_OK albo E_FAIL
//       Procedura modyfikuje zmienne globalne.
// #############################################
HRESULT MyDoConfig(const HINSTANCE p_hinst, const HWND p_par);

// #############################################
// Nazwa: funkcja TmpFile
// Opis: Tworzy plik tymczasowy,
//       który zostanie usuniêty.
// #############################################
//extern CGrowableArray<TCHAR *> CreatedTemps;
//TCHAR * TmpFile(const TCHAR * pPrefix, const INT pID);

#endif