// MonadSet.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Shlwapi.h>
#include <shellapi.h>
#include <boost/algorithm/string.hpp>
#include "MonadSet.h"
#include "MonadRegistry.h"
#include "MonadFile.h"

inline void SET_EMPTY_STRING( WCHAR * str ) { ( str ) [0] = L'\0'; }
static LPCWSTR Profile( L"SOFTWARE\\www.kubera.info\\Screen Saver\\Clock3D\\Config-3.0" );
static LPCWSTR MainPath( L"SOFTWARE\\www.kubera.info\\Screen Saver\\Clock3D" );
static LPCWSTR MainPath2( L"SOFTWARE\\www.kubera.info\\Screen Saver" );
static LPCWSTR MainPath3( L"SOFTWARE\\www.kubera.info\\" );
static LPCWSTR MainFolder( L"MainFolder" );
static LPCWSTR Desktop( L"Control Panel\\Desktop" );

static const std::wstring g_szSystemFolder( MonadFile::GetSystemFolder() );
static const std::wstring g_szBinary( g_szSystemFolder + L"\\MainXIX.scr" );

static LPCWSTR ScrnsaveExe( L"SCRNSAVE.EXE" );

bool DXUtil_ReadStringRegKey( HKEY hKey, LPCWSTR strRegName, LPWSTR strValue, 
                                 DWORD dwLength )
{
    return RegQueryValueExW( hKey, strRegName, 0, 0, (BYTE*)strValue, &dwLength ) == ERROR_SUCCESS;
}

void CleanRegistry( HKEY hRootOfOp )
{
	SHDeleteKey( hRootOfOp, Profile );
	SHDeleteEmptyKey( hRootOfOp, MainPath );
	SHDeleteEmptyKey( hRootOfOp, MainPath2 );
	SHDeleteEmptyKey( hRootOfOp, MainPath3 );
}
static const std::wstring GetRootPathGeneric()
{
	WCHAR MainFolder [MAXSHORT]; // Not initialized, but it is no problem here

	if( 0 == GetModuleFileName( g_hModule, MainFolder, MAXSHORT ) )
		return L"";
	WCHAR * CutMainDir = wcsrchr( MainFolder, L'\\' );
	if( 0 == CutMainDir )
		return L"";
	SET_EMPTY_STRING( CutMainDir );
	CutMainDir = wcsrchr( MainFolder, L'\\' );
	if( 0 == CutMainDir )
		return L"";
	CutMainDir++;
	SET_EMPTY_STRING( CutMainDir );

	return MainFolder;
}

MONADSET_API UINT AddClock( /*MSIHANDLE hModule*/ )
{
	BOOL bOK( EXIT_FAILURE );
	{
		struct CWriteProfileRegistry : private CRegistry
		{
			CWriteProfileRegistry( BOOL * p_bOK )
				: CRegistry( HKEY_LOCAL_MACHINE, Profile, KEY_WRITE )
			{
				*p_bOK = SetString( MainFolder, GetRootPathGeneric() ) ? ERROR_SUCCESS : EXIT_FAILURE;
			}
		} mWriteReg( &bOK );
	}

	return bOK;
}
MONADSET_API UINT RemoveClock( /*MSIHANDLE hModule*/ )
{
	CleanRegistry( HKEY_LOCAL_MACHINE );
	struct CWriteProfileRegistry : private CRegistry
	{
		CWriteProfileRegistry()
			: CRegistry( HKEY_USERS, L"", KEY_ENUMERATE_SUB_KEYS )
		{
			const DWORD MySize( 2048 );
			WCHAR Path [MySize];
			DWORD dwRegIdx( 0 );
			long res;
			do
			{
				res = RegEnumKey(
					m_hKey,
					dwRegIdx,
					Path,
					MySize
					);

				{
					struct CDelProfileRegistry : private CRegistry
					{
						CDelProfileRegistry( LPCWSTR p_szPathToUser )
							: CRegistry( HKEY_USERS, p_szPathToUser, KEY_WRITE )
						{
							CleanRegistry( m_hKey );
						}					
					} mDelProfile( Path );
				}

				{
					struct CExeRegistry : private CRegistry
					{
						CExeRegistry( const std::wstring & p_szPathToUser )
							: CRegistry( HKEY_USERS, ( p_szPathToUser + L"\\" + Desktop ).c_str(), KEY_WRITE | KEY_READ )
						{
							WCHAR ReadMe [MAXSHORT] = L"";
							if( DXUtil_ReadStringRegKey( m_hKey, ScrnsaveExe, ReadMe, MAXSHORT ) )
								if( boost::iequals( g_szBinary, ReadMe ) )
									SetString( ScrnsaveExe, L"" ) ? ERROR_SUCCESS : EXIT_FAILURE;
						}
					} mBinaryReg( Path );
					mBinaryReg;
				}
				dwRegIdx++;

			}
			while( ERROR_SUCCESS == res );
		}
	} EnumProfiles;
	return ERROR_SUCCESS;
}