#ifndef _BLAD_H_
#define _BLAD_H_

#include <stdio.h> // _snprintf

HRESULT wypisz_blad(const HRESULT = GetLastError());

#define W   "B³¹d obliczeñ: \n" \
			"%8d :" \
			"%s\n" \
			"Plik: %s"

//-----------------------------------------------------------------------------
// Name: struct CBlad
// Desc: 
//-----------------------------------------------------------------------------

class CBlad
{
public:
	CBlad(const char * p_wyrazenie, const DWORD p_wiersz, const char * p_plik)
	{
		m_wyrazenie = NULL;
		size_t len = strlen(p_wyrazenie) + strlen(p_plik) + 128;
		m_wyrazenie = new char [strlen(p_wyrazenie) + strlen(p_plik) + 128];
		_snprintf(m_wyrazenie, len, W, p_wiersz, p_wyrazenie, p_plik);		
		m_wyrazenie[len - 1] = 0;
	}
	virtual void MsgBox()
	{
		MessageBox(0, m_wyrazenie, "B³¹d pracy aplikacji", MB_ICONSTOP);
	}
	~CBlad()
	{
		SAFE_DELETE_ARRAY(m_wyrazenie);
	}
private:
	char * m_wyrazenie;
};

class CBladCichy : public CBlad
{
public:
	CBladCichy(const char * p_wyrazenie, const DWORD p_wiersz, const char * p_plik) :
		CBlad(p_wyrazenie, p_wiersz, p_plik)
	{
	}
	void MsgBox()
	{
	}
};

extern TCHAR g_blad [4096];

#endif