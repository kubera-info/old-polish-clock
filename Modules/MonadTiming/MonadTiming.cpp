#include "MonadTiming.h"
#include "MonadTime.h"
#include "Singleton.h"
#include <limits>
#include "MonadTTS.h"
#include "MetaDefine.h"

using namespace std;

CWaveStreaming * Streaming( NULL );
CTicToc * TicToc( NULL );
CBell * Bell( NULL );

CTicToc::CTicToc()
{
	RegisterNewSecondEvent();
}
void CTicToc::OnNewSecond()
{
	if( MonadAudio::GetVoiceVolume( 1 ) > 0.0f )
		MonadAudio::EnqueueVoice( L"tic", 1 );
};

CBell::CBell()
{
	RegisterNewSecondEvent();
}
void CBell::OnNewSecond()
{
	static ULONG_PTR s_lBell( 0UL );
	if( 0UL < s_lBell )
	{
		MonadAudio::SetVoiceVolume( 1.0f, 2 );
		MonadAudio::ResetQueue( 2 );
		MonadAudio::EnqueueVoice( L"gong", 2 );
		s_lBell --;
		if( 0UL == s_lBell )
			SayTime( g_aktual_czas );
	}
	else if( 59 == g_aktual_czas.wSecond )
	{
		if( 59 == g_aktual_czas.wMinute )
		{
			s_lBell = ( g_aktual_czas.wHour + 1 ) % 12;
			if( 0UL == s_lBell )
				s_lBell = 12UL;
		}
		else if( 29 == g_aktual_czas.wMinute )
			s_lBell = 1UL;
	}
};

static const DWORD g_StepFactor = 4;

CMonadTiming::CMonadTiming()
{
	SetLerpParams(
		3.0, 
		0.0, 
		0.1 );
}

bool CMonadTiming::IsWarming() throw()
{
	return !IsFinished();
}

void CMonadTiming::OnStop()
{
	if( MonadAudio::IsAudioOn() )
	{
		if( NULL == Streaming )
			Streaming = new CWaveStreaming();
		if( NULL == TicToc )
			TicToc = new CTicToc();
		if( NULL == Bell )
			Bell = new CBell();
	}
}

void SetAllVolumes( float pStreamVolume )
{
	MonadAudio::SetVoiceVolume( pStreamVolume );
	MonadAudio::SetVoiceVolume( 1.0f - pStreamVolume, 1 );
}

void CalculateFallingVolume()
{
	if( 59 == g_aktual_czas.wMinute || 29 == g_aktual_czas.wMinute )
	{
		if( g_aktual_czas.wSecond > 54 )
		{
			const WORD wSecond = 60 - g_aktual_czas.wSecond;
			const WORD wMilliseconds = 1000 - g_aktual_czas.wMilliseconds;
			SetAllVolumes( 
				( float( wSecond ) + float( wMilliseconds ) / 1000.0f ) / 6.0f );
		}
	}	
	else if( MonadAudio::GetVoiceVolume( 0 ) == 0.0f && !Singleton->mRestoreVolume.IsRunning() && !MonadAudio::IsPlaying( 2 ) )
	{
		Singleton->mRestoreVolume.Reset();
		Singleton->mRestoreVolume.Start();
	}
}

CMonadVolumeRestore::CMonadVolumeRestore()
{
	SetLerpParams(
		6.0, 
		0.0, 
		1.0 );
}
void CMonadVolumeRestore::OnFrameMove()
{
	SetAllVolumes( float( GetCurrentValue() ) );
}

void CMonadVolumeRestore::OnStop()
{
	SetAllVolumes( 0.0f );
}
