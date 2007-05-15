// ##########################################################
// MyEncrypt.h - pakiet szyfrowania i odszyfrowywania danych.
// Umo¿liwia przechowywanie zasobów zaszyfrowanych plików
// oraz zarz¹dzanie nimi.
// ##########################################################

#ifndef _MY_ENCRYPT_H_
#define _MY_ENCRYPT_H_

#include "stdafx.h"

// ############################################################
// Nazwa: class ResourceCrypt
// Opis: Klasa szyfrowania i deszyfrowania zasobów (plików).
//       Identyfikator zasobu jest u¿ywany jako jeden z kluczy.
// ############################################################
class ResourceCrypt
{
public:
	// Za³adowanie zaszyfrowanego zasobu.
	ResourceCrypt(const INT p_ID);

#if defined(_DEBUG)
	// Konstruktor alokuj¹cy bufor do zapisania w formie zaszyfrowanej.
	ResourceCrypt(const PCHAR pFileName,
		const PCHAR pEncryptedFileName,
		const INT p_ID);

#endif    
	~ResourceCrypt();
	// Klar bufora.
	LPCVOID GetBuffer();
	// Wielkoœæ bufora.
	DWORD GetBufferSize();
private:
	// Funkcja szyfruj¹ca.
	HRESULT Encrypt(const LPBYTE bKey, const DWORD dwKeySize);
	// Funkcja deszyfruj¹ca.
	HRESULT Decrypt(const LPBYTE bKey, const DWORD dwKeySize);
	// Dwie powy¿sze funkcje w bie¿¹cej wersji zegara wykonuj¹
	// tak¹ sam¹ operacjê.

	// Klar bufora.
	LPCVOID m_Buffer;
	// Wielkoœæ bufora.
	DWORD m_BufferSize;
	// Salt.
	const static DWORD salt = 701;
	static int NewSalt(INT pIn);
};

#endif
