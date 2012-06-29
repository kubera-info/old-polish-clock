#pragma once
#ifndef _MONAD_GLOBALS_H_
#define _MONAD_GLOBALS_H_

#include "MonadUniqueInstance.h"

extern const std::wstring g_szMajorVer;
extern const std::wstring g_szMinorVer;
extern volatile bool bAnyError;
extern volatile bool bRebootRequired;
extern volatile bool g_bSilent;
extern volatile bool g_bContinue;
extern volatile bool g_bInstalled;

extern volatile bool g_bCancelInstall;


extern volatile unsigned ulSetupThreadStatus;

extern CAmIUnique g_AmIUnique;

extern const UINT WM_THREAD_DONE;
extern const UINT WM_ERROR_MESSAGE;

extern HWND gHWND;

extern const WCHAR g_szContinue [];
extern const WCHAR g_szNoDX [];
extern const WCHAR g_szNoApp [];
extern const WCHAR g_szSilent [];

#endif // _MONAD_GLOBALS_H_