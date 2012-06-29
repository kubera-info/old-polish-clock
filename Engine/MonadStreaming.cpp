#include "DXUT.h"
#include "MonadStreaming.h"
#include "Singleton.h"
#include "OldPolishClock.h"
#include "FileManager.h"
#include "Path.h"
//#include "MonadThread.h"
#include "MonadTiming.h"
#include "MonadCatch.h"

CWaveStreaming::~CWaveStreaming()
{
	MonadAudio::ResetQueue();
}
CWaveStreaming::CWaveStreaming()
{
	StartOneStream();
}
void CWaveStreaming::BeforeWave( const std::wstring & pFilename )
{
	StartOneStream();
	const CCue CueParams( Singleton->MonadFactory.GetCue( pFilename ) );
	CAnimateChange::GetAnimateChange().Alter( CueParams.m_fBar * CueParams.m_fPhrase * 60.0f / CueParams.m_fBPM );
}
/*void CWaveStreaming::AfterWave( const std::wstring & pFilename )
{	
}*/
void CWaveStreaming::AddStream( const std::wstring & pFilename )
{
	GetVoiceName().AddString( pFilename );
}
void CWaveStreaming::StartOneStream()
{
	MonadAudio::EnqueueVoice( GetVoiceName(), 0, this );
}

/*CWaveStreaming::CStreamLoader::CStreamLoader( const std::wstring & pFolder, const std::wstring & pFilename )
: CFileHolder( pFolder, pFilename + szExtStrm )
{
}*/