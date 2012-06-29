#include "DXUT.h"
#include <boost/algorithm/string/replace.hpp>
#include "I18N.h"
#include "MonadStream.h"
#include <delayimp.h>
#include "MonadResource.h"
#include "MonadThreadBuilder.h"
#include "MonadRegistry.h"
#include "MonadSetupReg.h"

extern HMODULE                      s_hModDSetup;
CAutoPtr<const CTemporaryFolder> gTmp;
std::wstring FileNameSuffix()
{
	struct CIntoStream : public std::ostringstream
	{
		CIntoStream()
		{
			imbue( std::locale::classic() );
		}
	} mStrm;

	mStrm << CTime::GetCurrentTime();

	return MonadI18N::AnsiToUnicode( boost::replace_all_copy( mStrm.str(), ":", "." ) );
}
const std::wstring & CTemporaryFolder::FileNameSuffix()
{
	static const std::wstring Suff( ::FileNameSuffix() );
	return Suff;
}
const std::wstring CTemporaryFolder::GetUserTemporaryFolder()
{
	TCHAR szDir[MAX_PATH];
	if( GetTempPath( sizeof( szDir )/sizeof( TCHAR ), szDir ) == 0 )
		return L"";
	else
		return szDir;
}

std::wstring CTemporaryFolder::CreateTmpFolder()
{
	std::wstring szFileName( GetUserTemporaryFolder() );
	szFileName += + L"Kubera";
	szFileName += FileNameSuffix();
	szFileName += L'\\';

	if( !CreateDirectory( szFileName.c_str(), NULL ) )
	{
		clog << "Can not create a temporary folder." << endl;
		return L"";
	}

	return szFileName;
}

CTemporaryFolder::CTemporaryFolder()
: m_szTmpFolder( CreateTmpFolder() ), bPurged( m_szTmpFolder.empty() )
{
	if( m_szTmpFolder.empty() )
		ExitProcess( EXIT_FAILURE );
}
CTemporaryFolder::~CTemporaryFolder()
{
	Purge();
}

void CTemporaryFolder::Purge() const
{
	if( !bPurged )
	{
		if( NULL != s_hModDSetup )
		{
			FreeLibrary( s_hModDSetup );
			s_hModDSetup = NULL;
		}

		( ERROR_SUCCESS != ExecApp( GetFullPath( L"MonadScriptDelParent.vbe" ), L"/Monad(a).info" ) );

		bPurged = true;	
	}
}
CTemporaryFolder::operator const std::wstring & () const throw()
{
	return m_szTmpFolder;
}
std::wstring CTemporaryFolder::GetFullPath( const std::wstring & pFilename ) const
{
	return m_szTmpFolder + pFilename;
}

LPCVOID LoadMonadResource(CONST INT pResID, DWORD & pLen)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	HRSRC hRsrc = FindResource(
		hInst,
		MAKEINTRESOURCE(pResID),
		RT_RCDATA);
	if(NULL == hRsrc)
	{
		clog << "No hRsrc." << endl;
		return NULL;
	}
	DWORD len = SizeofResource(hInst, hRsrc);
	if(len == 0)
	{
		clog << "No len." << endl;
		return NULL;
	}
	LPVOID lpLoad = LoadResource(hInst, hRsrc);
	if(NULL == lpLoad)
	{
		clog << "No load." << endl;
		return NULL;
	}
	pLen = len;
	return LockResource(lpLoad);
}

HRESULT SaveBlob( LPCWSTR pPath, LPCVOID pBuffer, DWORD pLen )
{
	if( !g_bCancelInstall )
	{
		HRESULT hr;
		CAtlFile mFile;
		DWORD dummy;

		if( NULL == pBuffer )
			return E_FAIL;
		hr = mFile.Create( pPath, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS );
		if( FAILED ( hr ) )
		{
			clog << "Failed in creating a file " << pPath << ", HRESULT(" << hr << ")." << endl;
			return hr;
		}

		hr = mFile.Write( pBuffer, pLen, &dummy );
		if( FAILED ( hr ) )
		{
			clog << "Failed in storing a file " << pPath << ", HRESULT(" << hr << ")." << endl;
			return hr;
		}

		mFile.Close();
	}

	return S_OK;
}

CBlob::CBlob( CONST INT pResID, LPCWSTR pFilename ) throw()
: hRarProcess( NULL ), szPath( gTmp->GetFullPath( pFilename ) )
{
	LPCVOID mBuffer( NULL );
	DWORD m_dwLen ( 0 );

	if( !g_bCancelInstall )
	{
		mBuffer = LoadMonadResource( pResID, m_dwLen );
		if( NULL == mBuffer )
			clog << "Buffer for " << szPath << " failed." << endl;
		else if( SUCCEEDED( SaveBlob( szPath.c_str(), mBuffer, m_dwLen ) ) )
		{
			std::wstring RarParams( L" -s -d\"" );
			RarParams += *gTmp;
			RarParams += L"\"";
			hRarProcess = ExecAppHandle( szPath, RarParams );
			if( NULL == hRarProcess )
				clog << "No handle for " << szPath << '.' << endl;
		}
		else
			clog << "Saving blob " << szPath << " failed." << endl;
	}
}
void CBlob::Validate() const
{
	if( !g_bCancelInstall )
	{
		if( hRarProcess )
		{
			DWORD res ( ExitApp( hRarProcess ) );
			DeleteFile( szPath.c_str() );
			if( 1 < res ) // WinRar: 0, 1 - OK
			{
				throw res;
				clog << "Extracting from WR failed with STATUS(" << res << ")." << endl;
			}
		}
		else
		{
			clog << "No WR created." << endl;
			throw EXIT_FAILURE;
		}
	}
}

HANDLE ExecAppHandle( const std::wstring & pApp, const std::wstring & pParams )
{
	SHELLEXECUTEINFO ShEx;
	ShEx.cbSize = sizeof( ShEx );
	ShEx.dwHotKey = NULL;
	ShEx.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShEx.hIcon = NULL;
	ShEx.hInstApp = NULL;
	ShEx.hkeyClass = NULL;
	ShEx.hMonitor = NULL;
	ShEx.hProcess = NULL;
	ShEx.hwnd = gHWND;
	ShEx.lpClass = NULL;
	ShEx.lpDirectory = ( ( std::wstring ) *gTmp ).c_str();
	ShEx.lpFile = pApp.c_str();
	ShEx.lpIDList = NULL;
	ShEx.lpParameters = pParams.c_str();
	ShEx.lpVerb = NULL;
	ShEx.nShow = 0;

	if( ShellExecuteEx( &ShEx ) )
		return ShEx.hProcess;
	else
	{
		clog << "Shell failed for " << pApp << '.' << endl;
		return NULL;
	}
}
DWORD ExitApp( HANDLE pAppHadnle, const std::wstring & pApp )
{
	if( NULL != pAppHadnle )
	{
		WaitForSingleObject( pAppHadnle, INFINITE );
		DWORD ExitCode;
		if( GetExitCodeProcess(
			pAppHadnle,
			&ExitCode ) )
		{
			if( ERROR_SUCCESS != ExitCode )
			{
				clog << "Application call error: ";
				clog << "STATUS(" << ExitCode << "). for: " << pApp << endl;
			}
			return ExitCode;
		}
		else 
		{
			clog << "No shell for: " << pApp << endl;
			return EXIT_FAILURE;
		}
	}
	else
	{
		clog << "No application handle " << pApp << '.' << endl;
		return EXIT_FAILURE;
	}
}
DWORD ExecApp( const std::wstring & pApp, const std::wstring & pParams )
{
	return ExitApp( ExecAppHandle( pApp, pParams ), pApp );
}