// #####################################################
// ogolne.h - pakiet ogólny, implenetacje klas bazowych.
// #####################################################

#ifndef _OGOLNE_H_
#define _OGOLNE_H_

#include "stdafx.h"

// Struktury Flexible Vertex Format u¿ywane w zegarze.
struct VERTEXTEX
{
	D3DXVECTOR3 pos;	// Pozycja
	D3DXVECTOR3 nrm;	// Wektor normalny
	FLOAT       tu, tv;	// Pozycja tekstury

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;
};
struct VERTEXPLAIN
{
	D3DXVECTOR3 pos;	// Pozycja
	D3DXVECTOR3 nrm;	// Wektor normalny

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
};

// Struktura do przechowywania figury wyœwietlaj¹cej cieñ.
struct SHADOWVERTEX
{
	D3DXVECTOR3 pos;	// Pozycja
	FLOAT		rhw;	// Untrasformed rhw
	D3DCOLOR    color;	// kolor

	static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
};

// Struktura do rysowania figur niewidocznych,
// ale zapisuj¹cych do bufora g³êbi (duszki).
struct VECTOR3VERTEX
{
	D3DXVECTOR3 pos;	// Pozycja

	static const DWORD FVF = D3DFVF_XYZ;
};

struct VERTEXCOLOR
{
	D3DXVECTOR3 pos;	// Pozycja
	D3DXVECTOR3 nrm;	// Wektor normalny
	D3DCOLOR    color;	// Kolor ARGB

	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
};



// Funkcja ³aduj¹ca zasób typu RT_RCDATA o podanym ID.
LPCVOID ZaladujZasob(CONST INT pResID, DWORD & pLen);




// ###########################################
// Nazwa: class CObiektGraf
// Opis: Bazowa klasa grafiki Direct3D zegara.
// ###########################################
class CObiektGraf
{
public:
	CObiektGraf() {};

	// Klasa dziedzicz¹ca musi implementowaæ rysowanie.
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice) = 0; 

	// Opcjonalna inicjalizacja przed rysowaniem.
	virtual void OnFrameMove( )									{ }

	// Jednorazowe utworzenie zasobów urz¹dzenie Direct3D (np. tekstury POOL_MANAGED).
	virtual HRESULT OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )								{ return S_OK; }
	
	//virtual HRESULT OnResetDevice( LPDIRECT3DDEVICE9 pd3dDevice )								{ return S_OK; }
	//virtual void OnLostDevice( )																{ }

	// Jednorazowe zwolnienie zasobów urz¹dzenia (usuniêcie tekstur, wy³¹czenie dŸwiêku itd.).
	virtual void OnDestroyDevice( )																{ }

	// Implementacja rzucania cienia technik¹ Shadow Volume.
	virtual void OnCastShadow( LPDIRECT3DDEVICE9 pd3dDevice )									{ };

	// Typ Flexible Vertex Format lub zero przy braku implentacji.
	virtual DWORD GetFVF()																		{ return NULL; }
};

typedef class CGrowableArray<CObiektGraf *> CObiektGrafArray;

// #########################################################################
// Nazwa: class CMacierz
// Opis: Ustawienie przestrzeni modelu, który zostanie za chwilê narysowany.
// #########################################################################
class CMacierz : public CObiektGraf
{
public:
	CMacierz(D3DXMATRIXA16 * p_matrix) : m_matrix(p_matrix)
	{
	}
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		SetTransform( pd3dDevice );
	}
	virtual void OnCastShadow( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		SetTransform( pd3dDevice );
	}
private:
	// Ustawienie przestrzeni modelu, który bêdzie nastêpnie rysowany
	void SetTransform(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		if(m_matrix)
			pd3dDevice->SetTransform(D3DTS_WORLD, m_matrix);
		else
			DXTRACE_ERR( TEXT("NULL matrix pointer"), E_FAIL);
	}
	const D3DXMATRIXA16 * m_matrix;
};

// ################################################################
// Nazwa: class CMacierz
// Opis: Ustawienie mataria³u dla modeli nie posiadaj¹cych tekstur.
// ################################################################
class CMaterial : public CObiektGraf
{
public:
	CMaterial(
		const float p_red, 
		const float p_green, 
		const float p_blue,
		const float p_alpha = 1.0f)
	{
		// Ustawienie koloru materia³u.
		ZeroMemory( &m_mtrl, sizeof(m_mtrl));
		m_mtrl.Diffuse.r = m_mtrl.Ambient.r = p_red;
		m_mtrl.Diffuse.g = m_mtrl.Ambient.g = p_green;
		m_mtrl.Diffuse.b = m_mtrl.Ambient.b = p_blue;
		m_mtrl.Diffuse.a = m_mtrl.Ambient.a = p_alpha;

		// Klasa CMaterial ustawia jako atrybut tekstury NULL.
		// Klasa dziedzicz¹ca mo¿e wykonaæ inn¹ operacjê.
		m_tekstura = NULL;
	}
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		pd3dDevice->SetMaterial(&m_mtrl);
		pd3dDevice->SetTexture( 0, m_tekstura );
	}

private:
	D3DMATERIAL9 m_mtrl;
protected:
	LPDIRECT3DTEXTURE9 m_tekstura;
};

// #############################################
// Nazwa: class CMacierz
// Opis: Ustawienie matarie³u i tekstury modelu.
// #############################################
class CFaktura : public CMaterial
{
public:
	CFaktura(
		const float p_red, 
		const float p_green, 
		const float p_blue,
		const INT p_IDB,
		const D3DFORMAT p_format = D3DFMT_UNKNOWN/*,
		const bool p_scale = false */) : m_IDB(p_IDB), /* m_scale(p_scale), */ CMaterial(p_red, p_green, p_blue)
	{
		m_format = p_format;
	}
	virtual HRESULT OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
	virtual void OnDestroyDevice( );

private:
	D3DFORMAT m_format;
	const INT m_IDB;
//	const bool m_scale;
};

// ##############################################
// Nazwa: class CPrymityw
// Opis: Klasa przechowuj¹ca i rysuj¹ca
//       tzw. "Primitive" - zbiór trójk¹tów.
//       Klasa nie jest u¿ywana w wersji finalnej
//       Jej zastosowaniem jest stworzenie
//       fragmentu modelu.
// ##############################################
class CPrymityw : public CObiektGraf
{
public:
	CPrymityw(
		const D3DPRIMITIVETYPE p_rodzaj_opt,
		const DWORD p_il_trojk)
	{
		m_rodzaj_opt = p_rodzaj_opt;
		m_vertices = NULL;
		m_VB = NULL;
		m_il_trojk = p_il_trojk;

		// Ten fragment s³u¿y wyliczeniu iloœci wierzcho³ków dla danego typu
		// figury podstawowej.
		switch(m_rodzaj_opt)
		{
		case D3DPT_TRIANGLESTRIP : 
			m_il_punkt = (m_il_trojk << 1) + 2;
			break;
		case D3DPT_TRIANGLEFAN : 
			m_il_punkt = m_il_trojk + 2;
			break;
		case D3DPT_TRIANGLELIST : 
			m_il_punkt = m_il_trojk * 3;
			break;
		}
	}
	virtual VOID * WezTymczas()																	{ return NULL; }
	virtual D3DXVECTOR3 * WezPunkt(const DWORD p_indeks) = 0;
	D3DPRIMITIVETYPE PodajTyp()
	{
		return m_rodzaj_opt;
	}
	// #############################################
	// Nazwa: metoda CPrymityw::DajIndeksy
	// Opis: Zwraca 3 wartoœci DWORD
	//       bêd¹ce indeskami wierczho³ków
	//       dla danego indeksu trójk¹ta.
	// #############################################
	void DajIndeksy(const DWORD p_trojkat, DWORD & a, DWORD & b, DWORD & c)
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
	virtual LONG SizeOf() = 0;
	
	// Typowa funkcja rysuj¹ca zadany "prymityw".
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		HRESULT hr;
		if(NULL != m_VB)
		{
			hr = pd3dDevice->SetStreamSource( 0, m_VB, NULL, SizeOf() );
			hr = pd3dDevice->SetFVF( GetFVF() );
			hr = pd3dDevice->DrawPrimitive(
				PodajTyp(), 
				0, 
				m_il_trojk);
		}
	}
	virtual HRESULT OnCreateDevice(LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc)
	{
		HRESULT hr;

		InverseZAxis( ); // Figury utworzone t¹ metod¹ by³y wyliczone dla innych macierzy...

		OnDestroyDevice();

		V_RETURN(pd3dDevice->CreateVertexBuffer( m_il_punkt * SizeOf(),
			0 , GetFVF() ,
			D3DPOOL_MANAGED, &m_VB, NULL ) );
		V_RETURN((m_VB->Lock( 0, 0, &m_vertices, 0 ) ) )
			memcpy(m_vertices, WezTymczas(), SizeOf() * m_il_punkt);
		return m_VB->Unlock();
	}
	virtual void OnDestroyDevice()
	{
		SAFE_RELEASE(m_VB);
	}
	virtual void InverseZAxis( ) = 0;

	VOID * m_vertices;
	D3DPRIMITIVETYPE m_rodzaj_opt;
	DWORD m_il_trojk, m_il_punkt;
	LPDIRECT3DVERTEXBUFFER9 m_VB;
};

class CKombajnGraf;

// ###############################################################
// Nazwa: template CSavingMesh
// Opis: Wzorzec, który z tablicy figur podstawowych tworzy mesh.
//       Argumentem wzorca jest zaimplementowany typ FVF.
// ###############################################################
template 
<typename FormatFigury> class CSavingMesh : public CObiektGraf
{
public:
	CSavingMesh( 
		LPDIRECT3DDEVICE9 pd3dDevice, 
		CKombajnGraf * p_zrodlo,
		CHAR * p_nazwa,
		const INT p_ID) : m_ID(p_ID)
	{
		m_mesh = NULL;
		m_zrodlo = p_zrodlo;
		m_nazwa = p_nazwa;
	}
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		//if(m_mesh)
		//m_mesh -> DrawSubset( 0 );
	}
	virtual DWORD GetFVF()
	{
		return FormatFigury::FVF;
	}
	virtual HRESULT OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
	{ 
		HRESULT hr; 

		// OnCreateDevice utworzy plik, tylko jeœli go jeszcze nie wykreowano.
		WIN32_FIND_DATAA FileData; 
		HANDLE hSearch; 
		hSearch = FindFirstFileA(m_nazwa, &FileData); 
		if (hSearch != INVALID_HANDLE_VALUE) 
		{
			FindClose(hSearch);
			return S_OK;
		}
		else
			FindClose(hSearch);
			
		DWORD m_NumFaces = 0;
		DWORD m_NumVertices = 0;
		VOID * m_Faces = NULL;
		VOID * m_Vertices = NULL;
		DWORD m_il_trojk = 0;
		if(NULL == m_zrodlo)
		{
			return DXTRACE_ERR( TEXT("NULL Collection"), E_FAIL );
		}
#define PPRYMITYW(i) ((CPrymityw *) (m_zrodlo->GetAt(i)))
		int cnt = m_zrodlo->GetSize();
		for (int i = 0; i < m_zrodlo->GetSize(); i++)
			if(GetFVF() == m_zrodlo->GetAt(i)->GetFVF())
				m_il_trojk += PPRYMITYW(i)->m_il_trojk;

		DWORD *        bufor_ind  = new DWORD [m_il_trojk*3];
		FormatFigury * bufor_vert = new FormatFigury [m_il_trojk*3];

		if(m_il_trojk)
		{
			for (int i = 0; i < m_zrodlo->GetSize(); i++)
			{
				if(GetFVF() != m_zrodlo->GetAt(i)->GetFVF())
					continue;
				DWORD tab[3]; 
				for(DWORD j = 0; j < PPRYMITYW(i)->m_il_trojk; j++)
				{
					PPRYMITYW(i)->DajIndeksy(j, tab[0], tab[1], tab[2]);
					for(DWORD l = 0; l < 3; l ++)
					{
						bool bFound = false;
						DWORD k = 0;
						while(k < m_NumVertices)
						{
							if( ( (FormatFigury *) (PPRYMITYW(i)->m_vertices))[tab[l]].pos == bufor_vert[k].pos)
							{
								bFound = true;
								break;
							}
							else
								k ++;
						}
						if(!bFound)
							bufor_vert[m_NumVertices++] = ( (FormatFigury *) (PPRYMITYW(i)->m_vertices))[tab[l]];

						bufor_ind[m_NumFaces ++] = k;
					}
				}
			}
		}

		V_RETURN(D3DXCreateMeshFVF(
			m_NumFaces,
			m_NumVertices,
			D3DXMESH_32BIT | D3DXMESH_IB_MANAGED | D3DXMESH_VB_MANAGED,
			FormatFigury::FVF,
			pd3dDevice,
			&m_mesh));
		V_RETURN(m_mesh->LockVertexBuffer(NULL, &m_Vertices));
		V_RETURN(m_mesh->LockIndexBuffer(NULL, &m_Faces));
		CopyMemory(m_Vertices, bufor_vert, sizeof(FormatFigury) * m_NumVertices);
		CopyMemory(m_Faces, bufor_ind, sizeof(DWORD) * m_NumFaces);
		V_RETURN(m_mesh->UnlockIndexBuffer());
		V_RETURN(m_mesh->UnlockVertexBuffer());
		V_RETURN(D3DXSaveMeshToXA(
			m_nazwa,
			m_mesh,
			NULL,
			NULL,
			NULL,
			0,
			D3DXF_FILEFORMAT_TEXT | D3DXF_FILEFORMAT_COMPRESSED));

		SAFE_DELETE_ARRAY(bufor_ind);
		SAFE_DELETE_ARRAY(bufor_vert);

		return S_OK;
	}
	virtual void OnDestroyDevice()
	{
		SAFE_RELEASE(m_mesh);
	}

private:
	LPD3DXMESH m_mesh;
	CKombajnGraf * m_zrodlo;
	CHAR * m_nazwa;
	const INT m_ID;
};

// ###############################################
// Nazwa: klasy dziedzicz¹ce CSavingMesh
// Opis: Implementacje klasy zapisuj¹cej mesh
//       dla danego FVF z tablicy prymitywów.
// ###############################################
typedef CSavingMesh <VERTEXPLAIN> CSavingMeshPlain;
typedef CSavingMesh <VERTEXTEX> CSavingMeshTex;
typedef CSavingMesh <VECTOR3VERTEX> CSavingMeshVec;
typedef CSavingMesh <VERTEXCOLOR> CSavingMeshColor;

// ######################################################
// Nazwa: template CNazwaKlasy
// Opis: Dowolny rodzaj "prymitywu".
//       S¹ to ró¿ne klasy udostêpniaj¹ce identyczne API.
// ######################################################
template 
<typename FormatFigury> class CNazwaKlasy : public CPrymityw
{
public:
	CNazwaKlasy(
		const D3DPRIMITIVETYPE p_rodzaj_opt,
		const DWORD p_il_trojk) :
	CPrymityw(p_rodzaj_opt, p_il_trojk)
	{
		m_tymczas = new FormatFigury [m_il_punkt];
		if(NULL == m_tymczas)
			m_il_punkt = 0;
	}

	DWORD GetFVF()
	{
		return FormatFigury::FVF;
	}
	LONG SizeOf()
	{
		return sizeof(FormatFigury);
	}
	D3DXVECTOR3 * WezPunkt(const DWORD p_indeks)
	{
		return &(m_tymczas[p_indeks].pos);
	}
	VOID * WezTymczas()
	{
		return (VOID *) m_tymczas;
	}
	virtual void InverseZAxis( )
	{
		for(DWORD m_ind = 0; m_ind < m_il_punkt; m_ind ++)
		{
			m_tymczas[m_ind].pos[0] = -m_tymczas[m_ind].pos[0];
			m_tymczas[m_ind].nrm[0] = -m_tymczas[m_ind].nrm[0];
		}
	}

	FormatFigury * m_tymczas;
};

// #############################################
// Nazwa: implementacje CNazwaKlasy
// Opis: Wybrany rodzaj "prymitywu".
// #############################################
typedef CNazwaKlasy <VERTEXPLAIN> CVertexPlain;
typedef CNazwaKlasy <VERTEXTEX> CVertexTex;
typedef CNazwaKlasy <VECTOR3VERTEX> CVertexVec3;
typedef CNazwaKlasy <VERTEXCOLOR> CVertexColor;
typedef CNazwaKlasy <SHADOWVERTEX> CShadowVertex;

// #####################################################
// Nazwa: class CMesh
// Opis: Bazowa klasa przechowywania i rysowania modelu.
// #####################################################
class CMesh : public CObiektGraf
{
public:
	CMesh(const INT p_ID/*, const DWORD p_flags = 0*/) : m_ID(p_ID)//, m_flags(p_flags)
	{
		m_model = NULL;
	}
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		if(m_model)
			m_model -> DrawSubset( 0 );
	}
	virtual HRESULT OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
	virtual void OnDestroyDevice()
	{
		SAFE_RELEASE(m_model);
	}

protected:
	LPD3DXMESH    m_model;
	const INT     m_ID;
};

// ##########################################################
// Nazwa: class CShadowMesh
// Opis: Klasa przechowywania i rysowania modelu.
//       Klasa implementuje dodatkowo technikê Shadow Volume.
// ##########################################################
class CShadowMesh : public CMesh
{
public:
	CShadowMesh(const INT p_ID) : CMesh(p_ID)
	{
		m_shadow = NULL;
	}
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		if(MyInvalidatedShadow)
			BuildFromMesh( pd3dDevice );
		CMesh::OnFrameRender ( pd3dDevice );
	};
	virtual void OnCastShadow( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		if(m_shadow)
			m_shadow -> DrawSubset( 0 );
	}
	virtual void OnDestroyDevice()
	{
		SAFE_RELEASE(m_shadow);
		CMesh::OnDestroyDevice();
	}

protected:

	bool MyInvalidatedShadow;

private:
	VOID AddEdge( DWORD* pEdges, DWORD& dwNumEdges, const DWORD v0, const DWORD v1 )
	{
		// Remove interior edges (which appear in the list twice)
		for( DWORD i=0; i < dwNumEdges; i++ )
		{
			if( ( pEdges[2*i+0] == v0 && pEdges[2*i+1] == v1 ) ||
				( pEdges[2*i+0] == v1 && pEdges[2*i+1] == v0 ) )
			{
				if( dwNumEdges > 1 )
				{
					pEdges[2*i+0] = pEdges[2*(dwNumEdges-1)+0];
					pEdges[2*i+1] = pEdges[2*(dwNumEdges-1)+1];
				}
				dwNumEdges--;
				return;
			}
		}

		pEdges[2*dwNumEdges+0] = v0;
		pEdges[2*dwNumEdges+1] = v1;
		dwNumEdges++;
	}

	D3DXVECTOR3 CastHelper(const LPVOID pVertices, const INT pPos)
	{
		if(NULL == pVertices)
			ExitProcess(DXTRACE_ERR( TEXT("ShadowVolume: no vertices"), E_INVALIDARG ));
		switch (m_model->GetFVF())
		{
		case VERTEXTEX::FVF :
			return ((VERTEXTEX *) pVertices) [pPos].pos;
		case VERTEXPLAIN::FVF :
			return ((VERTEXPLAIN *) pVertices) [pPos].pos;
		/*case VECTOR3VERTEX::FVF :
			return ((VECTOR3VERTEX *) pVertices) [pPos].pos;*/
		case VERTEXCOLOR::FVF :
			return ((VERTEXCOLOR *) pVertices) [pPos].pos;
		default :
			ExitProcess(DXTRACE_ERR( TEXT("ShadowVolume: unknown FVF type"), E_INVALIDARG ));
		}
	}

	HRESULT BuildFromMesh( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		HRESULT hr;

		if(NULL == m_model)
			return E_FAIL;

		D3DXVECTOR3 vLight;

		FLOAT Lx = - light.Direction.x;
		FLOAT Ly = - light.Direction.y;
		FLOAT Lz = light.Direction.z;

		// Transform the light vector to be in object space
		D3DXMATRIXA16 m_world, m;
		//pd3dDevice->GetTransform(D3DTS_VIEW, &m_view);
		pd3dDevice->GetTransform(D3DTS_WORLD, &m_world);
		//D3DXMatrixMultiply( &m_world, &m_world, &m_view );
		D3DXMatrixInverse( &m, NULL, &m_world );
		vLight.x = Lx*m._11 + Ly*m._21 + Lz*m._31 + m._41;
		vLight.y = Lx*m._12 + Ly*m._22 + Lz*m._32 + m._42;
		vLight.z = Lx*m._13 + Ly*m._23 + Lz*m._33 + m._43;

		LPVOID pVertices;
		DWORD* pIndices;

		// Lock the geometry buffers
		m_model->LockVertexBuffer( 0L, &pVertices );
		m_model->LockIndexBuffer( 0L, (LPVOID*) &pIndices );
		DWORD dwNumFaces    = m_model->GetNumFaces();

		// Allocate a temporary edge list
		DWORD* pEdges = new DWORD[dwNumFaces*6];
		if( NULL == pEdges )
		{
			m_model->UnlockVertexBuffer();
			m_model->UnlockIndexBuffer();
			return E_OUTOFMEMORY;
		}
		DWORD dwNumEdges = 0;

		// For each face
		for( DWORD i=0; i<dwNumFaces; i++ )
		{
			DWORD wFace0 = pIndices[3*i+0];
			DWORD wFace1 = pIndices[3*i+1];
			DWORD wFace2 = pIndices[3*i+2];

			D3DXVECTOR3 v0 = CastHelper(pVertices, wFace0);
			D3DXVECTOR3 v1 = CastHelper(pVertices, wFace1);
			D3DXVECTOR3 v2 = CastHelper(pVertices, wFace2);

			// Transform vertices or transform light?
			D3DXVECTOR3 vCross1(v2-v1);
			D3DXVECTOR3 vCross2(v1-v0);
			D3DXVECTOR3 vNormal;
			D3DXVec3Cross( &vNormal, &vCross1, &vCross2 );

			if( D3DXVec3Dot( &vNormal, &vLight ) < 0.0f )
			{
				AddEdge( pEdges, dwNumEdges, wFace0, wFace1 );
				AddEdge( pEdges, dwNumEdges, wFace1, wFace2 );
				AddEdge( pEdges, dwNumEdges, wFace2, wFace0 );
			}
		}

		VECTOR3VERTEX * m_pVertices = NULL;
		DWORD * m_pIndices = NULL;
		DWORD m_dwCurrentVert = 0, m_dwCurrentIndex = 0;

		SAFE_RELEASE(m_shadow)
			V_RETURN(D3DXCreateMeshFVF(
			dwNumEdges * 6,
			dwNumEdges * 4,
			D3DXMESH_32BIT
			| D3DXMESH_IB_MANAGED
			| D3DXMESH_VB_MANAGED
			| D3DXMESH_VB_WRITEONLY
			| D3DXMESH_IB_WRITEONLY,
			VECTOR3VERTEX::FVF,
			pd3dDevice,
			&m_shadow));
		if(FAILED ((hr = m_shadow->LockVertexBuffer(NULL, (LPVOID *) &m_pVertices))))
		{
			SAFE_RELEASE(m_shadow);
			return DXTRACE_ERR( TEXT("Cannot lock vertex buffer"), hr );
		}
		if(FAILED ((hr = m_shadow->LockIndexBuffer(NULL, (LPVOID *) &m_pIndices))))
		{
			SAFE_RELEASE(m_shadow);
			return DXTRACE_ERR( TEXT("Cannot lock index buffer"), hr );
		}

		for( DWORD i=0; i<dwNumEdges; i++ )
		{
#define	SHADOW_V1 (m_dwCurrentVert)
#define	SHADOW_V2 (m_dwCurrentVert + 1)
#define	SHADOW_V3 (m_dwCurrentVert + 2)
#define	SHADOW_V4 (m_dwCurrentVert + 3)
			D3DXVECTOR3 v1 = CastHelper(pVertices, pEdges[2*i+0]);
			D3DXVECTOR3 v2 = CastHelper(pVertices, pEdges[2*i+1]);
			D3DXVECTOR3 v3 = v1 - vLight*ZFAR;
			D3DXVECTOR3 v4 = v2 - vLight*ZFAR;

			m_pVertices[SHADOW_V1].pos = v1;
			m_pVertices[SHADOW_V2].pos = v2;
			m_pVertices[SHADOW_V3].pos = v3;
			m_pVertices[SHADOW_V4].pos = v4;

			// Add a quad (two triangles) to the vertex list
			m_pIndices[m_dwCurrentIndex++] = SHADOW_V1;
			m_pIndices[m_dwCurrentIndex++] = SHADOW_V2;
			m_pIndices[m_dwCurrentIndex++] = SHADOW_V3;

			m_pIndices[m_dwCurrentIndex++] = SHADOW_V2;
			m_pIndices[m_dwCurrentIndex++] = SHADOW_V4;
			m_pIndices[m_dwCurrentIndex++] = SHADOW_V3;

			m_dwCurrentVert += 4;
#undef SHADOW_V1
#undef SHADOW_V2
#undef SHADOW_V3
#undef SHADOW_V4
		}
		V_RETURN(m_shadow->UnlockIndexBuffer());
		V_RETURN(m_shadow->UnlockVertexBuffer());

		// Delete the temporary edge list
		delete[] pEdges;

		// Unlock the geometry buffers
		m_model->UnlockVertexBuffer();
		m_model->UnlockIndexBuffer();

		MyInvalidatedShadow = false;

		return S_OK;
	}

	LPD3DXMESH    m_shadow;
};

// ########################################################
// Nazwa: class CGhostMesh
// Opis: Klasa przechowywania i rysowania modelu.
//       Klasa odrysowuje model bez zape³niania Z-bufora.
//       Efektem jest brak rzucania lub przyjmowania cienia
//       przez modele rysowana t¹ klas¹.
// ########################################################
class CGhostMesh : public CMesh
{
public:
	CGhostMesh(const INT p_ID) : CMesh(p_ID/*, D3DXMESH_VB_WRITEONLY | D3DXMESH_IB_WRITEONLY*/)
	{
	}
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice)
	{
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		CMesh::OnFrameRender( pd3dDevice );
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	}
};

// ##################################################
// Nazwa: class CKombajnGraf
// Opis: Tablica inicjalizowania, przechowywania,
//       rysowania oraz zwalniania dowolnych klas
//       bazuj¹cych na CObiektGraf.
//
//       W trybie DEBUG mo¿e zapisaæ tablicê
//       prymitywów jako jeden model.
//       Produkcyjnie ten zapis nie jest wkompilowany.       
// ###################################################
class CKombajnGraf : public CObiektGraf
{
public:
#if defined(CPP_RAW_PRIMITIVE)
	CKombajnGraf(const INT p_ID = NULL) : m_ID(p_ID)
	{
		mFileName[0] = 'D';
		mFileName[1] = 'u';
		mFileName[2] = 'm';
		mFileName[3] = 'm';
		mFileName[4] = 'y';
		mFileName[5] = '.';
		mFileName[6] = 'x';
		mFileName[7] = 0;
		SavingTex = NULL;
		SavingPlain = NULL;
		SavingColor = NULL;
	}
#else
	CKombajnGraf()
	{
	}
#endif

	// OnFrameRender odrysowuje ca³¹ tablicê klas.
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		for (int i = 0; i < m_obiekty.GetSize(); i++)
			m_obiekty.GetAt(i)->OnFrameRender( pd3dDevice );
	}

	// OnCastShadow odrysowuje cieñ dla klas, które go implementuj¹.
	virtual void OnCastShadow( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		for (int i = 0; i < m_obiekty.GetSize(); i++)
			m_obiekty.GetAt(i)->OnCastShadow( pd3dDevice );
	}

	// OnFrameMove - kalkulacje dla poszczególnych klas przed kolejn¹ klatk¹.
	virtual void OnFrameMove( )
	{
		for (int i = 0; i < m_obiekty.GetSize(); i++)
			m_obiekty.GetAt(i)->OnFrameMove( );
	}

	// OnCreateDevice, np. za³adowanie zasobów.
	virtual HRESULT OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
	{
		HRESULT hr;

		for (int i = 0; i < m_obiekty.GetSize(); i++)
		{
			V (m_obiekty.GetAt(i)->OnCreateDevice( pd3dDevice, pBackBufferSurfaceDesc ));
			if( FAILED( hr ) )
			{
				OnDestroyDevice();
				return hr;
			}
		}
#if defined(CPP_RAW_DUMP)
		if(m_obiekty.GetSize() > 0)
		{
			if( m_obiekty.GetAt(m_obiekty.GetSize()-1)->GetFVF() == VERTEXTEX::FVF)
				SavingTex = new CSavingMeshTex(pd3dDevice, this, mFileName, m_ID);
			else if( m_obiekty.GetAt(m_obiekty.GetSize()-1)->GetFVF() == VERTEXPLAIN::FVF)
				SavingPlain = new CSavingMeshPlain(pd3dDevice, this, mFileName, m_ID);
			else if( m_obiekty.GetAt(m_obiekty.GetSize()-1)->GetFVF() == VERTEXCOLOR::FVF)
				SavingColor = new CSavingMeshColor(pd3dDevice, this, mFileName, m_ID);

			if(SavingTex)
				SavingTex->OnCreateDevice( pd3dDevice, pBackBufferSurfaceDesc );
			else if(SavingPlain)
				SavingPlain->OnCreateDevice( pd3dDevice, pBackBufferSurfaceDesc );
			else if(SavingColor)
				SavingColor->OnCreateDevice( pd3dDevice, pBackBufferSurfaceDesc );
		}
#endif

		return S_OK;
	}
	/*virtual HRESULT OnResetDevice( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		HRESULT hr;

		for (int i = m_obiekty.GetSize() - 1; i >= 0; i--)
		{
			V (m_obiekty.GetAt(i)->OnResetDevice( pd3dDevice ));
			if( FAILED( hr ) )
			{
				OnLostDevice();
				return hr;
			}
		}

		return S_OK;
	}*/

	// Zwolnienie wszystkich zasobów.
	// Kolejnoœæ odwrotna do tworzenia obiektów.
	virtual void OnDestroyDevice( )
	{
		for (int i = m_obiekty.GetSize() - 1; i >= 0; i--)
			m_obiekty.GetAt(i)->OnDestroyDevice( );
	}
	/*virtual void OnLostDevice( )
	{
		for (int i = m_obiekty.GetSize() - 1; i >= 0; i--)
			m_obiekty.GetAt(i)->OnLostDevice( );
	}*/

	// Dodania nowego obiektu.
	// Zostanie on póŸniej zainicjalizowany,
	// przetworzony oraz zwolniony.
	HRESULT AddRenderedObject(CObiektGraf * p_obiekt)
	{
		HRESULT hr;
		if(NULL == p_obiekt)
			return DXTRACE_ERR( TEXT("Collection NULL value"), E_INVALIDARG );
		else
		{
			V (m_obiekty.Add(p_obiekt));
			if( FAILED (hr) )
				return DXTRACE_ERR( TEXT("Adding to collection failed"), E_FAIL );
			return m_obiekty.GetSize() - 1; /* Positive value is an index */
		}
	}

	// Pobranie wskaŸnika do jednego obiektu.
	CObiektGraf * GetAt(const int nIndex)
	{
		if(nIndex < 0 || nIndex >= GetSize())
		{
			DXTRACE_ERR( TEXT("Argument out of bound"), E_INVALIDARG );
			return NULL;
		}
		CObiektGraf * tmp = m_obiekty.GetAt( nIndex );
		if(NULL == tmp)
			DXTRACE_ERR( TEXT("NULL value in the render table"), E_FAIL );
		return tmp;
	}

	// Bie¿¹ca iloœæ klas.
	int GetSize()
	{
		return m_obiekty.GetSize();
	}

private:
	CObiektGrafArray m_obiekty;

#if defined(CPP_RAW_PRIMITIVE)
	// Zmienne do tworzenie mesh-y.
protected:
	CHAR mFileName [MAX_PATH];
	CSavingMeshTex * SavingTex;
	CSavingMeshPlain * SavingPlain;
	CSavingMeshColor * SavingColor;
	const INT m_ID;
#endif;
};

#endif