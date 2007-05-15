// ##################################################################
// $nakretka$.h - plik tworz¹cy kolekcje trójk¹tów przysz³ych modeli.
// Wersja produkcyjnie nie korzysta z tego kodu.
// S³u¿y on wygenerowaniu obiektów typu mesh.
// ##################################################################



// #############################################
// Nazwa: proporcje_nakretki
// Opis: Ró¿ne promienie dla obiektu pierwszoplanowego (CMontaz)
//       Nakretka, gwiazdka i in.
// #############################################
typedef 
struct 
{
	float maly_r;
	float wiekszy_r;
	float duzy_r;
	float gwint_r; // Najwiêkszy promieñ œruby
	float koniec_r; // Promieñ przeciêcia sto¿ka
}
proporcje_nakretki;

// #############################################
// Nazwa: class CMontaz
// Opis: Zbiór ró¿nych elementów graficznych
//       Nakretka, gwiazdka i in.
// #############################################
class CMontazMosiadz : public CKombajnGraf
{
public:
	CMontazMosiadz() : CKombajnGraf(IDX_BRASS)
	{
		D3DXVECTOR3 m_srodek = D3DXVECTOR3(0.0f, 0.0f, STOZEK_Z);
		proporcje_nakretki m_rozm;
		m_rozm.duzy_r = 0.08f;
		m_rozm.wiekszy_r = 0.075f;
		m_rozm.maly_r = 0.05f;
		m_rozm.gwint_r = 0.027f;
		m_rozm.koniec_r = 0.0139f;

		AddRenderedObject(new CStozek(m_srodek, m_rozm));
		AddRenderedObject(new CNakretka(m_srodek, m_rozm));
		mFileName[0] = 'B';
		mFileName[1] = 'r';
		mFileName[2] = 'a';
		mFileName[3] = 's';
		mFileName[4] = 's';
		mFileName[5] = '.';
		mFileName[6] = 'x';
		mFileName[7] = 0;
	}
private:
	class CStozek : public CVertexTex
	{
	public:
		CStozek(
			const D3DXVECTOR3 & p_srodek, 
			const proporcje_nakretki & p_rozm) :
		CVertexTex(D3DPT_TRIANGLESTRIP, 36 << 2)
		{
			CElipsa m_okrag(36 << 1, 1.0f);
			m_rozm = p_rozm;

			for(DWORD m_ind = 0; m_ind < m_okrag.WezIlBok(); m_ind ++)
			{
				DWORD m_mul_ind = m_ind << 1;			
				m_tymczas[m_mul_ind    ].pos = D3DXVECTOR3(
					m_rozm.maly_r * m_okrag.WezX(m_ind),
					m_rozm.maly_r * m_okrag.WezY(m_ind),
					p_srodek[2]);
				m_tymczas[m_mul_ind    ].tu = (1.0f + 0.2f * m_okrag.WezX(m_ind)) / 2; 
				m_tymczas[m_mul_ind    ].tv = (1.0f - 0.2f * m_okrag.WezY(m_ind)) / 2;

				float m_r = (m_ind & 1 ? m_rozm.duzy_r : m_rozm.wiekszy_r);
				float m_x = m_r - m_rozm.maly_r;
				float m_z = m_r * WSP_KATA;

				m_tymczas[m_mul_ind + 1].pos = D3DXVECTOR3(
					m_r * m_okrag.WezX(m_ind),
					m_r * m_okrag.WezY(m_ind),
					- m_z + p_srodek[2]);
				m_tymczas[m_mul_ind + 1].tu = (1.0f + m_okrag.WezX(m_ind)) / 2; 
				m_tymczas[m_mul_ind + 1].tv = (1.0f - m_okrag.WezY(m_ind)) / 2;

				D3DXVECTOR3 tmp;
				tmp = m_tymczas[m_mul_ind + 1].pos - m_tymczas[m_mul_ind].pos;
				m_tymczas[m_mul_ind + 1].nrm = * D3DXVec3Normalize(&(m_tymczas[m_mul_ind    ].nrm),
					&(D3DXVECTOR3(m_z * tmp[0] / m_x, m_z * tmp[1] / m_x, - m_x)));
			}
			m_tymczas[    m_okrag.WezIlBok() * 2] = m_tymczas[0];
			m_tymczas[1 + m_okrag.WezIlBok() * 2] = m_tymczas[1];
		}

	protected:
		proporcje_nakretki m_rozm;
	};

	class CNakretka : public CVertexTex
	{
	public:
		CNakretka(
			const D3DXVECTOR3 & p_srodek, 
			const proporcje_nakretki & p_rozm) :
		CVertexTex(D3DPT_TRIANGLEFAN, 6 - 1)
		{
			CElipsa m_okrag(6, 1.0f, NULL, M_PI_MUL_2, -0.73f);
			m_rozm = p_rozm;

			for(DWORD m_ind = 0; m_ind < m_okrag.WezIlBok(); m_ind ++)
			{
				m_tymczas[m_ind].pos = D3DXVECTOR3(
					m_okrag.WezX(m_ind) * p_rozm.maly_r * 1.0f,
					m_okrag.WezY(m_ind) * p_rozm.maly_r * 1.0f,
					p_srodek[2] + .03f);
				m_tymczas[m_ind].tu = (1.0f + m_okrag.WezX(m_ind)) / 2; 
				m_tymczas[m_ind].tv = (1.0f + m_okrag.WezY(m_ind)) / 2;
				m_tymczas[m_ind].nrm = D3DXVECTOR3(0.0f, 0.0f, -1.0f); 
			}
			m_tymczas[m_okrag.WezIlBok()] = m_tymczas[0];
		}
	protected:
		proporcje_nakretki m_rozm;
	};
};

// #############################################
// Nazwa: class CMontaz
// Opis: Zbiór ró¿nych elementów graficznych
//       Nakretka, gwiazdka i in.
// #############################################
class CMontazStal : public CKombajnGraf
{
public:
	CMontazStal():CKombajnGraf(IDX_STEEL)
	{
#if defined(CPP_RAW_PRIMITIVE)
		mFileName[0] = 'S';
		mFileName[1] = 't';
		mFileName[2] = 'e';
		mFileName[3] = 'e';
		mFileName[4] = 'l';
		mFileName[5] = '.';
		mFileName[6] = 'x';
		mFileName[7] = 0;
#endif
		D3DXVECTOR3 m_srodek = D3DXVECTOR3(0.0f, 0.0f, STOZEK_Z);
		proporcje_nakretki m_rozm;
		m_rozm.duzy_r = 0.08f;
		m_rozm.wiekszy_r = 0.075f;
		m_rozm.maly_r = 0.05f;
		m_rozm.gwint_r = 0.027f;
		m_rozm.koniec_r = 0.0139f;

		CGwint * tmp = new CGwint(m_srodek, m_rozm);
		AddRenderedObject(tmp);
		AddRenderedObject(new CKoniec(m_srodek, m_rozm, tmp->m_tymczas, tmp->m_il_trojk));
	}
private:
	class CGwint : public CVertexTex
	{
	public:
		CGwint(
			const D3DXVECTOR3 & p_srodek, 
			const proporcje_nakretki & p_rozm) :
		CVertexTex(D3DPT_TRIANGLESTRIP, PRZEL(48) << 1)
		{
			CElipsa m_okrag(PRZEL(48), 1.0f);

			for(DWORD m_ind = 0; m_ind < m_okrag.WezIlBok(); m_ind ++)
			{
				DWORD m_mul_ind = m_ind << 1;			
				m_tymczas[m_mul_ind    ].pos = D3DXVECTOR3(
					p_rozm.koniec_r * m_okrag.WezX(m_ind),
					p_rozm.koniec_r * m_okrag.WezY(m_ind),
					p_srodek[2] + 0.07f);
				m_tymczas[m_mul_ind    ].tu = (1.0f + 0.2f * m_okrag.WezX(m_ind)) / 2; 
				m_tymczas[m_mul_ind    ].tv = (1.0f - 0.2f * m_okrag.WezY(m_ind)) / 2;

				static float m_x = p_rozm.gwint_r - p_rozm.koniec_r;
				static float m_z = p_rozm.gwint_r * WSP_KATA_SRUBY;

				m_tymczas[m_mul_ind + 1].pos = D3DXVECTOR3(
					p_rozm.gwint_r * m_okrag.WezX(m_ind),
					p_rozm.gwint_r * m_okrag.WezY(m_ind),
					- m_z + p_srodek[2] + 0.07f);
				m_tymczas[m_mul_ind + 1].tu = (1.0f + m_okrag.WezX(m_ind)) / 2; 
				m_tymczas[m_mul_ind + 1].tv = (1.0f - m_okrag.WezY(m_ind)) / 2;

				D3DXVECTOR3 tmp;
				tmp = m_tymczas[m_mul_ind + 1].pos - m_tymczas[m_mul_ind].pos;
				m_tymczas[m_mul_ind + 1].nrm = * D3DXVec3Normalize(&(m_tymczas[m_mul_ind    ].nrm), 
					&(D3DXVECTOR3(m_z * tmp[0] / m_x, m_z * tmp[1] / m_x, - m_x)));
			}
			m_tymczas[    m_okrag.WezIlBok() * 2] = m_tymczas[0];
			m_tymczas[1 + m_okrag.WezIlBok() * 2] = m_tymczas[1];
		}
	};

	class CKoniec : public CVertexTex
	{
	public:
		CKoniec(
			const D3DXVECTOR3 & p_srodek, 
			const proporcje_nakretki & p_rozm,
			const VERTEXTEX * p_tymczas,
			const DWORD p_il_trojk) :
		CVertexTex(D3DPT_TRIANGLEFAN, ((p_il_trojk >> 1) - 1))
		{
			DWORD m_ind;

			for(m_ind = 0; m_ind <= m_il_trojk; m_ind ++)
			{
				m_tymczas[m_ind].pos = p_tymczas[m_ind << 1].pos;
				m_tymczas[m_ind].nrm = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
				m_tymczas[m_ind].tu = (1.0f + m_tymczas[m_ind].pos[0] / (p_rozm.koniec_r * 2)) / 2;
				m_tymczas[m_ind].tv = (1.0f + m_tymczas[m_ind].pos[1] / (p_rozm.koniec_r * 2)) / 2;
			}
			m_tymczas[m_ind] = m_tymczas[0];
		}
	};
};