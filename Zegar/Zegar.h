#pragma once

// #############################################
// Nazwa: class CD3DZegarSaver
//        : public CD3DScreensaver
// Opis: Wygaszacz ekranu
// #############################################
class CD3DZegarSaver : public CD3DScreensaver
{	
	virtual HRESULT Render();
	virtual HRESULT FrameMove();
	void RenderAllShadows();
	HRESULT RenderShadow();
	HRESULT DrawShadow();
	void BuildProjectionMatrix2( );
	void LoadTexture(INT p_IDB, LPDIRECT3DTEXTURE8 & p_texture);
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT ConfirmDevice( D3DCAPS8* pCaps, DWORD dwBehavior,
											D3DFORMAT Format );
	virtual HRESULT OneTimeSceneInit();
	virtual BOOL    GetTextForError( HRESULT hr, TCHAR* pszError, DWORD dwNumChars );
	virtual VOID DoConfig();
	virtual VOID ReadSettings();
	virtual VOID WriteSettings();
	void Zeruj(); // Przypisz NULL itp.

	//Direct3D
	CTarcza * m_tarcza;
	CWskazGodz * m_wsk_godz;
	CWskazMin * m_wsk_min;
	CWskazSec * m_wsk_sec;
	CMontaz * m_montaz;
	D3DLIGHT8 m_tmp_light;
	float m_fSwiatlo;
	bool m_bTwoSided;
	D3DXMATRIXA16 m_proj, m_view;

	// Inne
	SYSTEMTIME m_poprzedz_czas;

public:
	CD3DZegarSaver(const bool Stencil) : CD3DScreensaver()
	{
		// Windows
		lstrcpy(m_strWindowTitle, _T("Wygaszacz \"Zegar3D\""));
		lstrcpy(m_strRegPath, g_reg);

		// Direct3D
		Zeruj();
		m_bOneScreenOnly	= TRUE;
		if(Stencil)
		{
			m_bUseDepthBuffer   = TRUE;
			g_bUseDepthBuffer   = TRUE;
			//m_dwMinDepthBits    =   16;
			m_dwMinStencilBits  =    4;
		}
		else
			g_bUseDepthBuffer   = FALSE;
	}
	HRESULT Get_hrError()
	{
		return m_hrError;
	}
	BOOL GetConfigErrorMode()
	{
		return (m_bErrorMode && (m_SaverMode == sm_config));
	}
	~CD3DZegarSaver()
	{
		// Stop playback
		SAFE_DELETE(g_pDzwiek);
	}
	/* Destruktory, które by³y wo³ane dopiero po dezaktywacji zegara zosta³y usuniête,
	   gdy¿ tylko fragmentarycznie zwalania³y pamiêæ */

	HRESULT Create( HINSTANCE hInstance );
};
