#ifndef _MUTERKA_H_
#define _MUTERKA_H_

// #############################################
// Nazwa: proporcje_mutry
// Opis: Ró¿ne promienie dla obiektu pierwszoplanowego (CMontaz)
//       Muterka, gwiazdka i in.
// #############################################
typedef 
	struct 
	{
		float maly_r;
		float wiekszy_r;
		float duzy_r;
		float gwint_r; // Najwiêkszy promieñ œruby
		float koniec_r; // Promieñ zciêcia sto¿ka
	}
	proporcje_mutry;

// #############################################
// Nazwa: class CMontaz
// Opis: Zbiór ró¿nych elementów graficznych
//       Muterka, gwiazdka i in.
// #############################################
class CMontaz : public CKombajnGraf
{
public:
	CMontaz(
		const proporcje_mutry & p_rozm,
		const D3DXVECTOR3 p_srodek) :
		CKombajnGraf(p_srodek)
		
	{
		m_obiekty.Add(new CStozek(p_srodek, p_rozm));
		m_obiekty.Add(new CNakretka(p_srodek, p_rozm));
		CGwint * tmp = new CGwint(p_srodek, p_rozm);
		m_obiekty.Add(tmp);
		m_obiekty.Add(new CKoniec(p_srodek, p_rozm, tmp->m_tymczas, tmp->m_il_trojk));
	}
	HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice )
	{
		CKombajnGraf::Render( pd3dDevice );
		pd3dDevice->SetTexture(0, NULL);

		return S_OK;
	}
private:
		class CStozek : public CVertexTex
		{
		public:
			CStozek(
					const D3DXVECTOR3 & p_srodek, 
					const proporcje_mutry & p_rozm) :
				CVertexTex(p_srodek, D3DPT_TRIANGLESTRIP)
			{
				CElipsa m_okrag(36 << 1, 1.0f);
				m_rozm = p_rozm;
			
				m_il_punkt = (m_il_trojk = m_okrag.m_il_bok << 1) + 2;
				m_tymczas = new VERTEXTEX [m_il_punkt];

				for(WORD m_ind = 0; m_ind < m_okrag.m_il_bok; m_ind ++)
				{
					WORD m_mul_ind = m_ind << 1;			
					m_tymczas[m_mul_ind    ].pos = D3DXVECTOR3(
						m_rozm.maly_r * m_okrag.WezX(m_ind),
						m_rozm.maly_r * m_okrag.WezY(m_ind),
						m_srodek[2]);
					m_tymczas[m_mul_ind    ].tu = (1.0f + 0.2f * m_okrag.WezX(m_ind)) / 2; 
					m_tymczas[m_mul_ind    ].tv = (1.0f - 0.2f * m_okrag.WezY(m_ind)) / 2;

					float m_r = (m_ind & 1 ? m_rozm.duzy_r : m_rozm.wiekszy_r);
					float m_x = m_r - m_rozm.maly_r;
					float m_z = m_r * WSP_KATA;

					m_tymczas[m_mul_ind + 1].pos = D3DXVECTOR3(
						m_r * m_okrag.WezX(m_ind),
						m_r * m_okrag.WezY(m_ind),
						- m_z + m_srodek[2]);
					m_tymczas[m_mul_ind + 1].tu = (1.0f + m_okrag.WezX(m_ind)) / 2; 
					m_tymczas[m_mul_ind + 1].tv = (1.0f - m_okrag.WezY(m_ind)) / 2;
					
					D3DXVECTOR3 tmp;
					tmp = m_tymczas[m_mul_ind + 1].pos - m_tymczas[m_mul_ind].pos;
					m_tymczas[m_mul_ind + 1].nrm = * D3DXVec3Normalize(&(m_tymczas[m_mul_ind    ].nrm),
						&(D3DXVECTOR3(m_z * tmp[0] / m_x, m_z * tmp[1] / m_x, - m_x)));
				}
				m_tymczas[    m_okrag.m_il_bok * 2] = m_tymczas[0];
				m_tymczas[1 + m_okrag.m_il_bok * 2] = m_tymczas[1];
			}
			HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice )
			{
				ASERT( pd3dDevice->SetMaterial(&mtrl_mosiadz) );
				ASERT( pd3dDevice->SetTexture( 0, g_Texture2 ) );
				return CVertexTex::Render( pd3dDevice );				
			}

		protected:
			proporcje_mutry m_rozm;
		};

		class CNakretka : public CVertexTex
		{
		public:
			CNakretka(
					const D3DXVECTOR3 & p_srodek, 
					const proporcje_mutry & p_rozm) :
				CVertexTex(p_srodek, D3DPT_TRIANGLEFAN)
			{
				CElipsa m_okrag(6, 1.0f, NULL, M_PI_MUL_2, -0.73f);
				m_rozm = p_rozm;
			
				m_il_punkt = (m_il_trojk = m_okrag.m_il_bok - 1) + 2;
				m_tymczas = new VERTEXTEX [m_il_punkt];		

				for(WORD m_ind = 0; m_ind < m_okrag.m_il_bok; m_ind ++)
				{
					m_tymczas[m_ind].pos = D3DXVECTOR3(
						m_okrag.WezX(m_ind) * p_rozm.maly_r * 1.0f,
						m_okrag.WezY(m_ind) * p_rozm.maly_r * 1.0f,
						m_srodek[2] + .03f);
					m_tymczas[m_ind].tu = (1.0f + m_okrag.WezX(m_ind)) / 2; 
					m_tymczas[m_ind].tv = (1.0f + m_okrag.WezY(m_ind)) / 2;
					m_tymczas[m_ind].nrm = D3DXVECTOR3(0.0f, 0.0f, -1.0f); 
				}
				m_tymczas[m_okrag.m_il_bok] = m_tymczas[0];
			}
			HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice )
			{
				ASERT( pd3dDevice->SetMaterial(&mtrl_mosiadz) );
				ASERT( pd3dDevice->SetTexture( 0, g_Texture2 ) );
				return CVertexTex::Render( pd3dDevice );
			}
		protected:
			proporcje_mutry m_rozm;
		};

		class CGwint : public CVertexTex
		{
		public:
			CGwint(
					const D3DXVECTOR3 & p_srodek, 
					const proporcje_mutry & p_rozm) :
				CVertexTex(p_srodek, D3DPT_TRIANGLESTRIP)
			{
				CElipsa m_okrag(PRZEL(48), 1.0f);
				m_il_punkt = (m_il_trojk = m_okrag.m_il_bok << 1) + 2;		
				m_tymczas = new VERTEXTEX [m_il_punkt];

				for(WORD m_ind = 0; m_ind < m_okrag.m_il_bok; m_ind ++)
				{
					WORD m_mul_ind = m_ind << 1;			
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
						- m_z + m_srodek[2] + 0.07f);
					m_tymczas[m_mul_ind + 1].tu = (1.0f + m_okrag.WezX(m_ind)) / 2; 
					m_tymczas[m_mul_ind + 1].tv = (1.0f - m_okrag.WezY(m_ind)) / 2;
					
					D3DXVECTOR3 tmp;
					tmp = m_tymczas[m_mul_ind + 1].pos - m_tymczas[m_mul_ind].pos;
					m_tymczas[m_mul_ind + 1].nrm = * D3DXVec3Normalize(&(m_tymczas[m_mul_ind    ].nrm), 
						&(D3DXVECTOR3(m_z * tmp[0] / m_x, m_z * tmp[1] / m_x, - m_x)));
				}
				m_tymczas[    m_okrag.m_il_bok * 2] = m_tymczas[0];
				m_tymczas[1 + m_okrag.m_il_bok * 2] = m_tymczas[1];
			}
			HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice )
			{
				ASERT( pd3dDevice->SetMaterial(&mtrl_stal) );
				ASERT( pd3dDevice->SetTexture( 0, g_Texture3 ));
				return CVertexTex::Render( pd3dDevice );
			}
		};

		class CKoniec : public CVertexTex
		{
		public:
			CKoniec(
					const D3DXVECTOR3 & p_srodek, 
					const proporcje_mutry & p_rozm,
					const VERTEXTEX * p_tymczas,
					const WORD p_il_trojk) :
				CVertexTex(p_srodek, D3DPT_TRIANGLEFAN)
			{
				m_il_punkt = (m_il_trojk = ((p_il_trojk >> 1) - 1)) + 2;
				m_tymczas = new VERTEXTEX [m_il_punkt];
				for(WORD m_ind = 0; m_ind <= m_il_trojk; m_ind ++)
				{
					m_tymczas[m_ind].pos = p_tymczas[m_ind << 1].pos;
					m_tymczas[m_ind].nrm = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
					m_tymczas[m_ind].tu = (1.0f + m_tymczas[m_ind].pos[0] / (p_rozm.koniec_r * 2)) / 2;
					m_tymczas[m_ind].tv = (1.0f + m_tymczas[m_ind].pos[1] / (p_rozm.koniec_r * 2)) / 2;
				}
				m_tymczas[m_ind] = m_tymczas[0];
			}
			HRESULT Render( LPDIRECT3DDEVICE8 pd3dDevice )
			{
				ASERT( pd3dDevice->SetMaterial(&mtrl_stal) );
				ASERT( pd3dDevice->SetTexture( 0, g_Texture3 ));
				return CVertexTex::Render( pd3dDevice );
			}
		};
};

#endif