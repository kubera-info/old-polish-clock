// ###########################################################
// Dzwiek.h - pakiet dzwiêku generowanego przez DirectSound3d.
// ###########################################################

#ifndef _DZWIEK_H_
#define _DZWIEK_H_

#include <dsound.h>
#include "konfig.h"

// ########################################################
// Nazwa: class CDzwiek
// Opis: Klasa odpowiadaj¹ca za odtworzenie dŸwiêków
//       wytwarzanych przez chodz¹cy zegar.
//       Klasa dziedziczy po CObiektGtaf i jest kolejkowana
//       w czasie pracy animacji.
// ########################################################
class CDzwiek : public CObiektGraf
{
public:
	// Utworzenie klasy.
	CDzwiek(const HWND hMainWindow);
	// Za³adowanie dŸwiêków.
	HRESULT OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
	// Podczas rysowania grafiki klasa nie wykona ¿adnych czynnoœci.
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice) { };
	// Odegranie dzwiêku(ów).
	virtual void OnFrameMove( );
	// Ustawienie g³oœnoœci wg skali liniowej.
	void Ustaw(const DWORD pSlider);
	// Aktualizacja g³oœnoœci.
	HRESULT Glosnosc();
	void StopAll();
	~CDzwiek();
	// Odegranie d¿iêku pracy zegara.
	HRESULT GrajTik();
	// Odegranie pracy sprê¿yny gongu.
	HRESULT GrajGong();

private:
	// Utworzenie jednego dzwiêku z zasobu aplikacji.
	HRESULT AlokujDzwiek(CSound ** pDzwiek, const INT p_IDWAV);
	// Odegranie jednego dŸwiêku.
	HRESULT Graj(CSound * pDzwiek);

	CSoundManager * pSoundManager; 
	CSound * DzwTik, * DzwGong;
	LPDIRECTSOUND3DLISTENER m_pDSListener;		// 3D listener object.
	DS3DLISTENER            m_dsListenerParams;	// Listener properties.
	
	long nVolume; // Bie¿¹ca g³oœnoœæ.
	long nIlSekund; // Iloœæ odegranych sekund dla gongu.
	SYSTEMTIME m_poprzedz_czas; // Czas ostatniej operacji, pozwala wykryæ zmianê czasu bie¿¹cego.
};

#endif