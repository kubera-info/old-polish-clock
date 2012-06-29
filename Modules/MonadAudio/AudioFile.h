#pragma once
#ifndef _MONAD_AUDIO_FILE_H_
#define _MONAD_AUDIO_FILE_H_

#include "DXUT.h"
#include <mmsystem.h>
#include <dxerr.h>
#include <xaudio2.h>
#include <assert.h>
#include <boost/shared_ptr.hpp>
#include "ByteArray.h"
#include "MonadTemplates.h"

namespace MonadAudio
{
	struct MONAD_XAUDIO2_BUFFER_WMA : public XAUDIO2_BUFFER_WMA
	{
		MONAD_XAUDIO2_BUFFER_WMA()
		{
			pDecodedPacketCumulativeBytes = NULL;
		}
		~MONAD_XAUDIO2_BUFFER_WMA()
		{
			if( pDecodedPacketCumulativeBytes )
				delete [] pDecodedPacketCumulativeBytes;
		}
	};

	typedef CAutoPtr<MONAD_XAUDIO2_BUFFER_WMA> CWMABuffer;

	typedef boost::shared_ptr<WAVEFORMATEX> CAutoWaveFormatEx;

	bool operator<( const WAVEFORMATEX & pLeftFormat, const WAVEFORMATEX & pRightFormat );
	struct LessMonadWave
	{
		bool operator()( const CAutoWaveFormatEx & pLeft, const CAutoWaveFormatEx & pRight ) const
		{
			return *pLeft.get() < *pRight.get();
		}
	};

	typedef CMonadCoherentSet<CAutoWaveFormatEx, LessMonadWave> CWaveFormatExArray;

	struct CMonadWaveBuffer
	{
	public:
		CMonadWaveBuffer(
			const LPWAVEFORMATEX pwfx,
			DWORD pBufferLen,
			BYTE * pBuffer,
			CWMABuffer & pxWMA,
			CWaveFormatExArray & pFormats )
			: m_wfx( GetWaveFormatEx( pwfx, pFormats ) ),
			m_BufferLen( pBufferLen ),
			m_Buffer( pBuffer ),
			m_xWMA( pxWMA )
		{
		}
		CMonadWaveBuffer( const CMonadWaveBuffer &Source );
		void operator=( const CMonadWaveBuffer &Source );
		inline const XAUDIO2_BUFFER_WMA * GetBufferWMA() const throw()
		{
			return m_xWMA;
		}
		inline BYTE * GetBuffer() const throw()
		{
			return m_Buffer;
		}
		inline const DWORD & GetBufferLen() const throw()
		{
			return m_BufferLen;
		}
		inline const LPWAVEFORMATEX GetWfx() const throw()
		{
			return m_wfx;
		}

	private:
		const LPWAVEFORMATEX m_wfx;
		const DWORD m_BufferLen;
		const CAutoSimpleVectorBytePtr m_Buffer;
		const CWMABuffer m_xWMA;
		static const LPWAVEFORMATEX GetWaveFormatEx( const LPWAVEFORMATEX pPossiblyNewFormat, CWaveFormatExArray & pFormats );
	};
	struct CMonadWaveBuffer * ReadAudioFile( const BYTE * pInBuffer, DWORD p_dwInSize, CWaveFormatExArray & pFormats );
}

#endif // _MONAD_AUDIO_FILE_H_