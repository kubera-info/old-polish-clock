#include "DXUT.h"
#include <string>
#include "ByteArray.h"
#include "System.h"

std::wstring GetApplicationVersion()
{
	WCHAR lpszMe [MAXWORD];
	if( FAILED ( GetModuleFileName(
		NULL,
		lpszMe,
		MAXWORD
		) ) )
		return L"";
#define SIZE_OF_ABOUT (128)
	WCHAR About [SIZE_OF_ABOUT] = L"\0";
	const WCHAR * AboutFormat = L"%d.%d.%d.%d.%s";
	DWORD dwDummy; 
	DWORD dwFVISize = GetFileVersionInfoSize( lpszMe , &dwDummy ); 
	CAutoSimpleVectorBytePtr lpVersionInfo( new BYTE[ dwFVISize ] );
	if( NULL == lpVersionInfo )
		return L"";
	GetFileVersionInfo( lpszMe , 0 , dwFVISize , lpVersionInfo ); 
	UINT uLen; 
	VS_FIXEDFILEINFO *lpFfi; 
	VerQueryValue( lpVersionInfo, L"\\" , (LPVOID *)&lpFfi , &uLen );
	const DWORD dwFileFlags = lpFfi->dwFileFlags;
	const DWORD dwLeftMost = HIWORD(lpFfi->dwFileVersionMS); 
	const DWORD dwSecondLeft = LOWORD(lpFfi->dwFileVersionMS); 
	const DWORD dwSecondRight = HIWORD(lpFfi->dwFileVersionLS); 
	const DWORD dwRightMost = LOWORD(lpFfi->dwFileVersionLS); 

#ifdef _M_X64
#define FileArch L"64bit"
#else
#define FileArch L"32bit"
#endif

#ifdef DEBUG
#define FileProd FileArch L".debug"
#define FileBeta FileArch L".beta-debug"
#else
#define FileProd FileArch
#define FileBeta FileArch L".beta"
#endif

	if( -1 == swprintf_s(
		About, 
		SIZE_OF_ABOUT, 
		AboutFormat, 
		dwLeftMost,
		dwSecondLeft,
		dwSecondRight,
		dwRightMost,
		dwFileFlags & 2 ? FileBeta : FileProd
		) )
		return L"";
	return About;
}

bool CMonadWindowsVersion::IsWindows_XP_SP2() const throw()
{
	static const bool bXP( TestVer( 5, 1, 2, 0 ) );
	return bXP;
}

bool CMonadWindowsVersion::IsWindows_Vista_SP2() const throw()
{
	static const bool bVista( TestVer( 6, 0, 2, 0 ) );
	return bVista;
}

bool CMonadWindowsVersion::IsWindows_7() const throw()
{
	static const bool b7( TestVer( 6, 1, 0, 0 ) );
	return b7;
}

bool CMonadWindowsVersion::IsWindows_8() const throw()
{
	static const bool b8( TestVer( 6, 2, 0, 0 ) );
	return b8;
}

bool CMonadWindowsVersion::TestVer(
								   const DWORD dwMajorVersion,
								   const DWORD dwMinorVersion,
								   const WORD wServicePackMajor,
								   const WORD wServicePackMinor ) const throw()
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;
	const BYTE op=VER_GREATER_EQUAL;

	// Initialize the OSVERSIONINFOEX structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osvi.dwMajorVersion = dwMajorVersion;
	osvi.dwMinorVersion = dwMinorVersion;
	osvi.wServicePackMajor = wServicePackMajor;
	osvi.wServicePackMinor = wServicePackMinor;

	// Initialize the condition mask.

	VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, op );
	VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, op );
	VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMAJOR, op );
	VER_SET_CONDITION( dwlConditionMask, VER_SERVICEPACKMINOR, op );

	// Perform the test.

	return TRUE == VerifyVersionInfo(
		&osvi, 
		VER_MAJORVERSION | VER_MINORVERSION | 
		VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR,
		dwlConditionMask);
}