#include "DXUT.h"
#include "resource.h"
#include "MonadDXUpdate.h"
#include "MonadResource.h"
#include "OldPolishClockSetup3.0Dlg.h"

typedef INT ( WINAPI* LPDIRECTXSETUPSETCALLBACK )( DSETUP_CALLBACK Callback );
typedef INT ( WINAPI * LPDIRECTXSETUP )( HWND hWnd, LPWSTR lpszRootPath, DWORD dwFlags );

// Module and function pointers
HMODULE                      s_hModDSetup = NULL;
static LPDIRECTXSETUPSETCALLBACK    s_DirectXSetupSetCallback = NULL;
static LPDIRECTXSETUP               s_DirectXSetup = NULL;

//-----------------------------------------------------------------------------
// Name: DirectXSetupCallbackFunction()
// Desc: Handle each reason for why the callback was called, filtering each
//       message by what the current state of g_fStatus is.
//-----------------------------------------------------------------------------
DWORD WINAPI DirectXSetupCallbackFunction( DWORD dwReason, DWORD dwMsgType,
										  LPWSTR strMessage, LPWSTR strName,
										  VOID* pInfo )
{
	strName;
	dwMsgType;
	pInfo;

	switch( dwReason )
	{
	case DSETUP_CB_MSG_INTERNAL_ERROR:
		{
			const static std::wstring res( strMessage );
			SetErrorCommentExternal( res );
			return MB_OK;
		}

	default:
		return IDOK;
	}
}


int DirectXInstall()
{
	s_hModDSetup = LoadLibrary( gTmp->GetFullPath( L"dsetup.dll" ).c_str() );
	if( NULL == s_hModDSetup )
	{
		clog << "Library not available." << endl;
		return EXIT_FAILURE;
	}
	s_DirectXSetupSetCallback = ( LPDIRECTXSETUPSETCALLBACK )GetProcAddress( s_hModDSetup, "DirectXSetupSetCallback" );
	if( NULL == s_DirectXSetupSetCallback )
	{
		clog << "Callback not available." << endl;
		return EXIT_FAILURE;
	}
	s_DirectXSetup = (LPDIRECTXSETUP)GetProcAddress( s_hModDSetup, "DirectXSetupW" );
	if( NULL == s_DirectXSetup )
	{
		clog << "Direct Setup not available." << endl;
		return EXIT_FAILURE;
	}

	// Set up the callback function before calling DirectXSetup.
	s_DirectXSetupSetCallback( ( DSETUP_CALLBACK ) DirectXSetupCallbackFunction );

	return s_DirectXSetup( 0, &( ( std::wstring ) *gTmp )[ 0 ], DSETUP_DIRECTX );
}


void MonadReboot( DWORD dwReason )
{
	HANDLE hProcess( GetCurrentProcess() );
	HANDLE hToken( NULL );
	if( OpenProcessToken( hProcess, TOKEN_ALL_ACCESS, &hToken ) )
	{
		LUID luidShutDown;
		if( LookupPrivilegeValue( NULL, SE_SHUTDOWN_NAME, &luidShutDown ) )
		{
			TOKEN_PRIVILEGES Privilege;
			Privilege.PrivilegeCount = 1;
			Privilege.Privileges[0].Luid = luidShutDown;
			Privilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			AdjustTokenPrivileges( hToken, FALSE, &Privilege, 0, NULL, NULL );
		}
		CloseHandle( hToken );
	}

	InitiateSystemShutdownEx( NULL,
		NULL,
		0,
		FALSE,
		TRUE,
		dwReason );
}