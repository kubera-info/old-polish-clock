#ifndef _KONFIG_H_
#define _KONFIG_H_

// #############################################
// Nazwa: funkcja Jezyk
// Opis: Zwraca przemieszczenia dla
//       narodowych danych (0 - j. Polski)
// #############################################
inline BYTE Jezyk();

// #############################################
// Nazwa: funkcja MyDoConfig
// Opis: Otwiera okienko konfiguracyjne
//       Zwracane wartoœci: S_OK i E_FAIL
//       Procedura modyfikuje zmienne glob.
// #############################################
HRESULT MyDoConfig(HINSTANCE p_hinst, HWND p_par);

// #############################################
// Nazwa: funkcja DirectX9OK
// Opis: Test na wystêpowanie DirectX9
//       w systemie operacyjnym
// #############################################
bool DirectX9OK();

#endif