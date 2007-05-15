#include "stdafx.h"
#include "Dzwiek.h"
#include "Zegar.h"
#include "Ogolne.h"

CDzwiek::CDzwiek(const HWND hMainWindow) : pSoundManager (new CSoundManager())
{
	if( FAILED ( pSoundManager->Initialize( hMainWindow, DSSCL_PRIORITY ) ) )
		SAFE_DELETE( pSoundManager );
	DzwTik = NULL;
	DzwGong = NULL;
	m_pDSListener = NULL;	// 3D listener object
	m_dsListenerParams;		// Listener properties
	nIlSekund = 60L;
}
HRESULT CDzwiek::OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	HRESULT hr;
	if(NULL == pSoundManager)
		return E_OUTOFMEMORY;
	V_RETURN(AlokujDzwiek(&DzwTik, IDR_TIK));
	V_RETURN(AlokujDzwiek(&DzwGong, IDR_GONG));
	V_RETURN(pSoundManager->Get3DListenerInterface( &m_pDSListener ));

	// Get listener parameters
	m_dsListenerParams.dwSize = sizeof(DS3DLISTENER);
	V_RETURN(m_pDSListener->GetAllParameters( &m_dsListenerParams ));
	m_dsListenerParams.vPosition = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	V_RETURN(m_pDSListener->SetAllParameters( &m_dsListenerParams, DS3D_IMMEDIATE ));

	Ustaw(g_wGlosnosc);

	GetLocalTime(&m_poprzedz_czas);

	return S_OK;
}
void CDzwiek::OnFrameMove( )
{
	static LONG_PTR m_il_gong = 0;

	if(D3DSaver::g_aktual_czas.wHour != m_poprzedz_czas.wHour)
	{
		m_il_gong = D3DSaver::g_aktual_czas.wHour % 12;
		if(m_il_gong == 0)
			m_il_gong = 12;
	}
	else if(D3DSaver::g_aktual_czas.wMinute == 30 && D3DSaver::g_aktual_czas.wMinute > m_poprzedz_czas.wMinute)
		m_il_gong = 1;

	if(D3DSaver::g_aktual_czas.wSecond != m_poprzedz_czas.wSecond) // Nowa sekunda?
	{			
		if(m_il_gong > 0)
			GrajGong();
		m_il_gong --;
		GrajTik();
	}
	m_poprzedz_czas = D3DSaver::g_aktual_czas;
}
void CDzwiek::Ustaw(const DWORD pSlider)
{
	// Na ka¿de kolejne +3 db przypada podwojenie gloœnoœci
	// Obliczenie przekszta³ca skalê logarytmincz¹ w liniow¹ bez zmiany przedzia³u
	const static float skala = 100;
	const static float cmaxlog = logf(1.0f + 100.0f / 3);
	const        float csldlog = logf(1.0f + (float) pSlider / 30.0f);
	nVolume = (long) ((((csldlog / cmaxlog) * skala - skala) /* ujemna skala */ ) * skala /* il. jedn * 1db */ );
}
HRESULT CDzwiek::Glosnosc()
{
	HRESULT hr;

	LPDIRECTSOUNDBUFFER pDSB = DzwTik->GetBuffer( 0 );
	if(pDSB)
	{
		V_RETURN(pDSB->SetVolume(nVolume));
	}
	else
		return E_FAIL;
	pDSB = DzwGong->GetBuffer( 0 );
	if(pDSB)
	{
		V_RETURN(hr = pDSB->SetVolume(nVolume));
	}
	else
		return E_FAIL;
	return S_OK;
}
HRESULT CDzwiek::Graj(CSound * pDzwiek)
{	
	HRESULT hr;

	if(pDzwiek)
	{
		V_RETURN(pDzwiek->Stop());
		V_RETURN(pDzwiek->Reset());
		return pDzwiek ? pDzwiek->Play(0, 0, nVolume) : E_FAIL;
	}
	else
		return E_FAIL;
}
HRESULT CDzwiek::GrajTik()
{
	if( g_bOpadanie )
	{
		Ustaw(max(long(float(g_wGlosnosc) * float(nIlSekund) / 60.0f), 0L));
		Glosnosc();
		nIlSekund --;
	}
	return Graj(DzwTik);
}
HRESULT CDzwiek::GrajGong()
{
	return Graj(DzwGong);
}
HRESULT CDzwiek::AlokujDzwiek(CSound ** pDzwiek, const INT p_IDWAV)
{
	HRESULT hr;

	TCHAR Label [8] = L"\0";
	swprintf_s(
		Label, 
		sizeof(Label) / sizeof(TCHAR),
		L"#%d",
		p_IDWAV);

	V(pSoundManager->Create(pDzwiek, Label, DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D, DS3DALG_HRTF_FULL));
	return hr;
}
void CDzwiek::StopAll()
{
	if(DzwTik)
	{
		DzwTik->Stop();
		DzwTik->Reset();
	}
	if(DzwGong)
	{
		DzwGong->Stop();
		DzwGong->Reset();
	}
}
CDzwiek::~CDzwiek()
{
	StopAll();
	SAFE_RELEASE( m_pDSListener );
	SAFE_DELETE(DzwTik);
	SAFE_DELETE(DzwGong);
	SAFE_DELETE(pSoundManager);
}
