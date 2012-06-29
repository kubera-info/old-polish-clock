#include "DXUT.h"
#include "MonadFileTimestamp.h"

void MonadSetFileTime( const std::wstring & pFileName )
{
	CAtlFile mFile;
	if( SUCCEEDED( mFile.Create( pFileName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, OPEN_EXISTING ) ) )
	{
		MonadSetFileTime( mFile );
	}
}

FILETIME MonadGetCurrentFTime()
{
	SYSTEMTIME st;
	st.wYear = 2012;
	st.wMonth = 5;  
	st.wDayOfWeek = 0;  
	st.wDay = 3;  
	st.wHour = 0;
	st.wMinute = 0;  
	st.wSecond = 0;  
	st.wMilliseconds = 0;
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);  

	return ft;
}
void MonadSetFileTime( CAtlFile & pAtlFile )
{
	const static FILETIME ft( MonadGetCurrentFTime() );

	SetFileTime(pAtlFile,           // sets last-write time for file
		&ft, &ft, &ft);
}