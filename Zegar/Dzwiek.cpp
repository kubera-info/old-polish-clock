#include "stdafx.h"
#include <objbase.h>
#include "Dzwiek.h"

CDzwiek * g_pDzwiek = NULL;

// #############################################
// Nazwa: CDzwiek::CDzwiek()
// Opis: Inicjalizuje klasê do pracy w pe³ni
// #############################################
CDzwiek::CDzwiek()
{
	m_pMusicManager = NULL;
	m_pMusicSegment = NULL;
}

// #############################################
// Nazwa: CDzwiek::GrajTik()
// Opis: Tik/tak
// #############################################
HRESULT CDzwiek::GrajTik()
{
	if(m_pMusicSegment)
	{
		IDirectMusicSegment8 * m_isegm = m_pMusicSegment->GetSegment();	
		if(m_isegm)
			if(SUCCEEDED(m_isegm->SetStartPoint( DMUS_PPQ * 8 )))
				return m_pMusicSegment->Play( DMUS_SEGF_INVALIDATE_PRI );
	}
	return E_FAIL;
}

// #############################################
// Nazwa: CDzwiek::GrajGong()
// Opis: Gong + Tik/tak
// #############################################
HRESULT CDzwiek::GrajGong()
{	
	if(m_pMusicSegment)
	{
		IDirectMusicSegment8 * m_isegm = m_pMusicSegment->GetSegment();	
		if(m_isegm)
			if(SUCCEEDED(m_isegm->SetStartPoint( 0 )))
				return m_pMusicSegment->Play( DMUS_SEGF_INVALIDATE_PRI );
	}
	return E_FAIL;
}

// #############################################
// Nazwa: HRESULT CDzwiek::Glosnosc(const long pSlider)
// Opis: G³oœnoœæ w przeskalowanych Decybelach
// #############################################
HRESULT CDzwiek::Glosnosc(const WORD pSlider)
{
	IDirectMusicPerformance8 * pPerf = NULL;
	if(m_pMusicManager)
		pPerf = 
			m_pMusicManager->
				GetPerformance();
	if(pPerf)
	{
		// Na ka¿de kolejne +3 db przypada podwojenie glosnosci
		// Preferowana glosnosc w PSDK zawiera siê w <-100db, 10db>, i tak jest to zbyt wiele - :)
		// Obliczenie przekszta³ca skalê logarytmincz¹ w liniow¹ bez zmiany przedzia³u
		const static       skala = 100; // 1db => GUID_PerfMasterVolume
		const static float cmaxlog = 3.6109178f; // = logf(1.0f + 110.0f / 3);
		const        float csldlog = logf(1.0f + (float) pSlider / 3.0f);
		const              nVolume = (long) (((csldlog / cmaxlog) * 110 - 100) * skala);
		
		pPerf->SetGlobalParam( GUID_PerfMasterVolume, 
			(void*)&nVolume, sizeof(long) );  
		return S_OK;
	}
	return E_FAIL;

}
void CDzwiek::StopAll()
{
	if(m_pMusicManager)
	{
		IDirectMusicPerformance8 * Perf = m_pMusicManager->GetPerformance();
		if(Perf)
			Perf->Stop(NULL, NULL, 0, 0);
	}
}
// #############################################
// Nazwa: CDzwiek::~CDzwiek()
// Opis: Zatrzymuje dŸwiêk i zwalnia
//       wszystkie zasoby
// #############################################
CDzwiek::~CDzwiek()
{
	StopAll();

	// Release all remaining pointers
    SAFE_DELETE( m_pMusicSegment );
    SAFE_DELETE( m_pMusicManager );
}

CDzwiek::CZegarMusicManager::CZegarMusicManager()
{
	m_pLoader       = NULL;
	m_pPerformance  = NULL;
//	m_pDSListener   = NULL;
	
	// Initialize COM
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	m_bCleanupCOM = SUCCEEDED(hr);
}
