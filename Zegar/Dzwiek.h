#ifndef _DZWIEK_H_
#define _DZWIEK_H_

// #############################################
// Nazwa: class CDzwiek
// Opis: Klasa oparta na DirectMusic
//       CDzwiek s³u¿y 
//       do odtwarzania jednego zasou MIDI
// #############################################
class CDzwiek
{
public:
	CDzwiek();
	// #############################################
	// Nazwa: metoda CDzwiek.Create()
	// Opis: Przygotowanie wymaganych obiektów COM
	//       DirectMusic wymaganych do odtwarzania
	//       zasobu MIDI
	// #############################################
	HRESULT Create()
	{
		HRESULT hr;
		m_pMusicManager = new CZegarMusicManager();
		if(SUCCEEDED(hr = m_pMusicManager->Initialize(NULL)))
			if(SUCCEEDED(m_pMusicManager->CreateSegmentFromResource(
				&m_pMusicSegment,
				MAKEINTRESOURCE(IDR_GONG), "MID",
				TRUE, TRUE)))
			{
				float m_fTempo = 6.0f / 5.0f;
				m_pMusicManager->GetPerformance()->
					SetGlobalParam(GUID_PerfMasterTempo, (VOID *) (&m_fTempo), sizeof(m_fTempo));
				Glosnosc(g_wGlosnosc);
				return S_OK;
			}
		SAFE_DELETE(m_pMusicSegment);
		SAFE_DELETE(m_pMusicManager);
		return E_FAIL;
	}
	HRESULT GrajTik();
	HRESULT GrajGong();
	HRESULT Glosnosc(const WORD pSlider);
	void StopAll();
	~CDzwiek();

protected:
		// #############################################
		// Nazwa: class CDzwiek
		// Opis: Klasa oparta na CMusicManager
		//       Ró¿nica miêdzy obiema klasami wystêpuje
	    //       w konstruktorze, ten u¿ywa nowszej
	    //       funkcji API dla interfejsów COM
		// #############################################
		class CZegarMusicManager : public CMusicManager
		{
		public:
			CZegarMusicManager();
		};

	CZegarMusicManager* m_pMusicManager; // Maganager DirectMusic
	CMusicSegment*      m_pMusicSegment; // Segement z dŸwiêkiem MIDI
} extern * g_pDzwiek;

#endif