// ###############################################################
// $wskaz$.h - plik tworz¹cy kolekcje trójk¹tów przysz³ych modeli.
// Wersja produkcyjnie nie korzysta z tego kodu.
// S³u¿y on wygenerowaniu obiektów typu mesh.
// ###############################################################



// ###########################################
// Nazwa: proporcje_wsk
// Opis: Struktura opisuj¹ca wygl¹d wskazówki.
// ###########################################
typedef 
struct 
{
	float szer_prostak;
	float szer_wsk; // Ko³o oraz koniczynka
	float szer_krzyza;
	float dlugosc_krzyza; // dlugoœæ grota mo¿na policzyæ z ró¿nicy
	float dlugosc_wsk;
}
proporcje_wsk;

// ##################################
// Nazwa: class CElemWskaz
// Opis: Element graficzny wskazówki.
// ##################################
class CElemWskaz : public CVertexPlain
{
public:
	CElemWskaz(
		const D3DXVECTOR3 p_srodek = D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		const D3DPRIMITIVETYPE p_rodzaj_opt = D3DPT_TRIANGLESTRIP,
		const DWORD p_il_trojk = 1) : // default...
	CVertexPlain(p_rodzaj_opt, p_il_trojk)
	{
		for(DWORD m_ind = 0; m_ind < m_il_punkt; m_ind ++)
		{
			m_tymczas[m_ind].nrm[0] =  0.0f;
			m_tymczas[m_ind].nrm[1] =  0.0f;
			m_tymczas[m_ind].nrm[2] = -1.0f;
		}
	}
};

// #########################
// Nazwa: class CTrojkat
// Opis: Trójk¹t,
//       fragment wskazówki.
// #########################
class CTrojkat : public CElemWskaz //CTriangle
{
public:
	CTrojkat(const D3DXVECTOR2 & a1, const D3DXVECTOR2 & a2, const D3DXVECTOR2 & a3, const D3DXVECTOR3 & p_srodek) :
	  CElemWskaz(p_srodek, D3DPT_TRIANGLELIST, 1)
	  {
		  m_tymczas[0].pos = D3DXVECTOR3(a1[0], a1[1], p_srodek[2]);
		  m_tymczas[1].pos = D3DXVECTOR3(a2[0], a2[1], p_srodek[2]);
		  m_tymczas[2].pos = D3DXVECTOR3(a3[0], a3[1], p_srodek[2]);
	  }
};

// #############################################
// Nazwa: class CCentrum
// Opis: Ko³o odtwarzane,
//       fragment wskazówki
// #############################################
class CCentrum : public CElemWskaz //CTriangleFan
{
public:
	CCentrum(const D3DXVECTOR2 p_rozm, const D3DXVECTOR3 & p_srodek)
		: CElemWskaz(p_srodek, D3DPT_TRIANGLEFAN, PRZEL(IL_KOLO)), m_centrum(PRZEL(IL_KOLO), p_rozm[0])
	{
		m_tymczas[0].pos = D3DXVECTOR3(0.0f, 0.0f, p_srodek[2]);
		DWORD m_ind = 0;
		while( m_ind <= m_il_trojk )
		{
			register float m_x = m_centrum.WezX(m_ind);
			register float m_y = m_centrum.WezY(m_ind);
			m_ind ++;
			m_tymczas[m_ind].pos = D3DXVECTOR3(m_x, m_y, p_srodek[2]);
		}
		m_tymczas[m_ind].pos = m_tymczas[1].pos;
	}
	CElipsa m_centrum;
};

// #############################################
// Nazwa: class CKoniczynka
// Opis: Koniczynka -
//       fragment wskazówki
// #############################################
class CKoniczynka : public CElemWskaz // CTriangleStrip
{
public:
	CKoniczynka(const proporcje_wsk & p_rozm, const D3DXVECTOR3 & p_srodek)
		: CElemWskaz(p_srodek, D3DPT_TRIANGLESTRIP, PRZEL(IL_W_KONICZ) << 3),
		m_luk_maly(PRZEL(IL_W_KONICZ), PROM_X_MALY, PROM_Y_MALY, KAT_KONICZ(WSP_MALA_KONICZ), START_KONICZ(WSP_MALA_KONICZ)),
		m_luk_duzy(PRZEL(IL_W_KONICZ), PROM_X_DUZY, PROM_Y_DUZY, KAT_KONICZ(WSP_DUZA_KONICZ), START_KONICZ(WSP_DUZA_KONICZ))
	{
		const float m_promien = m_luk_maly.WezX(m_luk_maly.WezIlBok()) + fabsf(m_luk_maly.WezY(m_luk_maly.WezIlBok()));
		const float m_promien_duzy = m_luk_duzy.WezX(m_luk_duzy.WezIlBok()) + fabsf(m_luk_duzy.WezY(m_luk_duzy.WezIlBok()));
		m_mnoznik = 
			(m_promien_duzy + m_luk_duzy.WezY(m_luk_duzy.WezIlBok() >> 1)) / p_rozm.szer_wsk;

		for(DWORD m_ind = 0; m_ind <= m_luk_maly.WezIlBok(); m_ind ++)
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
			m_tymczas[2 * m_ind + (m_luk_maly.WezIlBok() << 1)    ].pos = 
				D3DXVECTOR3(
				+m_y + m_promien, 
				-m_x,
				p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.WezIlBok() << 1) * 2].pos = 
				D3DXVECTOR3(
				-m_x, 
				-m_y - m_promien,
				p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.WezIlBok() << 1) * 3].pos = 
				D3DXVECTOR3(
				-m_y - m_promien, 
				+m_x,
				p_srodek[2]);

			m_tymczas[2 * m_ind                                  + 1].pos = 
				D3DXVECTOR3( 
				+m_x_duzy, 
				+m_y_duzy + m_promien_duzy,
				p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.WezIlBok() << 1)     + 1].pos = 
				D3DXVECTOR3(
				+m_y_duzy + m_promien_duzy, 
				-m_x_duzy,
				p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.WezIlBok() << 1) * 2 + 1].pos = 
				D3DXVECTOR3(
				-m_x_duzy, 
				-m_y_duzy - m_promien_duzy,
				p_srodek[2]);
			m_tymczas[2 * m_ind + (m_luk_maly.WezIlBok() << 1) * 3 + 1].pos = 
				D3DXVECTOR3(
				-m_y_duzy - m_promien_duzy, 
				+m_x_duzy,
				p_srodek[2]);
		}
		m_tymczas[m_il_punkt - 2] = m_tymczas[0];
		m_tymczas[m_il_punkt - 1] = m_tymczas[1];
		for(DWORD m_ind = 0; m_ind < m_il_punkt; m_ind ++)
		{
			m_tymczas[m_ind].pos[0] /= m_mnoznik;
			m_tymczas[m_ind].pos[1] /= m_mnoznik;
			m_tymczas[m_ind].pos[0] += p_rozm.dlugosc_krzyza + MalyPromienY();
		}
	}
	float MalyPromienY()
	{
		static const float m_promien_maly = 
			(m_luk_maly.WezX(m_luk_maly.WezIlBok()) 
			+ fabsf(m_luk_maly.WezY(m_luk_maly.WezIlBok()))
			+ fabsf(m_luk_maly.WezY(m_luk_maly.WezIlBok() >> 1)))
			/ m_mnoznik;
		return m_promien_maly;
	}

private:
	CElipsa m_luk_duzy, m_luk_maly;
	float m_mnoznik;
};

// #########################
// Nazwa: class CKrzyz
// Opis: Krzy¿ -
//       fragment wskazówki.
// #########################
class CKrzyz : public CElemWskaz //CTriangleFan
	// Klasa s³u¿y do rysowania krzy¿a z 12 trójk¹tów posiadaj¹cych punkt wspólny wewn¹trz figury
{
public:
	CKrzyz(const proporcje_wsk & p_rozm, const D3DXVECTOR3 & p_srodek)
		: CElemWskaz(p_srodek, D3DPT_TRIANGLEFAN, IL_W_KRZYZ)
	{
		float m_szer = p_rozm.szer_prostak;
		float m_szerkrzyz = p_rozm.szer_krzyza;
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

// #############################
// Nazwa: class CWskazowka
// Opis: Klasa bazowa wskazówek.
// #############################
class CWskazowka : public CKombajnGraf
{
public:
	CWskazowka(const proporcje_wsk p_rozm, const D3DXVECTOR3 & p_srodek, const INT p_ID) : CKombajnGraf(p_ID)
	{
		CKoniczynka * m_koni = new CKoniczynka(p_rozm, p_srodek);
		const float m_pocz_piora = m_koni->MalyPromienY() * 2.0f + p_rozm.dlugosc_krzyza;

		AddRenderedObject(m_koni);
		AddRenderedObject(new CCentrum(D3DXVECTOR2(p_rozm.szer_wsk * 0.9f, GODZ_Z), p_srodek));
		AddRenderedObject(new CKrzyz(p_rozm, p_srodek));
		AddRenderedObject(new CTrojkat(
			D3DXVECTOR2(m_pocz_piora, -p_rozm.szer_prostak * 0.8f),
			D3DXVECTOR2(m_pocz_piora, +p_rozm.szer_prostak * 0.8f),
			D3DXVECTOR2(p_rozm.dlugosc_wsk, 0.0f),
			p_srodek));
	}
	virtual float WezKat(const SYSTEMTIME & p_czas) = 0;
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		D3DXMATRIXA16 mat_wsk;
		//D3DXMatrixRotationZ(&mat_wsk, 1.5f * D3DX_PI + WezKat(p_czas));
		//D3DXMatrixMultiply(&mat_wsk, &mat_wsk, &g_world);
		//pd3dDevice->SetTransform(D3DTS_WORLD, &mat_wsk);
		CKombajnGraf::OnFrameRender( pd3dDevice );
	}

	// #####################################################
	// Nazwa: virtual void Oblicz(const SYSTEMTIME & p_czas)
	// Opis: Obraca uk³ad wspó³rzêdnych wskazówki
	//       i zachowuje wynik do narysowania.
	// #####################################################
	/*~CWskazowka()
	{
	}*/
};

// #########################
// Nazwa: class CWskazMin
// Opis: Wskazówka minutowa.
// #########################
class CWskazMin : public CWskazowka
{
public:
	CWskazMin() :
	  CWskazowka(Forma(), D3DXVECTOR3(0.0f, 0.0f, g_bWskazSek ? (GODZ_Z + MIN_Z) / 2 : MIN_Z), IDX_MIN_HAND)
	  {
#if defined(CPP_RAW_PRIMITIVE)
		  mFileName[0] = 'M';
		  mFileName[1] = 'i';
		  mFileName[2] = 'n';
		  mFileName[3] = '.';
		  mFileName[4] = 'x';
		  mFileName[5] = 0;
#endif
	  }
	  static proporcje_wsk Forma()
	  {
		  proporcje_wsk m_rozm;

		  m_rozm.dlugosc_wsk = 0.966f;
		  m_rozm.dlugosc_krzyza = 0.476f;
		  m_rozm.szer_krzyza = 0.045f;
		  m_rozm.szer_prostak = 0.023f;
		  m_rozm.szer_wsk = 0.118f;

		  return m_rozm;
	  }
private:
	virtual float WezKat(const SYSTEMTIME & p_czas)
	{
		return (float)
			// ((M_PI * 2 * (p_czas.wMinute * 60 + p_czas.wSecond)) / 3600);
			((M_PI_MUL_2 * (p_czas.wMinute)) / 60);
	}
};

// ##########################
// Nazwa: class CWskazSec
// Opis: Wskazówka sekundowa.
// ##########################
class CWskazSec : public CWskazowka
{
public:
	CWskazSec() :
	  CWskazowka(Forma(), D3DXVECTOR3(0.0f, 0.0f, MIN_Z), IDX_SEC_HAND)
	  {
#if defined(CPP_RAW_PRIMITIVE)
		  mFileName[0] = 'S';
		  mFileName[1] = 'e';
		  mFileName[2] = 'c';
		  mFileName[3] = '.';
		  mFileName[4] = 'x';
		  mFileName[5] = 0;
#endif
	  }
	  static proporcje_wsk Forma()
	  {
		  proporcje_wsk m_rozm;

		  m_rozm.dlugosc_wsk = 0.966f;
		  m_rozm.dlugosc_krzyza = 0.477f;
		  m_rozm.szer_krzyza = 0.040f;
		  m_rozm.szer_prostak = 0.020f;
		  m_rozm.szer_wsk = 0.106f;

		  return m_rozm;
	  }
private:
	virtual float WezKat(const SYSTEMTIME & p_czas)
	{
		return (float)
			((M_PI_MUL_2 * (p_czas.wSecond)) / 60);
	}
};

// ##########################
// Nazwa: class CWskazGodz
// Opis: Wskazówka godzinowa.
// ##########################
class CWskazGodz : public CWskazowka
{
public:
	CWskazGodz() :
	  CWskazowka(Forma(), StrzalkaPoz(), IDX_HOUR_HAND)
	  {
#if defined(CPP_RAW_PRIMITIVE)
		  mFileName[0] = 'H';
		  mFileName[1] = 'o';
		  mFileName[2] = 'u';
		  mFileName[3] = 'r';
		  mFileName[4] = '.';
		  mFileName[5] = 'x';
		  mFileName[6] = 0;
#endif

		  AddRenderedObject(new CTrojkat( // Ogon wskazówki godzinowej
			  D3DXVECTOR2(0.0f, +Forma().szer_prostak * 1.2f),
			  D3DXVECTOR2(0.0f, -Forma().szer_prostak * 1.2f),
			  D3DXVECTOR2(-Forma().szer_wsk * 1.9f, 0.0f),
			  StrzalkaPoz()));
	  }
	  static D3DXVECTOR3 StrzalkaPoz()
	  {
		  static D3DXVECTOR3 V = D3DXVECTOR3(0.0f, 0.0f, GODZ_Z);
		  return V;
	  }
	  static proporcje_wsk Forma()
	  {
		  proporcje_wsk m_rozm;

		  m_rozm.dlugosc_wsk = 0.70f;
		  m_rozm.dlugosc_krzyza = 0.3f;
		  m_rozm.szer_krzyza = 0.052f;
		  m_rozm.szer_prostak = 0.026f;
		  m_rozm.szer_wsk = 0.132f;

		  return m_rozm;
	  }
private:
	float WezKat(const SYSTEMTIME & p_czas)
	{
		return (float)
			((M_PI_MUL_2 * ((p_czas.wHour % 12) * 60 + p_czas.wMinute)) / (12 * 60));
	}
};
