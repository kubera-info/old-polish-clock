#include "DXUT.h"
#include <atlfile.h>
#include <boost/algorithm/string.hpp>
#include "define.h"
#include "FileManager.h"
#include "Singleton.h"
#include "MonadException.h"
#include "MonadAudio.h"
#include "MonadCipher.h"
#include "MonadHash.h"
#include "MonadXML.h"
#include "MonadStreaming.h"
#include "Path.h"

const std::wstring szExtSnd = L".monad-snd";
const std::wstring szExtStrm = L".monad-strm";
const std::wstring szExtFx = L".monad-fx";
const std::wstring szExtMesh = L".monad-mesh";
const std::wstring szExtDb = L".monad-db";
const std::wstring szExtTex = L".monad-tex";

#define FM_MAX_FILE_SIZE ( 1024 * 1024 * 16 )

void CPaths::add_path( const std::wstring & pFolder )
{
	push_back( GetRootPath() + L"Res\\" + pFolder + L"\\" );
}

/*DWORD GetResFileSize( const std::wstring & pPath )
{
	CAtlFile fSizeFile;

	ULONGLONG nLen;
	THROW_EXC_IFFAILED( MonadException::IOError, fSizeFile.Create( pPath.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING ), L"fsize" );
	THROW_EXC_IFFAILED( MonadException::IOError, fSizeFile.GetSize( nLen ), L"fsize" );
	if( nLen > FM_MAX_FILE_SIZE )
		THROW_EXC( MonadException::OutOfMemory, E_FAIL, L"File too large" );

	return (DWORD) nLen;
}*/

CAutoPtr<CFileSearcher> g_pFileManager;

CFileHolder::CFileHolder( const std::wstring & pFolder, const std::wstring & pFilename, const DWORD pFileSize ) 
: 
mPath( _wcsdup( ( pFolder + pFilename ).c_str() ) ),
mFileSize( pFileSize ),
mBuffer( NULL )
{
	if( NULL == mPath )
		throw std::bad_alloc( "path" );
}
/*CFileHolder::CFileHolder( const std::wstring & pFolder, const std::wstring & pFilename ) 
: 
mPath( _wcsdup( ( pFolder + pFilename ).c_str() ) ),
mFileSize( GetResFileSize( GetPath() ) ),
mBuffer( NULL )
{
	if( NULL == mPath )
		throw std::bad_alloc( "Path" );
}*/
CFileHolder::~CFileHolder()
{
	ReleaseFileHolder();
	if( mPath )
		free( const_cast < WCHAR * > ( mPath ) );
}
const std::wstring CFileHolder::GetExtension() const
{
	std::wstring Ext( PathFindExtension( const_cast< LPCWSTR > ( mPath ) ) );
	boost::algorithm::to_lower( Ext );
	return Ext;
}
const DWORD CFileHolder::GetSize() const
{
	return mFileSize;
}
BYTE * CFileHolder::GetBuffer() const
{
	return const_cast<BYTE *> ( mBuffer );
}
void CFileHolder::Run() throw(...)
{
	if( MonadException::ExceptionInProgress() )
		return;

	if( sizeof( ULONG ) > mFileSize )
		THROW_EXC( MonadException::IOError, E_FAIL, L"File is corrupt" );

	CAtlFile mFile;
	THROW_EXC_IFFAILED( MonadException::IOError, mFile.Create( const_cast< LPCWCHAR > ( mPath ), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING), L"fopen" );

	CAutoVectorPtr<BYTE> mTmpBuffer( new byte[ mFileSize ] );
	THROW_EXC_IFFAILED( MonadException::IOError, mFile.Read( mTmpBuffer, mFileSize ), L"fread" );
	mFile.Close();
	mFileSize -= sizeof( ULONG );

	const std::wstring szKey( GetFilename() );
	const std::wstring szExt( GetExtension() );

	THROW_EXC_IFFAILED(
		MonadException::IOError, 
		Decrypt( 
		reinterpret_cast<LPCBYTE> ( szKey.c_str() ),
		( DWORD ) szKey.length() * sizeof( WCHAR ), 
		reinterpret_cast<LPBYTE> ( mTmpBuffer.m_p ),
		mFileSize ),
		L"Unwind" );

	ULONG & CheckSum = *reinterpret_cast< ULONG * > ( mTmpBuffer + mFileSize );
	const ULONG CheckSumFromFile = HashFromFileHolder( szKey, szExt, reinterpret_cast< char * > ( mTmpBuffer.m_p ), mFileSize );
	if( CheckSumFromFile != CheckSum )
		THROW_EXC( MonadException::FileCorrupt, E_FAIL, L"Validation" );
	CheckSum = 0UL; // Finishing NULL
	mBuffer = mTmpBuffer.Detach();


	if( szExt == szExtSnd || szExt == szExtStrm )
	{
		MonadAudio::CreateVoice( szKey, const_cast< LPCBYTE > ( mBuffer ), mFileSize );
		ReleaseFileHolder();
		/*if( szExt == szExtStrm )
		{
			CWaveStreaming::PostStream( szKey );
		}*/
	}
	else if( szExt == szExtDb )
	{
		{
			struct CXMLArrayLoader ParseXML( ( const char * ) ( const_cast< LPCBYTE > ( mBuffer ) ) );
		}
		ReleaseFileHolder();
	}
}

void CFileHolder::ProcessQueue() throw(...)
{
	if( NULL != mBuffer && !MonadException::ExceptionInProgress() && bFactoryIsReady )
		Singleton->MonadFactory.OnCreateResource( *this );

	g_pFileManager->UnregisterTreasure();

	delete this;
}
const std::wstring CFileHolder::GetPath() const
{
	return const_cast< LPCWSTR > ( mPath );
}

const std::wstring CFileHolder::GetFilename() const
{
	std::wstring Filename( PathFindFileName( const_cast< LPCWSTR > ( mPath ) ) );
	PathRemoveExtension( &Filename[ 0 ] );
	boost::algorithm::to_lower( Filename );
	return Filename.c_str();
}

void CFileHolder::DetachFileHolder()
{
	mBuffer = NULL;
}

void CFileHolder::ReleaseFileHolder()
{
	SAFE_DELETE_ARRAY( mBuffer );
}

CFileSearcher::CFileSearcher()
: mCntOfFound( 0L )
{
}

CFileSearcher::~CFileSearcher()
{
	assert( IsFinished() );
}

void CFileSearcher::Run( const CStringArray & pSearchPaths ) throw(...)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA FindFileData;

	{
		BOOST_FOREACH( const std::wstring & tmpFolder, pSearchPaths )
		{
			if( INVALID_HANDLE_VALUE == ( hFind = FindFirstFile( ( tmpFolder + L"*.monad-*" ).c_str(), &FindFileData) ) )
			{
				THROW_EXC_ONERROR( MonadException::FileNotFound, GetLastError(), L"Searching resources folders error");
			}
			else
			{			
				AddOneFileToList( tmpFolder, FindFileData );
				// List all the other files in the directory.
				while ( FindNextFile(hFind, &FindFileData ) != 0 ) 
					AddOneFileToList( tmpFolder, FindFileData );
			}

			DWORD dwError = GetLastError();
			FindClose( hFind );
			if( ERROR_NO_MORE_FILES != dwError )
			{
				THROW_EXC_ONERROR( MonadException::FileNotFound, GetLastError(), L"cannot find files #2");
			}
		}
	}

	THROW_EXC_IFZ( mCntOfFound, MonadException::FileNotFound, L"Empty directory or does not exist");
}

void CFileSearcher::AddOneFileToList( const std::wstring & pFolder, const WIN32_FIND_DATA & pFindFileData )
{
	if( 0 == ( pFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
	{
		if( FM_MAX_FILE_SIZE < ( pFindFileData.nFileSizeLow | pFindFileData.nFileSizeHigh << 16 ) )
			THROW_EXC_ONERROR( MonadException::OutOfMemory, ERROR_FILE_TOO_LARGE, L"File too large");
		CFileHolder * tmpPlaceHolder = new CFileHolder( pFolder, pFindFileData.cFileName, pFindFileData.nFileSizeLow );
		mThreadPool.InsertTask( tmpPlaceHolder );
		RegisterTreasure();
	}
}

void CFileSearcher::RegisterTreasure() throw()
{
	/*
#ifdef _M_X64
	InterlockedIncrement64( &mCntOfFound );
#else
	InterlockedIncrement( &mCntOfFound );
#endif
	*/
	++ mCntOfFound;
}
void CFileSearcher::UnregisterTreasure() throw()
{
	-- mCntOfFound;
	// mMutexForApplied.Enter();
/*#ifdef _M_X64
	InterlockedDecrement64( &mCntOfFound );
#else
	InterlockedDecrement( &mCntOfFound );
#endif*/
	// mMutexForApplied.Leave();
}

bool CFileSearcher::IsFinished() /* volatile */ const throw()
{
	return 0L == mCntOfFound;
}