#pragma once
#ifndef _MONAD_STREAMING_H_
#define _MONAD_STREAMING_H_

#include "MonadAudio.h"
#include "FileManager.h"
#include "Patterns.h"

struct CCue
{
	CCue() : m_fBPM( 0.0f ), m_fBar( 0.0f ), m_fPhrase( 0.0f ) {}
	CCue( float pBPM, float pBar, float pPhrase )
		: m_fBPM( pBPM ), m_fBar( pBar ), m_fPhrase( pPhrase )
	{
	}
	volatile const float m_fBPM;
	volatile const float m_fBar;
	volatile const float m_fPhrase;
};

struct CWaveStreaming sealed : public MonadAudio::CWaveCallback
{
	CWaveStreaming();
	~CWaveStreaming();

	// Processing function
	virtual void BeforeWave( const std::wstring & pFilename );
	// virtual void AfterWave( const std::wstring & pFilename );

	static void AddStream( const std::wstring & pFilename );
	//static void PostStream( const std::wstring & pFilename );

private:
	void StartOneStream();

	static CRandomString & GetVoiceName()
	{
		static CRandomString s_Streams;

		return s_Streams;
	}	

	/*class CStreamLoader : public CFileHolder
	{
	public:
		CStreamLoader( const std::wstring & pFolder, const std::wstring & pFilename );
		virtual void NotifyMainThread() { delete this; }; // Change default behaviour
	};*/
};

#endif