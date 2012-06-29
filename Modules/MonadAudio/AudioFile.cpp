//#include "stdafx.h"
#include "AudioFile.h"
#include "MonadException.h"
#include "MonadLock.h"

namespace MonadAudio
{
	// Based on http://forums.xna.com/forums/t/29824.aspx

	bool operator<( const WAVEFORMATEX & pLeftFormat, const WAVEFORMATEX & pRightFormat )
	{
		if( pLeftFormat.cbSize < pRightFormat.cbSize )
			return true;
		else if( pLeftFormat.cbSize > pRightFormat.cbSize )
			return false;
		return 0 < memcmp( &pLeftFormat, &pRightFormat, sizeof( WAVEFORMATEX ) + pRightFormat.cbSize );
	}

	const LPWAVEFORMATEX CMonadWaveBuffer::GetWaveFormatEx( const LPWAVEFORMATEX pPossiblyNewFormat, CWaveFormatExArray & pFormats )
	{
		MonadThread::CLock tmpLock( pFormats.GetMutex() );
		CAutoWaveFormatEx tmpPossiblyNewFormat( pPossiblyNewFormat );
		return pFormats.insert( tmpPossiblyNewFormat ).first->get();
	}

	struct DataBuffer
	{  
		UINT32 size;  
		BYTE *data;  
	};  

	/*WAVEFORMATEX *ReadWave( DataBuffer *buffer, HMMIO hmmio )  
	{  
		MMCKINFO riff;  
		// Search the input file for for the 'fmt ' chunk.  
		riff.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );  
		if( 0 != mmioDescend( hmmio, &riff, NULL, MMIO_FINDCHUNK ) )  
		{  
			return NULL;  
		}  

		// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;  
		// if there are extra parameters at the end, we'll ignore them  
		if( riff.cksize < ( LONG )sizeof( PCMWAVEFORMAT ) )  
		{  
			return NULL;  
		}  

		// Read the 'fmt ' chunk into <pcmWaveFormat>.  
		PCMWAVEFORMAT pcmWaveFormat;  // Temp PCM structure to load in.  

		if( mmioRead( hmmio, ( HPSTR )&pcmWaveFormat, sizeof( pcmWaveFormat ) ) != sizeof( pcmWaveFormat ) )  
		{  
			return NULL;  
		}  

		// Allocate the waveformatex, but if its not pcm format, read the next  
		// word, and thats how many extra bytes to allocate.  
		assert( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM );  
		const int formatSize = sizeof( WAVEFORMATEX );
		WAVEFORMATEX *wfx = ( WAVEFORMATEX* )new BYTE[ formatSize ];  

		// Copy the bytes from the pcm structure to the waveformatex structure  
		memcpy( wfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );  
		wfx->cbSize = 0;  

		// Ascend the input file out of the 'fmt ' chunk.  
		if( 0 != mmioAscend( hmmio, &riff, 0 ) )  
		{  
			SAFE_DELETE_ARRAY( wfx );  
			return NULL;  
		}  

		riff.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );  
		if( 0 != mmioDescend( hmmio, &riff, NULL, MMIO_FINDCHUNK ) )  
		{  
			SAFE_DELETE_ARRAY( wfx );  
			return NULL;  
		}  
		buffer->size = riff.cksize;  
		buffer->data = new BYTE[ riff.cksize ];  
		if( mmioRead( hmmio, ( HPSTR )buffer->data, riff.cksize ) != ( LONG )riff.cksize )  
		{  
			SAFE_DELETE_ARRAY( wfx );  
			SAFE_DELETE_ARRAY( buffer->data );  
			return NULL;  
		}     
		return wfx;  
	}*/  

	WAVEFORMATEX *ReadXWMA( DataBuffer *buffer, MONAD_XAUDIO2_BUFFER_WMA *extraInfo, HMMIO hmmio )  
	{  
		MMCKINFO riff;  
		// Search the input file for for the 'fmt ' chunk.  
		riff.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );  
		if( 0 != mmioDescend( hmmio, &riff, NULL, MMIO_FINDCHUNK ) )  
		{  
			return NULL;  
		}  

		// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;  
		// if there are extra parameters at the end, we'll ignore them  
		if( riff.cksize < ( LONG )sizeof( PCMWAVEFORMAT ) )  
		{  
			return NULL;  
		}  

		// Read the 'fmt ' chunk into <pcmWaveFormat>.  
		PCMWAVEFORMAT pcmWaveFormat;  // Temp PCM structure to load in.  

		if( mmioRead( hmmio, ( HPSTR )&pcmWaveFormat, sizeof( pcmWaveFormat ) ) != sizeof( pcmWaveFormat ) )  
		{  
			return NULL;  
		}  
		// Read in length of extra bytes.  
		WORD cbExtraBytes = 0L;  
		if( mmioRead( hmmio, ( CHAR* )&cbExtraBytes, sizeof( WORD ) ) != sizeof( WORD ) )  
		{  
			return NULL;  
		}  

		const int formatSize = sizeof( WAVEFORMATEX ) + cbExtraBytes;  
		WAVEFORMATEX *wfx = ( WAVEFORMATEX* )new BYTE[ formatSize ];  
		if( NULL == wfx )  
		{  
			return NULL;  
		}  

		// Copy the bytes from the pcm structure to the waveformatex structure  
		memcpy( wfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );  
		wfx->cbSize = cbExtraBytes;  

		// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.  
		if( mmioRead( hmmio, ( CHAR* )( ( ( BYTE* )&( wfx->cbSize ) ) + sizeof( WORD ) ),  
			cbExtraBytes ) != cbExtraBytes )  
		{  
			SAFE_DELETE_ARRAY( wfx );
			return NULL;  
		}  

		// Ascend the input file out of the 'fmt ' chunk.  
		if( 0 != mmioAscend( hmmio, &riff, 0 ) )  
		{  
			SAFE_DELETE_ARRAY( wfx );  
			return NULL;  
		}  

		// Search the input file for for the 'dpds' chunk.  
		riff.ckid = mmioFOURCC( 'd', 'p', 'd', 's' );  
		if( 0 != mmioDescend( hmmio, &riff, NULL, MMIO_FINDCHUNK ) )  
		{  
			SAFE_DELETE_ARRAY( wfx );  
			return NULL;  
		}  

		extraInfo->PacketCount = riff.cksize / sizeof( UINT32 );  
		extraInfo->pDecodedPacketCumulativeBytes = new UINT32[ riff.cksize ];  
		if( mmioRead( hmmio, ( HPSTR )extraInfo->pDecodedPacketCumulativeBytes, riff.cksize ) != ( LONG ) riff.cksize )  
		{  
			SAFE_DELETE_ARRAY( wfx );  
			return NULL;  
		}     
		// Ascend the input file out of the 'fmt ' chunk.  
		if( 0 != mmioAscend( hmmio, &riff, 0 ) )  
		{  
			SAFE_DELETE_ARRAY( wfx );  
			return NULL;  
		}  

		riff.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );  
		if( 0 != mmioDescend( hmmio, &riff, NULL, MMIO_FINDCHUNK ) )  
		{  
			SAFE_DELETE_ARRAY( wfx );  
			return NULL;  
		}  

		buffer->size = riff.cksize;  
		buffer->data = new BYTE[ riff.cksize ];
		if( mmioRead( hmmio, ( HPSTR )buffer->data, riff.cksize ) != ( LONG ) riff.cksize )  
		{  
			SAFE_DELETE_ARRAY( wfx );  
			SAFE_DELETE_ARRAY( buffer->data );  
			return NULL;  
		}     
		return wfx;  
	}  

	CMonadWaveBuffer * ReadAudioFile( const BYTE * pInBuffer, DWORD p_dwInSize, CWaveFormatExArray & pFormats )
	{  
		MMIOINFO mmioInfo;
		ZeroMemory( &mmioInfo, sizeof( mmioInfo ) );
		mmioInfo.fccIOProc = FOURCC_MEM;
		mmioInfo.cchBuffer = p_dwInSize;
		mmioInfo.pchBuffer = ( HPSTR ) pInBuffer;

		HMMIO hmmio = mmioOpen( NULL, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );

		MMCKINFO riff;  

		if( ( 0 != mmioDescend( hmmio, &riff, NULL, 0 ) ) )  
		{  
			mmioClose( hmmio, 0 );  
			THROW_EXC( MonadException::AudioFailed, E_FAIL, L"Parse Audio" );  
		}  

		// Check to make sure this is a valid wave file  
		if( riff.ckid != FOURCC_RIFF )   
		{  
			mmioClose( hmmio, 0 );  
		}  

		WAVEFORMATEX *wfx = NULL;  

		DataBuffer buffer;
		CWMABuffer pextraInfo;

		switch( riff.fccType )  
		{  
		/*case  mmioFOURCC( 'W', 'A', 'V', 'E' ):  
			{
				wfx = ReadWave( &buffer, hmmio );  
				break;  
			}*/

		case mmioFOURCC( 'X', 'W', 'M', 'A' ):  
			{
				pextraInfo.Attach( new MONAD_XAUDIO2_BUFFER_WMA() );
				wfx = ReadXWMA( &buffer, pextraInfo, hmmio );  
				break;  
			}  
		default:
			{
				THROW_EXC( MonadException::AudioFailed, E_FAIL, L"Invalid File Format" );
			}
		}  
		mmioClose( hmmio, 0 );  
		THROW_EXC_IFZ( wfx, MonadException::AudioFailed, L"Voice error" );

		return new CMonadWaveBuffer( wfx, buffer.size, buffer.data, pextraInfo, pFormats );
	}  
}