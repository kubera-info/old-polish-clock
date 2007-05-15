#ifndef _KUBERA_MATH_
#define _KUBERA_MATH_

#define M_PI_MUL_2 ((float) (M_PI * 2))
#define M_PI_DIV_2 ((float) M_PI_2)
#define M_PI_DIV_4 ((float) (M_PI / 4))

// #######################################
// Nazwa: class CElipsa
// Opis: Klasa obliczaj¹ca fragment elipsy
//       zawieraj¹cy ¿¹dan¹ iloœæ punktów.
// #######################################
class CElipsa
{
public:
	CElipsa(const DWORD p_il_bok, const float p_promien_x = NULL, float p_promien_y = 0.0f, const float p_kat = M_PI_MUL_2, const float p_start = 0.0f) :
	  m_promien_x(p_promien_x),
		  m_promien_y(NULL == p_promien_y ? p_promien_x : p_promien_y)
	  {
		  m_il_bok = p_il_bok;
		  m_punkty = new D3DXVECTOR2 [WezIlPunkt()];
		  if(m_punkty)
		  {
			  float m_faza = p_start;
			  float m_krok = p_kat / m_il_bok;
			  DWORD m_ind;
			  for(m_ind = 0; m_ind < m_il_bok; m_ind ++)
			  {
				  DodajPunkt(m_faza, m_ind);
				  m_faza = m_faza + m_krok;
			  }
			  if(M_PI_MUL_2 == p_kat)
				  m_punkty[m_il_bok] = m_punkty[0];
			  else
				  DodajPunkt(p_start + p_kat, m_il_bok);
		  }
		  else
			  m_il_bok = 0;
	  }
	  float WezX(const DWORD p_ind)
	  {
		  if(m_punkty)
				return (p_ind > m_il_bok || p_ind < 0) ? -1.0f : m_punkty[p_ind][0];
		  else
			  return 1.0f;
			 
	  }
	  float WezY(const DWORD p_ind)
	  {
		  if(m_punkty)
			  return (p_ind > m_il_bok || p_ind < 0) ? -1.0f : m_punkty[p_ind][1];
		  else
			  return 1.0f;
	  }
	  DWORD WezIlPunkt()
	  {
		  return m_il_bok + 1;
	  }
	  DWORD WezIlBok()
	  {
		  return m_il_bok;
	  }
	  ~CElipsa()
	  {
		  if(m_punkty)
			  SAFE_DELETE_ARRAY( m_punkty );
	  }
	  const float m_promien_x;
	  const float m_promien_y;

private:
	D3DXVECTOR2 * m_punkty; // Tablica wszystkich wierzcho³ków.
	DWORD m_il_bok; // Iloœæ wierzcho³ków = iloœæ punktów ³¹cz¹cych - 1.
	// Podprogram pomocniczy obliczaj¹cy wspó³rzêdne jednego wierzcho³ka.
	void DodajPunkt(const float p_faza, const DWORD p_ind)
	{
		m_punkty[p_ind] = D3DXVECTOR2(cosf(p_faza) * m_promien_x, -sinf(p_faza) * m_promien_y);
	}
};

#endif