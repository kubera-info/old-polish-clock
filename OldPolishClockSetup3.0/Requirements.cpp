#include "DXUT.h"
#include <string>
#include "MonadLog.h"
#include "MonadResource.h"
#include "Requirements.h"
#include "MonadSetupReg.h"
#include "MonadFile.h"

struct CMonadDeploymentVersion gVersion;

inline bool Is64Bit() throw()
{
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

	LPFN_ISWOW64PROCESS fnIsWow64Process;

	BOOL bIsWow64 = FALSE;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

	if( NULL != fnIsWow64Process )
		if( !fnIsWow64Process( GetCurrentProcess(), &bIsWow64 ) )
			return false;
	return TRUE == bIsWow64;
}

bool CMonadDeploymentVersion::Is64Bit() const throw()
{
	static const bool bIs64Bit ( ::Is64Bit() );
	return bIs64Bit;
}

inline bool IsWindowsServer() throw()
{
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;
	const BYTE op=VER_EQUAL;

	// Initialize the OSVERSIONINFOEX structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	osvi.wProductType = VER_NT_WORKSTATION;

	// Initialize the condition mask.

	VER_SET_CONDITION( dwlConditionMask, VER_PRODUCT_TYPE, op );

	// Perform the test.

	return FALSE == VerifyVersionInfo(
		&osvi, 
		VER_PRODUCT_TYPE,
		dwlConditionMask);
}
bool CMonadDeploymentVersion::IsWindowsServer() const throw()
{
	static const bool bServer( ::IsWindowsServer() );
	return bServer;
}

/*bool CMonadDeploymentVersion::IsSSE2() const throw()
{
	return TRUE == IsProcessorFeaturePresent( PF_XMMI64_INSTRUCTIONS_AVAILABLE );
}*/

static const std::wstring GetClockInfo( LPCTSTR p_szProperty )
{
    const DWORD BUF_SIZE = 64;
	DWORD dwBufferSize = BUF_SIZE;
	WCHAR tmpBuffer[ BUF_SIZE ] = L"";
	MsiGetProductInfo( MonadAppGuid, p_szProperty, tmpBuffer, &dwBufferSize );
	return tmpBuffer;
}

bool CMonadDeploymentVersion::IsInstalled() const throw()
{
	return !InstalledMajor().empty();
}

const std::wstring & CMonadDeploymentVersion::InstalledMajor() const throw()
{
	const static std::wstring Ver( GetClockInfo( INSTALLPROPERTY_VERSIONMAJOR ) );

	return Ver;
}

const std::wstring & CMonadDeploymentVersion::InstalledMinor() const throw()
{
	const static std::wstring Ver( GetClockInfo( INSTALLPROPERTY_VERSIONMINOR ) );

	return Ver;
}

void LogFreeSpace( ostream &Out, const std::wstring & szPath, const std::wstring & pId )
{
	struct CSpaceTag : public CMonadTag
	{
		CSpaceTag( const std::wstring & pId, const std::wstring & szPath, ostream &pOutStream )
			: CMonadTag( L"free_space", L"id", pId, pOutStream )
		{
			unsigned __int64 FreeBytesAvailableToCaller, TotalNumberOfBytes, TotalNumberOfFreeBytes;
			if( TRUE == GetDiskFreeSpaceEx(
				szPath.c_str(),
				( PULARGE_INTEGER ) &FreeBytesAvailableToCaller,
				( PULARGE_INTEGER ) &TotalNumberOfBytes,
				( PULARGE_INTEGER ) &TotalNumberOfFreeBytes ) )
			{
				mOutStream << TotalNumberOfFreeBytes;
			}
			else
				mOutStream << "NaN";
		}
	} mSpace( pId, szPath, Out );
}

struct CStringTag : public CMonadTag
{
	CStringTag(
		const std::wstring & pTag, 
		const std::wstring & pId,
		const std::wstring & p_szString,
		ostream & pOutStream )
		: CMonadTag( pTag, L"id", pId, pOutStream )
	{
		mOutStream << p_szString;
	}
};
struct CBoolTag : public CMonadTag
{
	CBoolTag(
		const std::wstring & pTag, 
		const std::wstring & pId,
		const bool pBoolVal,
		ostream & pOutStream )
		: CMonadTag( pTag, L"id", pId, pOutStream )
	{
		mOutStream << pBoolVal;
	}
};
void tag_property( ostream &Out, const std::wstring & pTag, const std::wstring & pId, const bool pbVal )
{
	CBoolTag( pTag, pId, pbVal, Out );
}
void windows_property( ostream &Out, const std::wstring & pId, const bool pbVal )
{
	tag_property( Out, L"windows", pId, pbVal );
}
void processor_property( ostream &Out, const std::wstring & pId, const bool pbVal )
{
	tag_property( Out, L"processor", pId, pbVal );
}
struct CMSIVersionTag : public CStringTag
{
	CMSIVersionTag(
		const std::wstring & pTag, 
		const std::wstring & pId,
		const std::wstring & pMajorVerNum,
		const std::wstring & pMinorVerNum,
		ostream & pOutStream )
		: CStringTag( pTag, pId, pMajorVerNum + std::wstring( L"." ) + pMinorVerNum, pOutStream )
	{
	}
};
struct CMSI_GUIDTag : public CStringTag
{
	CMSI_GUIDTag(
		ostream & pOutStream )
		: CStringTag( L"msi_guid", L"GUID", MonadAppGuid, pOutStream )
	{
	}
};

ostream & operator<< ( ostream &Out, const CMonadDeploymentVersion &SrcVer )
{
	CMonadTag Platform( L"platform", L"", L"", Out );
	windows_property( Out, L"WinXP_SP2", SrcVer.IsWindows_XP_SP2() );
	windows_property( Out, L"Win7", SrcVer.IsWindows_7() );
	windows_property( Out, L"Server", SrcVer.IsWindowsServer() );

	processor_property( Out, L"X64", SrcVer.Is64Bit() );
	//processor_property( Out, "SSE", SrcVer.IsSSE() );

	if( gTmp )
		LogFreeSpace( Out, *gTmp, L"MonadTmp" );
	LogFreeSpace( Out, MonadFile::GetSystemFolder(), L"System" );
	LogFreeSpace( Out, MonadFile::GetWindowsFolder(), L"Windows" );
	LogFreeSpace( Out, MonadFile::GetProgramsFolder(), L"Programs" );
	if( SrcVer.IsInstalled() )
	{
		CMSIVersionTag( 
			L"msi",
			L"Deployed",
			SrcVer.InstalledMajor(),
			SrcVer.InstalledMinor(),
			Out );
	}
	CMSIVersionTag( 
		L"msi",
		L"New",
		g_szMajorVer,
		g_szMinorVer,
		Out );
	{
		CMSI_GUIDTag GUID( Out );
	}

	return Out;
}
