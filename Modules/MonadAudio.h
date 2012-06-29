#pragma once
#ifndef _MONAD_SOUNDS_H_
#define _MONAD_SOUNDS_H_

#include <string>

namespace MonadAudio
{

#define SND_DEFAULT_QUEUE (0)

	struct CWaveCallback abstract
	{
		// Processing function
		virtual void BeforeWave( const std::wstring & ) throw() { };
		virtual void AfterWave( const std::wstring & ) throw() { };
	};

	void InitAudio( const ULONG_PTR pVoicesCnt );

	void ShutdownAudio() throw();

	void CreateVoice( const std::wstring & pFilename, const BYTE * pBuffer, const DWORD pBufSize );

	// void DropVoice( const std::wstring & pFilename );

	bool VoiceExists( const std::wstring & pFilename );

	void EnqueueVoice( const std::wstring & pFilename, const ULONG_PTR pQueue = SND_DEFAULT_QUEUE, CWaveCallback * pCallback = NULL );

	void ResetQueue( const ULONG_PTR pQueue = SND_DEFAULT_QUEUE );

	void SetMasterVolume( float pVolume );

	float GetMasterVolume();

	void SetVoiceVolume( float pVolume, const ULONG_PTR pQueue = SND_DEFAULT_QUEUE );

	float GetVoiceVolume( const ULONG_PTR pQueue = SND_DEFAULT_QUEUE );

	bool IsPlaying( const ULONG_PTR pQueue = SND_DEFAULT_QUEUE );

	bool IsAudioOn() throw();
}

#endif
