#pragma once
#ifndef _MONAD_MONITOR_H_
#define _MONAD_MONITOR_H_

#include <windows.h>
#include "MonadTemplates.h"

typedef std::set< RECT > CMonitors;

HMONITOR GetPrimaryMonitor() throw();
bool IsPrimaryMonitor( HMONITOR p_hMonitor );
RECT GetMonitorRect( HMONITOR p_hMonitor = GetPrimaryMonitor() );
CMonitors GetNonPrimaryMonitors();

#endif // _MONAD_MONITOR_H_