#include "DXUT.h"
#include "Path.h"
#include "OPC-ConfigDialog.h"
#include "MonadRegistry.h"

static LPCWSTR MainFolderName( L"MainFolder" );

//-----------------------------------------------------------------------------
// Name: DXUtil_ReadStringRegKey()
// Desc: Helper function to read a registry key string
//-----------------------------------------------------------------------------
HRESULT DXUtil_ReadStringRegKey( HKEY hKey, LPCWSTR strRegName, TCHAR* strValue, 
                                 DWORD dwLength )
{
    DWORD dwType;

    return RegQueryValueEx( hKey, strRegName, 0, &dwType, (BYTE*)strValue, &dwLength );
}

static const std::wstring GetRootPathGeneric()
{
	WCHAR MainFolder [MAXSHORT] = L"";

	struct CReadProfileRegistry : public CRegistry
	{
		CReadProfileRegistry( WCHAR * pMainFolder )
			: CRegistry( HKEY_LOCAL_MACHINE, Profile, KEY_READ ),
			m_MainFolder( pMainFolder )
		{
			DXUtil_ReadStringRegKey( m_hKey, MainFolderName, m_MainFolder, MAXSHORT );
		}
		WCHAR * m_MainFolder;
	} mReadReg( MainFolder );
	if( 0 == MainFolder[ 0 ] )
		THROW_EXC( MonadException::NotInit, E_FAIL, L"Path" );

	return MainFolder;
}

const std::wstring & GetRootPath()
{
	static const std::wstring g_s_szRoot( GetRootPathGeneric() );

	return g_s_szRoot;
}