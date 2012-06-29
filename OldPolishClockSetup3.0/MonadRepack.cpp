#include "DXUT.h"
#include <boost/algorithm/string.hpp>
#include "MonadRepack.h"
#include "MonadCipher.h"
#include "MonadHash.h"
#include "bzlib.h"
#include "MonadFileTimestamp.h"
#include "MonadResource.h"

static const std::wstring Unpacking( L"Error while unpacking resource file." );
static const std::wstring Saving( L"Error while saving resource file." );

#define UNPACK_AND_RETURN { clog << Unpacking << ", " << __LINE__ << endl; return hr; }
#define SAVE_AND_RETURN { clog << Saving << ", " << __LINE__ << endl; return hr; }

HRESULT Repack( const std::wstring & pSrcPath, const std::wstring & pFilename, const std::wstring pExtension, DWORD pSrcFileSize )
{
	HRESULT hr = S_OK;

	if( g_bCancelInstall )
		return hr;

	DWORD mFileSize = pSrcFileSize;
	if( sizeof( ULONG ) >= mFileSize )
	{
		hr = E_FAIL;
		UNPACK_AND_RETURN;
	}

	CAutoVectorBytePtr mFileBuffer;
	CAutoVectorBytePtr mBuffer;
	CAtlFile mFile;
	V( mFile.Create( ( pSrcPath + pFilename + pExtension ).c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING ) );
	if( FAILED( hr ) )
		UNPACK_AND_RETURN;

	mFileBuffer.reset( new BYTE [ mFileSize ] );
	V( mFile.Read( mFileBuffer.get(), mFileSize ) );
	if( FAILED ( hr ) )
	{
		UNPACK_AND_RETURN;
	}
	else
		mFileSize -= sizeof( ULONG );
	//{
	//	hr = E_FAIL;
	//	UNPACK_AND_RETURN;
	//}
	mFile.Close();

	std::wstring szKey( pFilename );
	std::wstring szExt( pExtension );

	V( Decrypt( 
		reinterpret_cast<LPCBYTE> ( szKey.c_str() ),
		szKey.length() * sizeof( WCHAR ), 
		mFileBuffer.get(), 
		mFileSize ) );

	if( FAILED( hr ) )
		UNPACK_AND_RETURN;

	ULONG & CheckSum = *reinterpret_cast< ULONG * > ( mFileBuffer.get() + mFileSize );
	const ULONG CheckSumFromFile = HashFromFileHolder( szKey, szExt, reinterpret_cast< char * > ( mFileBuffer.get() ), mFileSize );
	if( CheckSumFromFile != CheckSum )
	{
		hr = E_FAIL;
		UNPACK_AND_RETURN;
	}

	mFileSize -= sizeof( ULONG );
	unsigned int SizeToUncompress = * ( ULONG * ) mFileBuffer.get();
	mBuffer.reset( new BYTE [ SizeToUncompress + sizeof( ULONG ) ] );
	if( FAILED( hr ) )
		return false;
	assert( sizeof( ULONG ) == sizeof( unsigned int ) );

	const int bzip2Statis = BZ2_bzBuffToBuffDecompress(
		reinterpret_cast< char * > ( mBuffer.get() ),
		&SizeToUncompress,
		reinterpret_cast< char * > ( mFileBuffer.get() + sizeof( ULONG ) ),
		mFileSize,
		0, 
		0);
	if( BZ_OK != bzip2Statis )
	{
		hr = E_FAIL;
		UNPACK_AND_RETURN;
	}
	mFileSize = SizeToUncompress;
	ULONG & CheckSumOut = *reinterpret_cast< ULONG * > ( mBuffer.get() + mFileSize );
	szExt = szExt.substr( 0, szExt.length() - sizeof( ULONG ) );
	CheckSumOut = HashFromFileHolder( szKey, szExt, reinterpret_cast< char * > ( mBuffer.get() ), mFileSize );
	V( Decrypt( 
		reinterpret_cast<LPCBYTE> ( szKey.c_str() ),
		szKey.length() * sizeof( WCHAR ), 
		mBuffer.get(), 
		mFileSize ) );
	if( FAILED( hr ) )
	{
		SAVE_AND_RETURN;
	}
	DWORD dummy;
	std::wstring NewFile( pSrcPath + pFilename + szExt );
	V( mFile.Create( NewFile.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS ) );
	if( FAILED( hr ) )
	{
		SAVE_AND_RETURN;
	}

	V( mFile.Write( mBuffer.get(), mFileSize + sizeof( ULONG ), &dummy ) );
	if( FAILED( hr ) )
	{
		SAVE_AND_RETURN;
	}

	MonadSetFileTime( mFile );

	mFile.Close();

	DeleteFile( ( pSrcPath + pFilename + pExtension ).c_str() );

	return true;
}

CRepacker::CRepacker( const std::wstring & pSubPath )
: mSubPath( gTmp->GetFullPath( pSubPath ) )
{
}

unsigned CRepacker::Routine()
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA FindFileData;

	if( INVALID_HANDLE_VALUE != ( hFind = FindFirstFile( ( mSubPath + L"*.monad-*" ).c_str(), &FindFileData) ) )
	{
		unsigned res = ProcessFile( FindFileData );
		if( ERROR_SUCCESS != res )
			return res;
		// List all the other files in the directory.
		while ( FindNextFile( hFind, &FindFileData ) != 0 ) 
		{
			ProcessFile( FindFileData );
			if( ERROR_SUCCESS != res )
				return res;
		}
	}

	DWORD dwError = GetLastError();
	FindClose( hFind );
	if( dwError != ERROR_NO_MORE_FILES )
		return dwError;

	return 0;
}

unsigned CRepacker::ProcessFile( const WIN32_FIND_DATA & pFindFileData )
{
	if( 0 == ( pFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
	{
		std::wstring Ext( PathFindExtension( pFindFileData.cFileName ) );
		boost::algorithm::to_lower( Ext );
		std::wstring FileName( pFindFileData.cFileName );
		if( Ext.substr( Ext.length() - 3 ) == L"pak" )
		{
			FileName = FileName.substr( 0, FileName.length() - Ext.length() );

			const HRESULT hr( Repack( mSubPath, FileName, Ext, pFindFileData.nFileSizeLow ) );
			if( FAILED( hr ))
			{
				clog << "Pack failed, HRESULT(" << hr << L")." << endl;
				return HRESULT_CODE( hr );
			}
		}
		else
			MonadSetFileTime( mSubPath + pFindFileData.cFileName );
		return ERROR_SUCCESS;
	}
	else
		return ERROR_SUCCESS;
}
