#ifndef _TARCZA_H_
#define _TARCZA_H_

// #############################################
// Nazwa: class CTarcza
// Opis: Klasa przechowuj¹ca i rysuj¹ca
//       tarczê zegarow¹
// #############################################
class CTarcza : public CKombajnGraf
{
public:
	CTarcza(const D3DXVECTOR3 & p_rozm, const D3DXVECTOR3 & p_srodek) :
		CKombajnGraf(p_srodek),
		m_wycinek(PRZEL(IL), 1.0f, 1.0f)
	{
		m_rozm = p_rozm;

		CElipsa Przekroj(m_wycinek.m_il_bok, m_rozm[0], m_rozm[2], M_PI_DIV_2);
		// Kolejne fragmenty zakrzywienia
		WORD m_ind = m_wycinek.m_il_bok - 1;
		m_obiekty.Add(new CKolo
			(
				&m_wycinek,
				WezR(fabsf(Przekroj.WezX(m_ind))),
				WezZ(fabsf(Przekroj.WezY(m_ind))),
				m_rozm));
		while(m_ind)
		{
			m_obiekty.Add(new CDingo
				(	&m_wycinek,
					WezR(fabsf(Przekroj.WezX(m_ind))),
					WezZ(fabsf(Przekroj.WezY(m_ind))), 
					(CKombajn *) m_obiekty.GetAt(m_obiekty.GetCount() - 1),
					m_rozm));
			m_ind --;
		}
		AddPrimitive( );
	}
	virtual HRESULT DajDefinicje( )
	{
		return S_OK;
	}
	float WezZ(const float p_z)
	{
		return p_z - m_rozm[3];
	}
	float WezR(const float p_x)
	{
		return powf((p_x / m_rozm[0]), WSPOLCZ_PRZEKROJ);
	}
	HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice );
	//~CTarcza();
private:
		class CKombajn : public CVertexTex
		{
		public:
			CKombajn(
					CElipsa * p_otocz, 
					float p_promien, 
					float p_wycofanie, 
					const D3DXVECTOR3 & p_tarcza,
					const D3DPRIMITIVETYPE p_rodzaj_opt) :
			CVertexTex(D3DXVECTOR3(0.0f, 0.0f, 0.0f), p_rodzaj_opt) 
			{
				m_otocz = p_otocz;
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
			D3DXVECTOR3 WezAntyR(D3DXVECTOR3 & p_poz, float p_r) // Funkcja oblicza odwrotn¹ potêgê ni¿ WezR()
			{
				D3DXVECTOR3 tmp;
				float m_r = powf(p_r/* / m_rozm[0]*/, 1.0f / (WSPOLCZ_PRZEKROJ * WSPOLCZ_PRZEKROJ));
				return * D3DXVec3Normalize(&tmp,
					&D3DXVECTOR3(
					p_poz[0] * m_r,
					p_poz[1] * m_r,
					- p_poz[2] / (WYCOFANIE * WYCOFANIE)));
			}

			CElipsa * m_otocz;
			float m_wycofanie;
			float m_promien;
		};

		class CKolo : public CKombajn
		{
		public:
			CKolo(CElipsa * p_otocz, float p_promien, float p_wycofanie, const D3DXVECTOR3 & p_tarcza) :
				CKombajn(p_otocz, p_promien, p_wycofanie, p_tarcza, D3DPT_TRIANGLEFAN)
			{
				Create(m_otocz->WezIlPunkt() + 1);
				m_tymczas[0].pos = D3DXVECTOR3(0.0f, 0.0f, m_wycofanie);
				m_tymczas[0].nrm[0] =  0.0f;
				m_tymczas[0].nrm[1] =  0.0f;
				m_tymczas[0].nrm[2] = -1.0f;
				m_tymczas[0].tu = 0.5f;
				m_tymczas[0].tv = 0.5f;
				WORD m_ind = 0;
				while( m_ind < m_il_trojk )
				{
					register float m_x = m_otocz->m_punkty[m_ind][0] * m_promien;
					register float m_y = m_otocz->m_punkty[m_ind][1] * m_promien;
					m_ind ++;
					m_tymczas[m_ind].pos = D3DXVECTOR3(
						m_x,
						m_y,
						m_wycofanie);
					m_tymczas[m_ind].nrm = WezAntyR(m_tymczas[m_ind].pos, m_otocz->m_promien_x);
					m_tymczas[m_ind].tu = TexX(m_x, p_tarcza);
					m_tymczas[m_ind].tv = TexY(m_y, p_tarcza);					
				}
				m_tymczas[m_il_punkt - 1] = m_tymczas[1];
				//UtworzZrodlo(pd3dDevice);
			}
		};
		class CDingo : public CKombajn
		{
		public:
			CDingo(CElipsa * p_otocz, float p_promien, float p_wycofanie, CKombajn * p_mniejszy, const D3DXVECTOR3 & p_tarcza) :
				CKombajn(p_otocz, p_promien, p_wycofanie, p_tarcza, D3DPT_TRIANGLESTRIP)
			{
				Create(m_otocz->m_il_bok << 1);

				for(WORD m_ind = 0; m_ind < m_otocz->m_il_bok; m_ind ++)
				{
					WORD m_mul_ind = m_ind << 1;			
					m_tymczas[m_mul_ind + 0].pos = D3DXVECTOR3(
						m_otocz->WezX(m_ind) * p_mniejszy -> m_promien,
						m_otocz->WezY(m_ind) * p_mniejszy -> m_promien,
						p_mniejszy -> m_wycofanie);
					m_tymczas[m_mul_ind + 0].nrm = WezAntyR(m_tymczas[m_mul_ind].pos, p_mniejszy -> m_promien);
					m_tymczas[m_mul_ind + 0].tu = TexX(m_otocz->WezX(m_ind) * p_mniejszy -> m_promien, p_tarcza); 
					m_tymczas[m_mul_ind + 0].tv = TexY(m_otocz->WezY(m_ind) * p_mniejszy -> m_promien, p_tarcza);

					m_tymczas[m_mul_ind + 1].pos = D3DXVECTOR3(
						m_otocz->WezX(m_ind) * m_promien,
						m_otocz->WezY(m_ind) * m_promien,
						m_wycofanie);
					m_tymczas[m_mul_ind + 1].nrm = WezAntyR(m_tymczas[m_mul_ind + 1].pos, m_promien);
					m_tymczas[m_mul_ind + 1].tu = TexX(m_otocz->WezX(m_ind) * m_promien, p_tarcza); 
					m_tymczas[m_mul_ind + 1].tv = TexY(m_otocz->WezY(m_ind) * m_promien, p_tarcza);
				}

				m_tymczas[    m_otocz->m_il_bok * 2] = m_tymczas[0];
				m_tymczas[1 + m_otocz->m_il_bok * 2] = m_tymczas[1];
			}
		};

	D3DXVECTOR3 m_rozm;
	CElipsa m_wycinek;
};

#endif