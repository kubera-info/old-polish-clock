#include <atlbase.h>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#include "MonadException.h"
#include "..\..\Standard\MonadCatch.h"
#include "MonadThread.h"
#include "MonadTemplates.h"
#include "MonadAudio.h"
#include "AudioFile.h"
#include "MonadRelease.h"
#include "MonadCOM.h"
#include "MonadRelease.h"

#ifdef DEBUG 
#define OUTPUT_STR(x)
// OutputDebugString(x L"\n")
#else
#define OUTPUT_STR(x)
#endif

namespace MonadAudio
{
	static inline void MonadVoiceRelease( IXAudio2MasteringVoice * pInterface )
	{
		pInterface->DestroyVoice();
	}
	typedef boost::shared_ptr<IXAudio2> IMonadXAudio2;

	MonadThread::CMutex gSafeTransact;

	struct CWaveToPlayWithCallback : boost::noncopyable
	{
		CWaveToPlayWithCallback( const std::wstring & pFilename, CWaveCallback * pCallback )
			: mFilename( pFilename ), mCallback( pCallback ), bToPlay( true )
		{
		}
		bool IsCallback() const throw()
		{
			return NULL != mCallback;
		}
		inline const std::wstring & GetFilename() const throw()
		{
			return mFilename;
		}
		void BeforeWave()
		{
			if( IsCallback() && !MonadException::ExceptionInProgress() ) // External object may be invalid
				mCallback->BeforeWave( mFilename );
		}
		void AfterWave()
		{
			if( IsCallback() && !MonadException::ExceptionInProgress() )
				mCallback->AfterWave( mFilename );
		}
		const bool & IfPlay() const
		{
			return bToPlay;
		}
		void InvalidatePlayOption()
		{
			bToPlay = false;
		}

	private:
		CWaveCallback * mCallback;
		bool bToPlay;
		const std::wstring mFilename;
	};

	typedef boost::shared_ptr<CWaveToPlayWithCallback> CAutoWaveToPlayWithCallback;
	typedef boost::shared_ptr<CMonadWaveBuffer> CMonadAutoWaveBuffer;
	typedef CMonadCoherentMap<std::wstring, const CMonadAutoWaveBuffer> CBuffers;

	class CSoundCEO : boost::noncopyable
	{
	public:
		HRESULT Init( const ULONG_PTR pVoicesCnt )
		{
			HRESULT hr = S_OK;
			UINT32 flags = 0;
#ifdef _DEBUG
#	ifndef _WIN64
			flags |= XAUDIO2_DEBUG_ENGINE;
#	endif
#endif
			IXAudio2 * p_tmpXAudio2( NULL );
			if( FAILED( hr = XAudio2Create( &p_tmpXAudio2, flags ) ) )
				return DXTRACE_ERR( TEXT("Failed to init XAudio2 engine"), hr );			  
			g_pXAudio2.reset( p_tmpXAudio2, MonadRelease );
			//
			// Create a mastering voice
			//
			IXAudio2MasteringVoice * p_tmpMasteringVoice( NULL );
			if( FAILED( hr = g_pXAudio2->CreateMasteringVoice( &p_tmpMasteringVoice ) ) )
				return DXTRACE_ERR( TEXT("Failed creating mastering voice"), hr );
			pMasteringVoice.reset( p_tmpMasteringVoice, MonadVoiceRelease );

			mVoices.reserve( pVoicesCnt );
			for( ULONG_PTR iCnt = 0; pVoicesCnt > iCnt; iCnt ++ )
			{
				CAutoVoiceManager pVoiceManager( new CVoiceManager( g_pXAudio2.get(), &mBuffers ) );
				mVoices.push_back( pVoiceManager );					  
			}

			return hr;
		}
		HRESULT Reset( const ULONG_PTR pQueue )
		{
			// MonadThread::CLock tmpLock( mVoices.GetMutex() );
			return mVoices.GetManager( pQueue )->Reset( g_pXAudio2.get() );
		}
		void CreateVoice( const std::wstring & pFilename, const BYTE * pBuffer, const DWORD pBufSize )
		{
			MonadThread::CLock tmpLock( mBuffers.GetMutex() );
			const CMonadAutoWaveBuffer tmpAuto( ReadAudioFile( pBuffer, pBufSize, mFormatSignatures ) );
			THROW_EXC_IFZ( tmpAuto.get(), MonadException::SkillNotImplemented, L"Unknown Audio Format" );

			CBuffers::iterator it = mBuffers.find( pFilename );
			mBuffers.insert( std::make_pair( pFilename, tmpAuto ) );
		}
		bool VoiceExists( const std::wstring & pFilename )
		{
			MonadThread::CLock tmpLock( mBuffers.GetMutex() );
			return mBuffers.end() != mBuffers.find( pFilename );
		}
		~CSoundCEO()
		{
			g_pXAudio2->StopEngine();
		}
		HRESULT PlayWave( CAutoWaveToPlayWithCallback & pWave, const ULONG_PTR pQueue )
		{
			HRESULT hr;

			hr = mVoices.GetManager( pQueue )->AudioEnqueueVoice( pWave );
			return hr;
		}
		HRESULT SetMasterVolume( float pVolume )
		{
			HRESULT hr;

			hr = pMasteringVoice->SetVolume( pVolume );
			return hr;
		}
		float GetMasterVolume()
		{
			float fVolume( 1.0 );

			pMasteringVoice->GetVolume( &fVolume );
			return fVolume;
		}
		HRESULT SetVoiceVolume( float pVolume, const ULONG_PTR pQueue )
		{
			return mVoices.GetManager( pQueue )->SetVoiceVolume( pVolume );
		}
		float GetVoiceVolume( const ULONG_PTR pQueue )
		{
			return mVoices.GetManager( pQueue )->GetVolume();
		}
		bool IsPlaying( const ULONG_PTR pQueue ) const
		{
			return mVoices.GetManager( pQueue )->IsPlaying();
		}
	private:		
		CCoInitialize mComInit;
		class CVoiceManager : public IXAudio2VoiceCallback, boost::noncopyable
		{
			friend CSoundCEO;
		public:
			CVoiceManager( IXAudio2 * pInXAudio2, const CBuffers * pInBuffers ) : pXAudio2( pInXAudio2 ), pBuffers( pInBuffers ), m_fVolume( 1.0f ) {}

			CMonadWaveBuffer * ValidateBuffer( const std::wstring & pFilename ) const
			{
				CBuffers::const_iterator it = pBuffers->find( pFilename );
				if( pBuffers->end() == it )
					THROW_EXC_ONERROR( MonadException::NoDataFound, ERROR_FILE_NOT_FOUND, L"Spawning audio buffer" );

				return it->second.get();
			}
			CMonadWaveBuffer * ValidateBuffer() const
			{
				return ValidateBuffer( mCurrentWave->GetFilename() );
			}
			HRESULT NewVoice( const CMonadWaveBuffer * pMonadWave )
			{
				OUTPUT_STR( L"NewVoice" );
				HRESULT hr = S_OK;

				if( DistinctVoices.end() == DistinctVoices.find( pMonadWave->GetWfx() ) )
				{
					OUTPUT_STR( L"NewVoice #2" );
					CAutoVoice NewVoice( new CVoice( pMonadWave->GetWfx() ) );
					hr = NewVoice->Reset( pXAudio2, this, m_fVolume );
					if( SUCCEEDED( hr ) )
						DistinctVoices.insert( std::make_pair( pMonadWave->GetWfx(), NewVoice ) );
				}

				return hr;
			}
			HRESULT AudioEnqueueVoice( const CAutoWaveToPlayWithCallback & pWave )
			{
				OUTPUT_STR( L"AudioEnqueueVoice" );
				HRESULT hr = S_OK;

				{
					MonadThread::CLock tmpLock( mVoiceCommandQueue.GetMutex() );
					mVoiceCommandQueue.push( pWave );
				}
				if( !IsPlaying() )
					hr = AudioDequeueWave();
				else
					hr = Spawn();

				return hr;
			}
			HRESULT AudioPlayWave( CAutoWaveToPlayWithCallback & pPendingWave, const CMonadWaveBuffer * pPendingBuffer )
			{
				OUTPUT_STR( L"AudioPlayWave" );
				HRESULT hr = S_OK;

				if( pPendingWave->IfPlay() )
				{
					OUTPUT_STR( L"AudioPlayWave #2" );
					pPendingWave->InvalidatePlayOption();
					V_RETURN( NewVoice( pPendingBuffer ) );
					return DistinctVoices[ pPendingBuffer->GetWfx() ]->PlayWave( pPendingBuffer );
				}
				return S_OK;
			}
			HRESULT Spawn()
			{
				OUTPUT_STR( L"Spawn" );

				MonadThread::CLock tmpLock( mVoiceCommandQueue.GetMutex() );
				if( !mVoiceCommandQueue.empty() )
				{
					OUTPUT_STR( L"Spawn #2" );
					MonadThread::CLock tmpLock( pBuffers->GetMutex() );
					if( NULL != mCurrentWave.get() )
					{
						CMonadWaveBuffer * pMonadWave = ValidateBuffer();
						CMonadWaveBuffer * pPendingWave = ValidateBuffer( mVoiceCommandQueue.front()->GetFilename() );
						if( pPendingWave->GetWfx() == pMonadWave->GetWfx() )
							return AudioPlayWave( mVoiceCommandQueue.front(), pPendingWave );
					}
				}
				return S_OK;
			}
			HRESULT AudioDequeueWave()
			{
				OUTPUT_STR( L"AudioDequeueWave" );
				HRESULT hr = S_OK;

				{
					MonadThread::CLock tmpLock( mVoiceCommandQueue.GetMutex() );
					if( !mVoiceCommandQueue.empty() )
					{
						OUTPUT_STR( L"AudioDequeueWave #2" );
						mCurrentWave = mVoiceCommandQueue.front();
						MonadThread::CLock tmpLock( pBuffers->GetMutex() );
						CMonadWaveBuffer * pMonadWave = ValidateBuffer();
						hr = AudioPlayWave( mCurrentWave, pMonadWave );
						mVoiceCommandQueue.pop();
					}
				}
				if( SUCCEEDED( hr ) )
					V_RETURN( Spawn() );
				return S_OK;
			}
			void BeforeWave()
			{
				OUTPUT_STR( L"BeforeWave" );
				if( mCurrentWave )
					mCurrentWave->BeforeWave();
			}
			void AfterWave()
			{
				OUTPUT_STR( L"BeforeWave" );
				if( mCurrentWave )
					mCurrentWave->AfterWave();
			}
			HRESULT Reset( IXAudio2 * pXAudio2 )
			{
				OUTPUT_STR( L"Reset" );
				HRESULT hr = S_OK;

				MonadThread::CLock tmpLock( mVoiceCommandQueue.GetMutex() );
				mVoiceCommandQueue.flush();
				CVoice * tmp = GetPlayingVoice();
				if( NULL != tmp )
					hr = tmp->FlushSourceBuffers( pXAudio2 );
				mCurrentWave.reset();

				return hr;
			}
			const bool IsPlaying() const 
			{
				OUTPUT_STR( L"IsPlaying" );
				return GetPlayingVoice() != NULL;
			}
			STDMETHOD_( void, OnVoiceProcessingPassStart )( UINT32 ) throw()
			{
			}
			STDMETHOD_( void, OnVoiceProcessingPassEnd )() throw()
			{
			}
			STDMETHOD_( void, OnStreamEnd )() throw()
			{
			}
			STDMETHOD_( void, OnBufferStart )( void* ) throw()
			{
				OUTPUT_STR( L"OnBufferStart" );
				if( !MonadException::ExceptionInProgress() )
				{
					try
					{
						// A semaphore for not loosing ptr...
						BeforeWave();
					}
					CATCH;
				}
			}
			STDMETHOD_( void, OnBufferEnd )( void* ) throw()
			{
				OUTPUT_STR( L"OnBufferEnd" );
				if( !MonadException::ExceptionInProgress() )
					try
				{					
					AfterWave();
					AudioDequeueWave();
				}
				CATCH;
			}
			STDMETHOD_( void, OnLoopEnd )( void* ) throw()
			{
			}
			STDMETHOD_( void, OnVoiceError )( void*, HRESULT ) throw()
			{
				struct CAudioShutdown : public MonadThread::IQue
				{
					// Processing function
					virtual void ProcessQueue() throw(...)
					{
						MonadAudio::ShutdownAudio();
						delete this;
					}
				};

				MonadThread::PushMonadQueue( new CAudioShutdown() );
			}

		private:
			float m_fVolume;
			IXAudio2 * pXAudio2;
			typedef CMonadCoherentQueue<CAutoWaveToPlayWithCallback> CVoiceCommandQueue;
			CVoiceCommandQueue mVoiceCommandQueue;
			CAutoWaveToPlayWithCallback mCurrentWave;
			const CBuffers * pBuffers;

			class CVoice sealed : boost::noncopyable
			{
			public:
				CVoice( const LPWAVEFORMATEX pWaveFormatEx ) throw()
					: pSourceVoice( NULL ), m_WaveFormatEx( pWaveFormatEx )
				{
				}
				~CVoice()
				{
					if( NULL != pSourceVoice )
						pSourceVoice->DestroyVoice();
				}

				HRESULT PlayWave( const CMonadWaveBuffer * pMonadWave ) const
				{
					HRESULT hr;

					// Submit the wave sample data using an XAUDIO2_BUFFER structure
					XAUDIO2_BUFFER buffer = {0};
					buffer.pAudioData = pMonadWave->GetBuffer();
					buffer.Flags = 0;  // tell the source voice not to expect any data after this buffer
					buffer.AudioBytes = pMonadWave->GetBufferLen();

					V_RETURN( pSourceVoice->SubmitSourceBuffer( &buffer, pMonadWave->GetBufferWMA() ) );
					return pSourceVoice->Start( 0 );
				}
				HRESULT SetVolume( float pVolume )
				{
					return pSourceVoice->SetVolume( pVolume );
				}

				HRESULT Reset( IXAudio2 * pXAudio2, IXAudio2VoiceCallback * pCallback, float p_fVolume )
				{
					HRESULT hr;

					V_RETURN( pXAudio2->CreateSourceVoice(
						&pSourceVoice, 
						m_WaveFormatEx,
						/* XAUDIO2_VOICE_NOSRC | */ XAUDIO2_VOICE_NOPITCH,
						2.000000f,
						pCallback ) );
					return pSourceVoice->SetVolume( p_fVolume );
				}
				HRESULT FlushSourceBuffers( IXAudio2 * pXAudio2 )
				{
					HRESULT hr;

					pXAudio2->StopEngine();
					/*V_RETURN*/( pSourceVoice->Stop( 0 ) );
					/*V_RETURN*/( pSourceVoice->FlushSourceBuffers() );										
					V_RETURN( pXAudio2->StartEngine() );
					/*while( IsPlaying() )
						Sleep( 0 );*/
					return S_OK;
				}

				bool IsPlaying() const
				{
					XAUDIO2_VOICE_STATE state;
					pSourceVoice->GetState( &state );
					return state.BuffersQueued > 0;
				}

			private:
				IXAudio2SourceVoice * pSourceVoice;
				LPWAVEFORMATEX m_WaveFormatEx;
			};
			CVoice * GetPlayingVoice()
			{
				MonadThread::CLock tmpLock( DistinctVoices.GetMutex() );
				CDistinctVoices::const_iterator found = find_if( DistinctVoices.begin(),
					DistinctVoices.end(),
					CIsPlaying() );

				return found != DistinctVoices.end() ? found->second.get() : NULL;
			}
			const CVoice * GetPlayingVoice() const
			{
				MonadThread::CLock tmpLock( DistinctVoices.GetMutex() );
				CDistinctVoices::const_iterator found = find_if( DistinctVoices.begin(),
					DistinctVoices.end(),
					CIsPlaying() );

				return found != DistinctVoices.end() ? found->second.get() : NULL;
			}

			typedef boost::shared_ptr< CVoice > CAutoVoice;
			typedef CMonadCoherentMap<LPWAVEFORMATEX, CAutoVoice> CDistinctVoices;
			CDistinctVoices DistinctVoices;
			struct CIsPlaying
			{
				bool operator() ( const CDistinctVoices::value_type & tmpVoice ) const throw()
				{
					return tmpVoice.second->IsPlaying();
				}
			};
			HRESULT SetVoiceVolume( float pVolume )
			{
				if( m_fVolume == pVolume )
					return S_OK;

				HRESULT hr( S_OK );
				m_fVolume = pVolume;
				BOOST_FOREACH( CDistinctVoices::value_type & OneVoice, DistinctVoices )
				{
					V_RETURN( OneVoice.second->SetVolume( pVolume ) );
				}
				return hr;
			}
			float GetVolume() const throw()
			{
				return m_fVolume;
			}
		};
		typedef boost::shared_ptr<CVoiceManager> CAutoVoiceManager;
		typedef CMonadCoherentVector<CAutoVoiceManager> CVoiceManagersRaw;
		struct CVoiceManagers : public CVoiceManagersRaw
		{
			CAutoVoiceManager & GetManager( const ULONG_PTR pQueue )
			{
				ValidateManagerGetter( pQueue );

				return CVoiceManagersRaw::operator[]( pQueue );
			}
			const CAutoVoiceManager & GetManager( const ULONG_PTR pQueue ) const
			{
				ValidateManagerGetter( pQueue );

				return CVoiceManagersRaw::operator[]( pQueue );
			}
			void operator[]( ULONG_PTR ) const;
			void ValidateManagerGetter( const ULONG_PTR pQueue ) const
			{
				assert( pQueue < size() );
			}
		};

		CWaveFormatExArray mFormatSignatures;
		CBuffers mBuffers;
		IMonadXAudio2 g_pXAudio2;		
		boost::shared_ptr<IXAudio2MasteringVoice> pMasteringVoice;
		CVoiceManagers mVoices;
	} static * g_pSoundCEO( NULL );

	void InternalShutdownAudio() throw()
	{
		SAFE_DELETE( g_pSoundCEO );
	}

	void InitAudio( const ULONG_PTR pVoicesCnt )
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() )
			THROW_EXC( MonadException::JobInProgress, E_FAIL, L"Start Audio" );

		g_pSoundCEO = new CSoundCEO();
		HRESULT hr = g_pSoundCEO->Init( pVoicesCnt );
		if( FAILED( hr ) )
			InternalShutdownAudio();
	}

	void ShutdownAudio() throw()
	{
		gSafeTransact.Enter();
		InternalShutdownAudio();
		gSafeTransact.Leave();
	}

	void EnqueueVoice( const std::wstring & pFilename, const ULONG_PTR pQueue, CWaveCallback * pCallback )
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() && !MonadException::ExceptionInProgress() )
		{
			CAutoWaveToPlayWithCallback tmpCall( new CWaveToPlayWithCallback( pFilename, pCallback ) );
			THROW_EXC_IFFAILED( MonadException::AudioFailed, g_pSoundCEO->PlayWave( tmpCall, pQueue ), L"XAudio2 Que #2" );
		}
	}

	void ResetQueue( const ULONG_PTR pQueue )
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() && !MonadException::ExceptionInProgress() )
		{
			THROW_EXC_IFFAILED( MonadException::AudioFailed, g_pSoundCEO->Reset( pQueue ), L"XAudio2 Reset #2" );
		}
	}

	void CreateVoice( const std::wstring & pFilename, const BYTE * pBuffer, const DWORD pBufSize )
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() && !MonadException::ExceptionInProgress() )
			g_pSoundCEO->CreateVoice( pFilename, pBuffer, pBufSize );
	}

	bool VoiceExists( const std::wstring & pFilename )
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() && !MonadException::ExceptionInProgress() )
			return g_pSoundCEO->VoiceExists( pFilename );

		return false;
	}

	void SetMasterVolume( float pVolume )
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() && !MonadException::ExceptionInProgress() )
			g_pSoundCEO->SetMasterVolume( pVolume );
	}

	float GetMasterVolume()
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() && !MonadException::ExceptionInProgress() )
			return g_pSoundCEO->GetMasterVolume();
		return 1.0f; // Dummy
	}

	void SetVoiceVolume( float pVolume, const ULONG_PTR pQueue )
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() && !MonadException::ExceptionInProgress() )
			g_pSoundCEO->SetVoiceVolume( pVolume, pQueue );
	}

	float GetVoiceVolume( const ULONG_PTR pQueue )
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() && !MonadException::ExceptionInProgress() )
			return g_pSoundCEO->GetVoiceVolume( pQueue );
		return 1.0f;
	}
	bool IsPlaying( const ULONG_PTR pQueue )
	{
		MonadThread::CLock tmpTransactLock( gSafeTransact );

		if( IsAudioOn() && !MonadException::ExceptionInProgress() )
			return g_pSoundCEO->IsPlaying( pQueue );
		else
			return false;
	}
	bool IsAudioOn() throw()
	{
		return NULL != g_pSoundCEO;
	}
}