#ifndef _OGOLNE_H_
#define _OGOLNE_H_

#include "shadowvolume.h"
#include <atlcoll.h>

struct VERTEXTEX
{
    D3DXVECTOR3 pos;
	D3DXVECTOR3 nrm;
    FLOAT       tu, tv;   // The texture coordinates	

	static const DWORD FVF = (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL);
};
struct VERTEXPLAIN
{
    D3DXVECTOR3 pos;
	D3DXVECTOR3 nrm;

	static const DWORD FVF = (D3DFVF_XYZ | D3DFVF_NORMAL);
};

struct SHADOWVERTEX
{
    D3DXVECTOR4 p;
    D3DCOLOR    color;

    static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
};

// #############################################
// Nazwa: class CElipsa
// Opis: Klasa obliczaj¹ca fragment elipsy
//       zawieraj¹cy ¿¹dan¹ il. punktów
// #############################################
class CElipsa
{
public:
	CElipsa(const WORD p_il_bok, const float p_promien_x = NULL, float p_promien_y = 0.0f, const float p_kat = M_PI_MUL_2, const float p_start = 0.0f)
	{
		m_il_bok = p_il_bok;
		m_punkty = new D3DXVECTOR2 [m_il_bok + 1];		
		m_promien_x = p_promien_x;
		m_promien_y = (p_promien_y == NULL ? p_promien_x : p_promien_y);
		m_kat = p_kat;
		m_start = p_start;
		Oblicz();
	}
	virtual Oblicz()
	{
		float m_faza = m_start;
		float m_krok = m_kat / m_il_bok;
		for(WORD m_ind = 0; m_ind < m_il_bok; m_ind ++)
		{
			PrzyWpisaniu(m_faza, m_ind);
			m_faza += m_krok;
		}
		if(m_kat == M_PI_MUL_2)
			m_punkty[m_ind] = * m_punkty; // "0 = 2 PI"
		else
			PrzyWpisaniu(m_kat + m_start, m_ind); // Pomin zaokr¹glenie sumowania
	}
	void PrzyWpisaniu(const float p_faza, const WORD p_ind)
	{
		float m_x;
		float m_y;
		m_x = + cosf(p_faza);
		m_y = - sinf(p_faza);
		m_punkty[p_ind] = D3DXVECTOR2(m_x * m_promien_x, m_y * m_promien_y);
	}
	float WezX(const WORD p_ind)
	{
		return m_punkty[p_ind][0];
	}
	float WezY(const WORD p_ind)
	{
		return m_punkty[p_ind][1];
	}
	WORD WezIlPunkt()
	{
		return m_il_bok + 1;
	}
	~CElipsa()
	{
		delete [] m_punkty;
	}
	D3DXVECTOR2 * m_punkty;
	float m_kat, m_promien_x, m_promien_y, m_start;
	WORD m_il_bok; // == ilosc wierzcho³ków = ilosc punktów ³¹cz¹cych - 1
};

// #############################################
// Nazwa: class CObiektGraf
// Opis: Bazowa klasa grafiki Direct3D zegara
// #############################################
class CObiektGraf
{
public:
	CObiektGraf(const D3DXVECTOR3 p_srodek)
	{
		m_srodek = p_srodek;
	}
	virtual HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice ) = 0;
    virtual HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice = NULL )					{ return S_OK; }
    virtual HRESULT InvalidateDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice = NULL )				{ return S_OK; }
	virtual HRESULT RenderShadow( LPDIRECT3DDEVICE8 pd3dDevice )								{ return S_OK; }
	virtual HRESULT AddPrimitive( )																{ return S_OK; }

	D3DXVECTOR3 m_srodek;
};

typedef class CAtlArray<CObiektGraf *> CObiektGrafArray;

// #############################################
// Nazwa: class CPrymityw
// Opis: Klasa przechowuj¹ca i rysuj¹ca
//       tzw. "Primitive" - zbiór trójk¹tów
// #############################################
class CPrymityw : public CObiektGraf
{
public:
	CPrymityw(
			const D3DXVECTOR3 p_srodek = D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			const D3DPRIMITIVETYPE p_rodzaj_opt = D3DPT_TRIANGLESTRIP,
			ULONG p_nrkolejny = __COUNTER__) :
		CObiektGraf(p_srodek)
	{
		m_nrkolejny = p_nrkolejny;
		m_rodzaj_opt = p_rodzaj_opt;
		m_vertices = NULL;
		m_VB = NULL;
		m_wolumin = new CShadowVolume();
	}
	virtual D3DXVECTOR3 * WezPunkt(const WORD p_indeks) = 0;
	virtual D3DXVECTOR3 * WezNorm(const WORD p_indeks) = 0;
	D3DPRIMITIVETYPE PodajTyp()
	{
		return m_rodzaj_opt;
	}
	// #############################################
	// Nazwa: metoda CPrymityw::DajIndeksy
	// Opis: Zwraca 3 wartoœci WORD
	//       bêd¹ce indeskami wierczho³ków
	// #############################################
	void DajIndeksy(const WORD p_trojkat, WORD & a, WORD & b, WORD & c)
	{
		switch(PodajTyp())
		{
			case D3DPT_TRIANGLESTRIP : 
				if(p_trojkat % 2 == 0) // Nieparzyste trójk¹ty maj¹ odwrócon¹ kolejnoœæ wierzch.
				{
					a = p_trojkat + 0;
					b = p_trojkat + 1;
					c = p_trojkat + 2;
				}
				else
				{
					c = p_trojkat + 0;
					b = p_trojkat + 1;
					a = p_trojkat + 2;
				}
				break;
			case D3DPT_TRIANGLEFAN : 
					a = 0;
					b = p_trojkat + 1;
					c = p_trojkat + 2;
				break;
			case D3DPT_TRIANGLELIST : 
					a = p_trojkat * 3;
					b = p_trojkat * 3 + 1;
					c = p_trojkat * 3 + 2;
				break;
		}
	}
	virtual DWORD GetFVF() = 0;
	virtual LONG SizeOf() = 0;
	virtual VOID * WezTymczas() = 0;
	virtual HRESULT Create(const WORD p_il_trojk)
	{
		m_il_trojk = p_il_trojk;
		switch(PodajTyp())
		{
			case D3DPT_TRIANGLESTRIP : 
				m_il_punkt = (p_il_trojk << 1) + 2;
				break;
			case D3DPT_TRIANGLEFAN : 
				m_il_punkt = p_il_trojk + 2;
				break;
			case D3DPT_TRIANGLELIST : 
				m_il_punkt = p_il_trojk * 3;
				break;
		}
		return S_OK;
	}
	HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice )
	{
		pd3dDevice->SetStreamSource( 0, m_VB, SizeOf() );
		//pd3dDevice->SetStreamSource( 0, m_VB, NULL, sizeof(VERTEXTEX) );
		//pd3dDevice->SetVertexShader( NULL );
		//pd3dDevice->SetFVF( VERTEXTEX::FVF );
		pd3dDevice->SetVertexShader( GetFVF() );
		pd3dDevice->DrawPrimitive( 
			PodajTyp(), 
			0, 
			m_il_trojk);
		return S_OK;
	}
	HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE8 pd3dDevice = NULL)
	{
		if( FAILED(pd3dDevice->CreateVertexBuffer( m_il_punkt * SizeOf(),
			0 , GetFVF() ,
			D3DPOOL_DEFAULT, &m_VB ) ) )
			return E_FAIL;
		if( FAILED(m_VB->Lock( 0, 0, (BYTE **) &m_vertices, 0 ) ) )
			return E_FAIL;
		//m_VB->Lock( 0, 0, &m_vertices, 0 );
		memcpy(m_vertices, WezTymczas() , SizeOf() * m_il_punkt);
		if( FAILED(m_VB->Unlock()) )
			E_FAIL;

		return S_OK;
	}
	HRESULT InvalidateDeviceObjects(LPDIRECT3DDEVICE8 pd3dDevice = NULL)
	{
		SAFE_RELEASE(m_VB);

		return S_OK;
	}
	HRESULT RenderShadow( LPDIRECT3DDEVICE8 pd3dDevice )
	{
		m_wolumin->Render( pd3dDevice );
		return S_OK; 
	}

	virtual void Dodaj(const D3DXVECTOR3 & p_przesun)
	{
		for(WORD m_ind = 0; m_ind < m_il_punkt; m_ind ++)
			D3DXVec3Add(WezPunkt(m_ind), WezPunkt(m_ind), &p_przesun);
	}
	HRESULT AddPrimitive( )
	{
		return m_wolumin->AddFromPrimitive( * this );
		
	}

	/*~CPrymityw()
	{
		SAFE_RELEASE(m_VB->Release());
	}*/

	VOID * m_vertices;
	D3DPRIMITIVETYPE m_rodzaj_opt;
	WORD m_il_trojk, m_il_punkt;
	UINT m_nrkolejny;
	LPDIRECT3DVERTEXBUFFER8 m_VB;
	CShadowVolume * m_wolumin;
};

// #############################################
// Nazwa: wzorzec CNazwaKlasy
// Opis: Dowolny rodzaj "prymitywu"
// #############################################
template 
	<typename FormatFigury> class CNazwaKlasy : public CPrymityw
{
public:
	CNazwaKlasy(
		const D3DXVECTOR3 p_srodek, 
		const D3DPRIMITIVETYPE p_rodzaj_opt, 
		UINT p_nrkolejny = NULL) :
		CPrymityw(p_srodek, p_rodzaj_opt, p_nrkolejny)
	{
	}
	virtual HRESULT Create(const WORD p_il_trojk)
	{
		CPrymityw::Create(p_il_trojk);
		m_tymczas = new FormatFigury [m_il_punkt];		
		return (m_tymczas ? S_OK : E_FAIL);
	}

	DWORD GetFVF()
	{
		return FormatFigury::FVF;
	}
	LONG SizeOf()
	{
		return sizeof(FormatFigury);
	}
	D3DXVECTOR3 * WezPunkt(const WORD p_indeks)
	{
		return &(m_tymczas[p_indeks].pos);
	}
	D3DXVECTOR3 * WezNorm(const WORD p_indeks)
	{
		return &(m_tymczas[p_indeks].nrm);
	}
	VOID * WezTymczas()
	{
		return (VOID *) m_tymczas;
	}

	FormatFigury * m_tymczas;
};

typedef CNazwaKlasy <VERTEXPLAIN> CVertexPlain;
typedef CNazwaKlasy <VERTEXTEX> CVertexTex;

class CKombajnGraf : public CObiektGraf
{
public:
	CKombajnGraf(const D3DXVECTOR3 p_srodek) :
		CObiektGraf(p_srodek)
	{
	}

	HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice )
	{
		for (size_t i = 0; i < m_obiekty.GetCount(); i++)
		{
			if(FAILED(m_obiekty.GetAt(i)->Render( pd3dDevice )))
				return E_FAIL;
		}		

		return S_OK;
	}

	HRESULT RenderShadow( LPDIRECT3DDEVICE8 pd3dDevice )
	{
		for (size_t i = 0; i < m_obiekty.GetCount(); i++)
		{
			if(FAILED(m_obiekty.GetAt(i)->RenderShadow( pd3dDevice )))
				return E_FAIL;
		}

		return S_OK;
	}

	HRESULT AddPrimitive( )
	{
		for (size_t i = 0; i < m_obiekty.GetCount(); i++)
		{
			if(FAILED(m_obiekty.GetAt(i)->AddPrimitive( )))
				return E_FAIL;
		}

		return S_OK;
	}


    virtual HRESULT InvalidateDeviceObjects( LPDIRECT3DDEVICE8 pd3dDevice = NULL )
	{
		for (size_t i = 0; i < m_obiekty.GetCount(); i++)
		{
			if(FAILED(m_obiekty.GetAt(i)->InvalidateDeviceObjects( pd3dDevice )))
				return E_FAIL;
		}

		return S_OK;
	}

	HRESULT RestoreDeviceObjects(LPDIRECT3DDEVICE8 pd3dDevice = NULL)
	{
		for (size_t i = 0; i < m_obiekty.GetCount(); i++)
		{
			if(FAILED(m_obiekty.GetAt(i)->RestoreDeviceObjects( pd3dDevice )))
				return E_FAIL;
		}

		return S_OK;
	}

	CObiektGrafArray m_obiekty;
};

LPCVOID ZaladujZasob(CONST INT pResID, PCTSTR pResTyp, DWORD & pLen);

#endif