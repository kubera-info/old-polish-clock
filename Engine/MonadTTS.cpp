#include "DXUT.h"
#include "Singleton.h"
#include "MonadException.h"
#include "MonadAudio.h"
#include "MonadTemplates.h"
#include "MonadTiming.h"
#include "I18N.h"

// STL is safe enough in this code
// typedef CMonadCoherentMap<ULONG_PTR, CSpeech> COneLanguageSpeakBuffer;
typedef CStringArray CSpeech;
typedef std::map<ULONG_PTR, CSpeech> COneLanguageSpeakBuffer;

struct COneValidLanguageSpeakBuffer : public COneLanguageSpeakBuffer
{
	void SayOneLanguageTime( ULONG_PTR pTime )
	{
		const_iterator it( find( pTime ) );
		assert( end() != it );

		BOOST_FOREACH( const std::wstring & mFilename, it->second )
		{
			MonadAudio::EnqueueVoice( mFilename, 2 );
		}
	}
} PL, RU, EN;

void InsertNewVoice( COneLanguageSpeakBuffer & pBuffer, ULONG_PTR pTime, const std::wstring & pVoice )
{
	CSpeech & sp( pBuffer.insert( std::make_pair( pTime, CSpeech() ) ).first->second );
	sp.push_back( pVoice );
}

void PushTTSDefinition( const std::wstring & pLang, ULONG_PTR pTime, const std::wstring & pVoice )
{
	if( pLang == L"pl" )
		InsertNewVoice( PL, pTime, pVoice );
	else if( pLang == L"ru" )
		InsertNewVoice( RU, pTime, pVoice );
	else if( pLang == L"en" )
		InsertNewVoice( EN, pTime, pVoice );
	else
		THROW_EXC( MonadException::InvalidCall, E_FAIL, L"Speech I18N" );
}

void SayTime( const SYSTEMTIME & pTime )
{
	SetAllVolumes( 0.0f );
	const ULONG_PTR NumTime = pTime.wHour * 100 + pTime.wMinute;
	MonadAudio::SetVoiceVolume( 1.25f, 2 );
	switch( MonadI18N::GetLanguage() )
	{
	case LANG_PL :
		PL.SayOneLanguageTime( NumTime );
		break;
	case LANG_RU :
		RU.SayOneLanguageTime( NumTime );
		break;
	default :
		EN.SayOneLanguageTime( NumTime );
	}
}