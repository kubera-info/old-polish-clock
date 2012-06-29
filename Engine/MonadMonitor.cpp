#include "MonadMonitor.h"
#include "MonadException.h"

HMONITOR GetPrimaryMonitor() throw()
{
	POINT pt;
	pt.x = pt.y = 0;
	//the primary monitor always has its origin at 0,0
	return MonitorFromPoint( pt, MONITOR_DEFAULTTOPRIMARY );
}

bool IsPrimaryMonitor( HMONITOR p_hMonitor )
{
	
	MONITORINFO mi;

	mi.cbSize = sizeof( mi );
	THROW_EXC_IFZ( GetMonitorInfo( p_hMonitor, &mi ), MonadException::InvalidCall, "Is Primary" );
	
	return mi.dwFlags == MONITORINFOF_PRIMARY;
}

RECT GetMonitorRect( HMONITOR p_hMonitor )
{
	MONITORINFO mi;

	mi.cbSize = sizeof( mi );
	THROW_EXC_IFZ( GetMonitorInfo( p_hMonitor, &mi ), MonadException::InvalidCall, "Get Monitor" );

	return mi.rcMonitor;
}

BOOL CALLBACK MonadRegisterMonitor(
  __in  HMONITOR hMonitor,
  __in  HDC hdcMonitor,
  __in  LPRECT lprcMonitor,
  __in  LPARAM dwData
)
{
	if( !IsPrimaryMonitor( hMonitor ) )
		( ( CMonitors * ) dwData )->insert( * lprcMonitor );

	return TRUE;
}

CMonitors GetNonPrimaryMonitors()
{
	CMonitors Monitors;

	EnumDisplayMonitors(
		NULL,
		NULL,
		MonadRegisterMonitor,
		( LPARAM ) &Monitors );

	return Monitors;
}