#ifndef _WSKAZ_H_
#define _WSKAZ_H_

#include "stdafx.h"

// #############################################
// Nazwa: proporcje_wsk
// Opis: Struktura opisuj¹ca wygl¹d wskazówki
// #############################################
typedef 
	struct 
	{
		float szer_prostak;
		float szer_wsk; // Kolo z centrum oraz koniczynka
		float szer_krzyza;
		float dlugosc_krzyza; // dlugosc grota mozna policzyc z roznicy
		float dlugosc_wsk;
		D3DMATERIAL8 * wsk_mtrl;
	}
	proporcje_wsk;

// #############################################
// Nazwa: class CElemWskaz
// Opis: Element graficzny wskazówki
// #############################################
class CElemWskaz : public CVertexPlain
{
public:
	CElemWskaz(
		const D3DXVECTOR3 p_srodek = D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		const D3DPRIMITIVETYPE p_rodzaj_opt = D3DPT_TRIANGLESTRIP,
		ULONG p_nrkolejny = __COUNTER__) : // default...
	CVertexPlain(p_srodek, p_rodzaj_opt, p_nrkolejny)
	{
	}
    VOID Reset(D3DVECTOR & pLight)
	{
		m_wolumin->Reset( pLight );
	}

protected:
	virtual HRESULT Create(const WORD p_il_trojk)
	{
		if(SUCCEEDED(CVertexPlain::Create(p_il_trojk)))
		{
			for(WORD m_ind = 0; m_ind < m_il_punkt; m_ind ++)
			{
				m_tymczas[m_ind].nrm[0] =  0.0f;
				m_tymczas[m_ind].nrm[1] =  0.0f;
				m_tymczas[m_ind].nrm[2] = -1.0f;
				//m_tymczas[m_ind].pos[2] = m_srodek[2];
			}
			return S_OK;
		}
		else
			return E_FAIL;
	}
	/*~CElemWskaz()
	{
		delete [] m_tymczas;
		delete [] m_tymczas;
	}*/
};
/*	HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice )
	{
		//pd3dDevice->SetVertexShader( NULL );
		pd3dDevice->SetVertexShader( FormatFigury::FVF );
		pd3dDevice->DrawPrimitiveUP(
			PodajTyp(),
			m_il_trojk,
			m_tymczas,
			sizeof(FormatFigury));

		return S_OK;
	}
};*/

// #############################################
// Nazwa: class CTrojkat
// Opis: Trójk¹t
//       Fragment wskazówki
// #############################################
class CTrojkat : public CElemWskaz //CTriangle
{
public:
	CTrojkat(const D3DXVECTOR2 & a1, const D3DXVECTOR2 & a2, const D3DXVECTOR2 & a3, const D3DXVECTOR3 & p_srodek) :
		CElemWskaz(p_srodek, D3DPT_TRIANGLELIST)
	{
		Create(1);
		m_tymczas[0].pos = D3DXVECTOR3(a1[0], a1[1], p_srodek[2]);
		m_tymczas[1].pos = D3DXVECTOR3(a2[0], a2[1], p_srodek[2]);
		m_tymczas[2].pos = D3DXVECTOR3(a3[0], a3[1], p_srodek[2]);

	}
};

// #############################################
// Nazwa: class CCentrum
// Opis: Kolo odtwarzane
//       Fragment wskazówki
// #############################################
class CCentrum : public CElemWskaz //CTriangleFan
{
public:
	CCentrum(const D3DXVECTOR2 p_rozm, const D3DXVECTOR3 & p_srodek)
		: CElemWskaz(p_srodek, D3DPT_TRIANGLEFAN), m_centrum(PRZEL(IL_KOLO), p_rozm[0])
	{
		Create(m_centrum.m_il_bok);

		m_tymczas[0].pos = D3DXVECTOR3(0.0f, 0.0f, p_srodek[2]);
		WORD m_ind = 0;
		while( m_ind <= m_il_trojk )
		{
			register float m_x = m_centrum.m_punkty[m_ind][0];
			register float m_y = m_centrum.m_punkty[m_ind][1];
			m_ind ++;
			m_tymczas[m_ind].pos = D3DXVECTOR3(m_x, m_y, p_srodek[2]);
		}
		m_tymczas[m_ind].pos = m_tymczas[1].pos;
	}
	CElipsa m_centrum;
};

// #############################################
// Nazwa: class CKoniczynka
// Opis: Koniczynka
//       Fragment wskazówki
// #############################################
class CKoniczynka : public CElemWskaz // CTriangleStrip
{
public:
	CKoniczynka(const proporcje_wsk & p_rozm, const D3DXVECTOR3 & p_srodek)
		: CElemWskaz(p_srodek, D3DPT_TRIANGLESTRIP),
		m_luk_maly(PRZEL(IL_W_KONICZ), PROM_X_MALY, PROM_Y_MALY, KAT_KONICZ(WSP_MALA_KONICZ), START_KONICZ(WSP_MALA_KONICZ)),
		m_luk_duzy(PRZEL(IL_W_KONICZ), PROM_X_DUZY, PROM_Y_DUZY, KAT_KONICZ(WSP_DUZA_KONICZ), START_KONICZ(WSP_DUZA_KONICZ))
	{
		const float m_promien = m_luk_maly.WezX(m_luk_maly.m_il_bok) + fabsf(m_luk_maly.WezY(m_luk_maly.m_il_bok));
		const float m_promien_duzy = m_luk_duzy.WezX(m_luk_duzy.m_il_bok) + fabsf(m_luk_duzy.WezY(m_luk_duzy.m_il_bok));
		m_mnoznik = 
			(m_promien_duzy + m_luk_duzy.WezY(m_luk_duzy.m_il_bok >> 1)) / p_rozm.szer_wsk;
		Create(m_luk_maly.m_il_bok << 3); /* + zamkniêcie */;

		for(WORD m_ind = 0; m_ind <= m_luk_maly.m_il_bok; m_ind ++)
		{
			const float m_x = m_luk_maly.WezX(m_ind);
			const float m_y = m_luk_maly.WezY(m_ind);
			const float m_x_duzy = m_luk_duzy.WezX(m_ind);
			const float m_y_duzy = m_luk_duzy.WezY(m_ind);
			
			m_tymczas[2 * m_ind                                 ].pos = 
				D3DXVECTOR3(
					+m_x, 
					+m_y + m_promien,
					p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.m_il_bok << 1)    ].pos = 
				D3DXVECTOR3(
					+m_y + m_promien, 
					-m_x,
					p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.m_il_bok << 1) * 2].pos = 
				D3DXVECTOR3(
					-m_x, 
					-m_y - m_promien,
					p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.m_il_bok << 1) * 3].pos = 
				D3DXVECTOR3(
					-m_y - m_promien, 
					+m_x,
					p_srodek[2]);

			m_tymczas[2 * m_ind                                  + 1].pos = 
				D3DXVECTOR3( 
					+m_x_duzy, 
					+m_y_duzy + m_promien_duzy,
					p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.m_il_bok << 1)     + 1].pos = 
				D3DXVECTOR3(
					+m_y_duzy + m_promien_duzy, 
					-m_x_duzy,
					p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.m_il_bok << 1) * 2 + 1].pos = 
				D3DXVECTOR3(
					-m_x_duzy, 
					-m_y_duzy - m_promien_duzy,
					p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.m_il_bok << 1) * 3 + 1].pos = 
				D3DXVECTOR3(
					-m_y_duzy - m_promien_duzy, 
					+m_x_duzy,
					p_srodek[2]);
		}
		m_tymczas[m_il_punkt - 2] = m_tymczas[0];
		m_tymczas[m_il_punkt - 1] = m_tymczas[1];
		for(WORD m_ind = 0; m_ind < m_il_punkt; m_ind ++)
		{
			m_tymczas[m_ind].pos[0] /= m_mnoznik;
			m_tymczas[m_ind].pos[1] /= m_mnoznik;
			m_tymczas[m_ind].pos[0] += p_rozm.dlugosc_krzyza + MalyPromienY();
		}		
	}
	float MalyPromienY()
	{
		static const float m_promien_maly = 
			(m_luk_maly.WezX(m_luk_maly.m_il_bok) 
			+ fabsf(m_luk_maly.WezY(m_luk_maly.m_il_bok))
			+ fabsf(m_luk_maly.WezY(m_luk_maly.m_il_bok >> 1)))
			/ m_mnoznik;
		return m_promien_maly;
	}

private:
	CElipsa m_luk_duzy, m_luk_maly;
	float m_mnoznik;
};

// #############################################
// Nazwa: class CKrzyz
// Opis: Krzyz rysowany z u¿yciem triange fan
//       Fragment wskazówki
// #############################################
class CKrzyz : public CElemWskaz //CTriangleFan
	// Klasa s³u¿y do rysowania krzyza z 12 trójk¹tów posiadaj¹cych punkt wspólny wewn¹trz figury
{
public:
	CKrzyz(const proporcje_wsk & p_rozm, const D3DXVECTOR3 & p_srodek)
		: CElemWskaz(p_srodek, D3DPT_TRIANGLEFAN)
	{
		float m_szer = p_rozm.szer_prostak;
		float m_szerkrzyz = p_rozm.szer_krzyza;
		Create(IL_W_KRZYZ);
		m_start = p_rozm.dlugosc_krzyza * START;
		/*		 X - "ten element wystaje"
		XXXXXXXXX0XXXX
				 X*/
		m_tymczas[ 0].pos = D3DXVECTOR3(m_start, 0.0f, p_srodek[2]);
		m_tymczas[ 1].pos = D3DXVECTOR3(0.0f, -m_szer, p_srodek[2]);
		m_tymczas[ 2].pos = D3DXVECTOR3(0.0f, +m_szer, p_srodek[2]);
		m_tymczas[ 3].pos = D3DXVECTOR3(m_start - m_szer, +m_szer, p_srodek[2]);
		m_tymczas[ 4].pos = D3DXVECTOR3(m_start - m_szer, +m_szerkrzyz, p_srodek[2]);
		m_tymczas[ 5].pos = D3DXVECTOR3(m_start + m_szer, +m_szerkrzyz, p_srodek[2]);
		m_tymczas[ 6].pos = D3DXVECTOR3(m_start + m_szer, +m_szer, p_srodek[2]);
		m_tymczas[ 7].pos = D3DXVECTOR3(p_rozm.dlugosc_krzyza, +m_szer, p_srodek[2]);
		m_tymczas[ 8].pos = D3DXVECTOR3(p_rozm.dlugosc_krzyza, -m_szer, p_srodek[2]);
		m_tymczas[ 9].pos = D3DXVECTOR3(m_start + m_szer, -m_szer, p_srodek[2]);
		m_tymczas[10].pos = D3DXVECTOR3(m_start + m_szer, -m_szerkrzyz, p_srodek[2]);
		m_tymczas[11].pos = D3DXVECTOR3(m_start - m_szer, -m_szerkrzyz, p_srodek[2]);
		m_tymczas[12].pos = D3DXVECTOR3(m_start - m_szer, -m_szer, p_srodek[2]);
		m_tymczas[13].pos = D3DXVECTOR3(0.0f, -m_szer, p_srodek[2]);
	}
	D3DXVECTOR3 m_rozm;
	float m_start;
};

// #############################################
// Nazwa: class CWskazowka
// Opis: Klasa bazowa wskazówek
// #############################################
class CWskazowka : public CKombajnGraf
{
public:
	CWskazowka(const proporcje_wsk & p_rozm, const D3DXVECTOR3 & p_srodek) : CKombajnGraf(p_srodek)

	{

		m_rozm = p_rozm;
		m_fPoprzWart = -100.0f;
		CKoniczynka * m_koni = new CKoniczynka(p_rozm, p_srodek);
		const float m_pocz_piora = m_koni->MalyPromienY() * 2.0f + p_rozm.dlugosc_krzyza;
		wsk_mtrl = p_rozm.wsk_mtrl;

		m_obiekty.Add(m_koni);
		m_obiekty.Add(new CCentrum(D3DXVECTOR2(p_rozm.szer_wsk * 0.9f, GODZ_Z), p_srodek));
		m_obiekty.Add(new CKrzyz(p_rozm, p_srodek));
		m_obiekty.Add(new CTrojkat(
			D3DXVECTOR2(m_pocz_piora, -p_rozm.szer_prostak * 0.8f),
			D3DXVECTOR2(m_pocz_piora, +p_rozm.szer_prostak * 0.8f),
			D3DXVECTOR2(p_rozm.dlugosc_wsk, 0.0f),
			p_srodek));
	}

	// #############################################
	// Nazwa: virtual void Oblicz(const SYSTEMTIME & p_czas)
	// Opis: Obraca wspó³rzêdne wskazówki
	//       i zachowuje wynik do narysowania
	// #############################################
	virtual HRESULT Oblicz(const SYSTEMTIME & p_czas)
	{
		float tmp = WezKat(p_czas);
		if( tmp != m_fPoprzWart && g_bUseDepthBuffer )
		{
			m_fPoprzWart = tmp;
			for (size_t i = 0; i < m_obiekty.GetCount(); i++)
			{
				((CElemWskaz *) m_obiekty.GetAt(i))->Reset(
					D3DXVECTOR3(- sinf(WezKat()) * light.Direction.x - cosf(WezKat()) * light.Direction.y,
					- cosf(WezKat()) * light.Direction.x + sinf(WezKat()) * light.Direction.y,
					light.Direction.z));
				m_obiekty.GetAt(i)->AddPrimitive();
			}
		}
		return S_OK;
	}
	void Rotacja( LPDIRECT3DDEVICE8 pd3dDevice )
	{
		D3DXMATRIXA16 mat_wsk;
		D3DXMatrixRotationZ(&mat_wsk, WezKat());
		D3DXMatrixMultiply(&mat_wsk, &mat_wsk, &g_world);
		pd3dDevice->SetTransform(D3DTS_WORLD, &mat_wsk);
	}
	HRESULT RenderShadow( LPDIRECT3DDEVICE8 pd3dDevice )
	{
		Rotacja( pd3dDevice );
		for (size_t i = 0; i < m_obiekty.GetCount(); i++)
		{
			if(FAILED(m_obiekty.GetAt(i)->RenderShadow( pd3dDevice )))
				return E_FAIL;
		}
		return S_OK;
	}
	HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice )
	{
		Rotacja( pd3dDevice );
		pd3dDevice->SetMaterial( wsk_mtrl );
		return CKombajnGraf::Render( pd3dDevice );
	}

	virtual float WezKat(const SYSTEMTIME & p_czas) = 0;
	float WezKat()
	{
		return - m_fPoprzWart + M_PI_DIV_2;
	}

	/*~CWskazowka()
	{
	}*/

protected:
	proporcje_wsk m_rozm;
	float m_fPoprzWart;
	D3DMATERIAL8 * wsk_mtrl;
};

// #############################################
// Nazwa: class CWskazMin
// Opis: Wskazówka minutowa
// #############################################
class CWskazMin : public CWskazowka
{
public:
	CWskazMin(const proporcje_wsk & p_rozm, const D3DXVECTOR3 & p_srodek) :
	    CWskazowka(p_rozm, p_srodek)
	{
	}
	virtual float WezKat(const SYSTEMTIME & p_czas)
	{
		return (float)
			// ((M_PI * 2 * (p_czas.wMinute * 60 + p_czas.wSecond)) / 3600);
			((M_PI * 2 * (p_czas.wMinute)) / 60);
	}
};

// #############################################
// Nazwa: class CWskazSec
// Opis: Wskazówka sekundowa
// #############################################
class CWskazSec : public CWskazowka
{
public:
	CWskazSec(const proporcje_wsk & p_rozm, const D3DXVECTOR3 & p_srodek) :
	    CWskazowka(p_rozm, p_srodek)
	{
	}
	virtual float WezKat(const SYSTEMTIME & p_czas)
	{
		return (float)
			((M_PI * 2 * (p_czas.wSecond)) / 60);
	}
};

// #############################################
// Nazwa: class CWskazGodz
// Opis: Wskazówka godzinowa
// #############################################
class CWskazGodz : public CWskazowka
{
public:
	CWskazGodz(const proporcje_wsk & p_rozm, const D3DXVECTOR3 & p_srodek) :
		CWskazowka(p_rozm, p_srodek)
	{
		m_obiekty.Add(new CTrojkat( // Ogon wskazówki godzinowej
			D3DXVECTOR2(0.0f, +p_rozm.szer_prostak * 1.2f),
			D3DXVECTOR2(0.0f, -p_rozm.szer_prostak * 1.2f),
			D3DXVECTOR2(-p_rozm.szer_wsk * 1.9f, 0.0f),
			p_srodek));
	}
	float WezKat(const SYSTEMTIME & p_czas)
	{
		return (float)
			((M_PI * 2 * ((p_czas.wHour % 12) * 60 + p_czas.wMinute)) / (12 * 60));
	}
};

#endif