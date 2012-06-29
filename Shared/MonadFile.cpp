#include "DXUT.h"
#include <string>
#include <atlfile.h>
#include <shlobj.h>
#include "MonadFile.h"

namespace MonadFile
{
	std::wstring GetFolder( int iFolderType )
	{
		TCHAR strPath[ MAX_PATH ];
		SHGetSpecialFolderPath(
			0,
			strPath,
			iFolderType,
			FALSE );

		return strPath;
	}

	std::wstring GetWindowsFolder()
	{
		return GetFolder( CSIDL_WINDOWS );
	}

	std::wstring GetSystemFolder()
	{
		return GetFolder( CSIDL_SYSTEM );
	}

	std::wstring GetProgramsFolder()
	{
		return GetFolder( CSIDL_PROGRAM_FILES );
	}

	std::wstring GetAppDataFolder()
	{
		return GetFolder( CSIDL_APPDATA );
	}
}