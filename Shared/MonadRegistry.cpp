#include "DXUT.h"
#include "MonadRegistry.h"

CRegistry::CRegistry( HKEY phKey, const WCHAR * plpSubkey, DWORD pdwOptions )
: m_hKey( NULL )
{
	RegCreateKeyExW(
		phKey,
		plpSubkey,
		0,
		L"",
		0,
		pdwOptions,
		NULL,
		&m_hKey,
		NULL );
}
bool CRegistry::SetString( const WCHAR * pRegName, const std::wstring & pValue )
{
	return ERROR_SUCCESS == RegSetValueExW( m_hKey, pRegName, 0, REG_SZ, 
		reinterpret_cast< const BYTE* > ( pValue.c_str() ),
		( DWORD ) ( ( pValue.length() + 1 ) * sizeof( WCHAR ) ) );
}
bool CRegistry::SetDWORD( const WCHAR * pRegName, DWORD pValue )
{
	return ERROR_SUCCESS == RegSetValueExW( m_hKey, pRegName, 0, REG_DWORD, 
		reinterpret_cast< const BYTE* > ( &pValue ),
		( DWORD ) sizeof(DWORD) );
}
CRegistry::~CRegistry()
{
	if( m_hKey )
		RegCloseKey( m_hKey );
}