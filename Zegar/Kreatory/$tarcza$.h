// ################################################################
// $tarcza$.h - plik tworz¹cy kolekcje trójk¹tów przysz³ych modeli.
// Wersja produkcyjnie nie korzysta z tego kodu.
// S³u¿y on wygenerowaniu obiektów typu mesh.
// ################################################################



// ####################################
// Nazwa: class CTarcza
// Opis: Klasa przechowuj¹ca i rysuj¹ca
//       tarczê zegarow¹
// ####################################
class CTarcza : public CKombajnGraf
{
public:
	CTarcza() : CKombajnGraf(IDX_FACE)
	{
#if defined(CPP_RAW_PRIMITIVE)
		mFileName[0] = 'F';
		mFileName[1] = 'a';
		mFileName[2] = 'c';
		mFileName[3] = 'e';
		mFileName[4] = '.';
		mFileName[5] = 'x';
		mFileName[6] = 0;
#endif

		const D3DXVECTOR3 m_rozm = D3DXVECTOR3(1.0f, 1.0f, WYCOFANIE);
		CElipsa m_wycinek(PRZEL(IL), 1.0f, 1.0f);
		CElipsa Przekroj(m_wycinek.WezIlBok(), m_rozm[0], m_rozm[2], M_PI_DIV_2);

		// Kolejne fragmenty zakrzywienia
		DWORD m_ind = m_wycinek.WezIlBok() - 1;
		AddRenderedObject(new CKolo
			(
			&m_wycinek,
			WezR(fabsf(Przekroj.WezX(m_ind)), m_rozm),
			WezZ(fabsf(Przekroj.WezY(m_ind)), m_rozm),
			m_rozm));
		while(m_ind)
		{
			AddRenderedObject(new CDingo
				(	&m_wycinek,
				WezR(fabsf(Przekroj.WezX(m_ind)), m_rozm),
				WezZ(fabsf(Przekroj.WezY(m_ind)), m_rozm), 
				(CKombajnTarcza *) GetAt(GetSize() - 1),
				m_rozm));
			m_ind --;
		}
	}
private:
	float WezZ(const float p_z, const D3DXVECTOR3 & p_rozm)
	{
		return p_z;
	}
	float WezR(const float p_x, const D3DXVECTOR3 & p_rozm)
	{
		return powf(p_x / p_rozm[0], WSPOLCZ_PRZEKROJ);
	}
	class CKombajnTarcza : public CVertexTex
	{
	public:
		CKombajnTarcza(
			CElipsa * p_otocz, 
			float p_promien, 
			float p_wycofanie, 
			const D3DXVECTOR3 & p_tarcza,
			const D3DPRIMITIVETYPE p_rodzaj_opt,
			const DWORD p_il_trojk) :
		CVertexTex(p_rodzaj_opt, p_il_trojk) 
		{
			m_promien = p_promien;
			m_wycofanie = p_wycofanie;
		}
		float TexX(const float p_x, const D3DXVECTOR3 & p_tarcza)
		{
			return (1.0f + 0.99f * p_x / p_tarcza[0]) / 2.0f;
		}
		float TexY(const float p_y, const D3DXVECTOR3 & p_tarcza)
		{
			return (1.0f - 0.99f * p_y / p_tarcza[1]) / 2.0f;
		}
		D3DXVECTOR3 WezAntyR(D3DXVECTOR3 & p_poz, const float p_r) // Funkcja oblicza odwrotn¹ potêgê ni¿ WezR()
		{
			D3DXVECTOR3 tmp;
			float m_r = powf(p_r/* / m_rozm[0]*/, 1.0f / (WSPOLCZ_PRZEKROJ * WSPOLCZ_PRZEKROJ));
			return * D3DXVec3Normalize(&tmp,
				&D3DXVECTOR3(
				p_poz[0] * m_r,
				p_poz[1] * m_r,
				- p_poz[2] / (WYCOFANIE * WYCOFANIE)));
		}

		float m_wycofanie, m_promien;
	};

	class CKolo : public CKombajnTarcza
	{
	public:
		CKolo(CElipsa * p_otocz, float p_promien, float p_wycofanie, const D3DXVECTOR3 & p_tarcza) :
		  CKombajnTarcza(p_otocz, p_promien, p_wycofanie, p_tarcza, D3DPT_TRIANGLEFAN, p_otocz->WezIlBok())
		  {
			  m_tymczas[0].pos = D3DXVECTOR3(0.0f, 0.0f, m_wycofanie);
			  m_tymczas[0].nrm[0] =  0.0f;
			  m_tymczas[0].nrm[1] =  0.0f;
			  m_tymczas[0].nrm[2] = -1.0f;
			  m_tymczas[0].tu = 0.5f;
			  m_tymczas[0].tv = 0.5f;
			  for(DWORD m_ind = 1; m_ind <= m_il_trojk; m_ind ++)
			  {
				  register float m_x = p_otocz->WezX(m_ind) * m_promien;
				  register float m_y = p_otocz->WezY(m_ind) * m_promien;
				  m_tymczas[m_ind].pos = D3DXVECTOR3(
					  m_x,
					  m_y,
					  m_wycofanie);
				  m_tymczas[m_ind].nrm = WezAntyR(m_tymczas[m_ind].pos, p_otocz->m_promien_x);
				  m_tymczas[m_ind].tu = TexX(m_x, p_tarcza);
				  m_tymczas[m_ind].tv = TexY(m_y, p_tarcza);					
			  }
			  m_tymczas[m_il_punkt - 1] = m_tymczas[1];
		  }
	};

	class CDingo : public CKombajnTarcza
	{
	public:
		CDingo(CElipsa * p_otocz, float p_promien, float p_wycofanie, CKombajnTarcza * p_mniejszy, const D3DXVECTOR3 & p_tarcza) :
		  CKombajnTarcza(p_otocz, p_promien, p_wycofanie, p_tarcza, D3DPT_TRIANGLESTRIP, p_otocz->WezIlBok() << 1)
		  {
			  for(DWORD m_ind = 0; m_ind < p_otocz->WezIlBok(); m_ind ++)
			  {
				  DWORD m_mul_ind = m_ind << 1;			
				  m_tymczas[m_mul_ind + 0].pos = D3DXVECTOR3(
					  p_otocz->WezX(m_ind) * p_mniejszy -> m_promien,
					  p_otocz->WezY(m_ind) * p_mniejszy -> m_promien,
					  p_mniejszy -> m_wycofanie);
				  m_tymczas[m_mul_ind + 0].nrm = WezAntyR(m_tymczas[m_mul_ind].pos, p_mniejszy -> m_promien);
				  m_tymczas[m_mul_ind + 0].tu = TexX(p_otocz->WezX(m_ind) * p_mniejszy -> m_promien, p_tarcza); 
				  m_tymczas[m_mul_ind + 0].tv = TexY(p_otocz->WezY(m_ind) * p_mniejszy -> m_promien, p_tarcza);

				  m_tymczas[m_mul_ind + 1].pos = D3DXVECTOR3(
					  p_otocz->WezX(m_ind) * m_promien,
					  p_otocz->WezY(m_ind) * m_promien,
					  m_wycofanie);
				  m_tymczas[m_mul_ind + 1].nrm = WezAntyR(m_tymczas[m_mul_ind + 1].pos, m_promien);
				  m_tymczas[m_mul_ind + 1].tu = TexX(p_otocz->WezX(m_ind) * m_promien, p_tarcza); 
				  m_tymczas[m_mul_ind + 1].tv = TexY(p_otocz->WezY(m_ind) * m_promien, p_tarcza);
			  }

			  m_tymczas[    p_otocz->WezIlBok() * 2] = m_tymczas[0];
			  m_tymczas[1 + p_otocz->WezIlBok() * 2] = m_tymczas[1];
		  }
	};
};

class COzdoba : public CKombajnGraf
{
public:
	COzdoba() : Wzorek(), CKombajnGraf(IDX_ORNAMENT)
	{
#if defined(CPP_RAW_PRIMITIVE)
		mFileName[0] = 'O';
		mFileName[1] = 'r';
		mFileName[2] = 'n';
		mFileName[3] = 'a';
		mFileName[4] = 'm';
		mFileName[5] = 'e';
		mFileName[6] = 'n';
		mFileName[7] = 't';
		mFileName[8] = '.';
		mFileName[9] = 'x';
		mFileName[10] = 0;

		AddRenderedObject(&Wzorek);
#endif
	}

	class COzdobaInternal : public CVertexTex
	{
	public:
		COzdobaInternal() :
		  CVertexTex(D3DPT_TRIANGLEFAN, 2)
		  {
			  m_tymczas[ 0].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO, 1.0f, 0.0f);
			  m_tymczas[ 1].pos = D3DXVECTOR3( OZDOBA_ASPECT_RATIO, 1.0f, 0.0f);
			  m_tymczas[ 2].pos = D3DXVECTOR3( OZDOBA_ASPECT_RATIO,-1.0f, 0.0f);
			  m_tymczas[ 3].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO,-1.0f, 0.0f);
			  m_tymczas[ 0].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 1].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 2].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 3].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 0].tu =  0.0f;
			  m_tymczas[ 1].tu =  1.0f;
			  m_tymczas[ 2].tu =  1.0f;
			  m_tymczas[ 3].tu =  0.0f;
			  m_tymczas[ 0].tv =  0.0f;
			  m_tymczas[ 1].tv =  0.0f;
			  m_tymczas[ 2].tv =  1.0f;
			  m_tymczas[ 3].tv =  1.0f;

		  }
	} Wzorek;

};

class CSzklo : public CKombajnGraf
{
public:
	CSzklo() : Wzorek(), CKombajnGraf(IDX_GLASS)
	{
#if defined(CPP_RAW_PRIMITIVE)
		mFileName[0] = 'G';
		mFileName[1] = 'l';
		mFileName[2] = 'a';
		mFileName[3] = 's';
		mFileName[4] = 's';
		mFileName[5] = '.';
		mFileName[6] = 'x';
		mFileName[7] = 0;

		AddRenderedObject(&Wzorek);
		AddRenderedObject(&Wzorek2);
		AddRenderedObject(&Wzorek3);
#endif
	}

	class COzdobaInternal : public CVertexColor
	{
	public:
		COzdobaInternal() :
		  CVertexColor(D3DPT_TRIANGLEFAN, 2)
		  {
			  const float m_skala_x = 0.82f;
			  const float m_skala_y = 0.84f;
			  m_tymczas[ 0].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x * 0.96f, 0.99f * m_skala_y, SZKLO);
			  m_tymczas[ 1].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x * 0.2f, 0.99f * m_skala_y, SZKLO);
			  m_tymczas[ 2].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x * 0.4f, -m_skala_y*1.017f, SZKLO);
			  m_tymczas[ 3].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x * 0.97f,-m_skala_y*1.017f, SZKLO);
			  m_tymczas[ 0].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 1].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 2].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 3].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 0].color = 0x30FFFFFF;
			  m_tymczas[ 1].color = 0x00FFFFFF;
			  m_tymczas[ 2].color = 0x00FFFFFC;
			  m_tymczas[ 3].color = 0x1CFFFFFB;
		  }

	} Wzorek;
	class COzdobaInternal2 : public CVertexColor
	{
	public:
		COzdobaInternal2() :
		  CVertexColor(D3DPT_TRIANGLEFAN, 2)
		  {
			  const float m_skala_x = 0.82f;
			  const float m_skala_y = 0.84f;
			  m_tymczas[ 0].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x, 0.99f * m_skala_y, SZKLO);
			  m_tymczas[ 1].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x * 0.96f, 0.99f * m_skala_y, SZKLO);
			  m_tymczas[ 2].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x * 0.97f,-m_skala_y*1.017f, SZKLO);
			  m_tymczas[ 3].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x,-1.0f * m_skala_y*1.017f, SZKLO);
			  m_tymczas[ 0].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 1].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 2].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 3].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 0].color = 0x00FFFFFF;
			  m_tymczas[ 1].color = 0x00FFFFFF;
			  m_tymczas[ 2].color = 0x09FFFFFC;
			  m_tymczas[ 3].color = 0x09FFFFFB;
		  }

	} Wzorek2;

	class COzdobaInternal3 : public CVertexColor
	{
	public:
		COzdobaInternal3() :
		  CVertexColor(D3DPT_TRIANGLEFAN, 2)
		  {
			  const float m_skala_x = 0.82f;
			  const float m_skala_y = 0.84f;
			  m_tymczas[ 0].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x * 0.96f, 0.99f * m_skala_y, SZKLO);
			  m_tymczas[ 1].pos = D3DXVECTOR3(OZDOBA_ASPECT_RATIO * m_skala_x * 1.01f, 0.99f * m_skala_y, SZKLO);
			  m_tymczas[ 2].pos = D3DXVECTOR3(OZDOBA_ASPECT_RATIO * m_skala_x * 1.01f, m_skala_y*.520f, SZKLO);
			  m_tymczas[ 3].pos = D3DXVECTOR3(-OZDOBA_ASPECT_RATIO * m_skala_x * 0.96f,m_skala_y*.45f, SZKLO);
			  m_tymczas[ 0].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 1].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 2].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 3].nrm = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);
			  m_tymczas[ 0].color = 0x2BFFFFFF;
			  m_tymczas[ 1].color = 0x36FFFFFF;
			  m_tymczas[ 2].color = 0x03FFFFFC;
			  m_tymczas[ 3].color = 0x01FFFFFB;
		  }

	} Wzorek3;
};

class CCone : public CKombajnGraf
{
public:
	CCone() : CKombajnGraf(IDX_CONE)
	{
#if defined(CPP_RAW_PRIMITIVE)
		mFileName[0] = 'C';
		mFileName[1] = 'o';
		mFileName[2] = 'n';
		mFileName[3] = 'e';
		mFileName[4] = '.';
		mFileName[5] = 'x';
		mFileName[6] = 0;
#endif
		CGwint * tmp = new CGwint();
		AddRenderedObject(tmp);
		AddRenderedObject(new CKoniec(tmp->m_tymczas, tmp->m_il_trojk));
	}
private:

	class CGwint : public CVertexPlain
	{
	public:
		CGwint() :
		CVertexPlain(D3DPT_TRIANGLESTRIP, PRZEL(IL) << 1)
		{
			const float Maly_R = 0.392f;
			const float Duzy_R = 0.435f;
			CElipsa m_okrag(PRZEL(IL), 1.0f);

			for(DWORD m_ind = 0; m_ind < m_okrag.WezIlBok(); m_ind ++)
			{
				DWORD m_mul_ind = m_ind << 1;			
				m_tymczas[m_mul_ind    ].pos = D3DXVECTOR3(
					m_okrag.WezX(m_ind) * Maly_R,
					m_okrag.WezY(m_ind) * Maly_R,
					0.0f);

				static float m_x = Duzy_R - Maly_R;
				static float m_z = Duzy_R - Maly_R;

				m_tymczas[m_mul_ind + 1].pos = D3DXVECTOR3(
					Duzy_R * m_okrag.WezX(m_ind),
					Duzy_R * m_okrag.WezY(m_ind),
					m_z);

				D3DXVECTOR3 tmp;
				tmp = m_tymczas[m_mul_ind + 1].pos - m_tymczas[m_mul_ind].pos;
				m_tymczas[m_mul_ind + 1].nrm = * D3DXVec3Normalize(&(m_tymczas[m_mul_ind    ].nrm), 
					&(D3DXVECTOR3(- m_z * tmp[0] / m_x, - m_z * tmp[1] / m_x, - m_x)));
			}
			m_tymczas[    m_okrag.WezIlBok() * 2] = m_tymczas[0];
			m_tymczas[1 + m_okrag.WezIlBok() * 2] = m_tymczas[1];
			for(DWORD m_ind = 0; m_ind < m_il_punkt; m_ind ++)
			{
				m_tymczas[m_ind].pos[0] = m_tymczas[m_ind].pos[0] -.000f;
				m_tymczas[m_ind].pos[1] = m_tymczas[m_ind].pos[1] -.211f;
				//m_tymczas[m_ind].color = 0x00000000;
			}
		}
	};

	class CKoniec : public CVertexPlain
	{
	public:
		CKoniec(
			const VERTEXPLAIN * p_tymczas,
			const DWORD p_il_trojk) :
		CVertexPlain(D3DPT_TRIANGLEFAN, ((p_il_trojk >> 1) - 1))
		{
			DWORD m_ind;

			for(m_ind = 0; m_ind <= m_il_trojk; m_ind ++)
			{
				m_tymczas[m_ind] = p_tymczas[m_ind << 1];
			}
			m_tymczas[m_ind] = m_tymczas[0];
		}
	};
};