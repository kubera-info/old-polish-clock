#include "DXUT.h"
#include "MonadGlobals.h"

const std::wstring g_szMajorVer( L"3" );
const std::wstring g_szMinorVer( L"0" );
volatile bool bAnyError = false;
volatile bool bRebootRequired = false;
volatile bool g_bSilent = false;
volatile bool g_bContinue = false;
volatile bool g_bInstalled = false;
volatile bool g_bCancelInstall = false;

volatile unsigned ulSetupThreadStatus = ERROR_SUCCESS;

const UINT WM_THREAD_DONE = WM_APP + 1;
const UINT WM_ERROR_MESSAGE = WM_APP + 2;

CAmIUnique g_AmIUnique( L"Global\\MonadSetupOldPolishClock" );

const WCHAR g_szContinue [] = L"/continue";
const WCHAR g_szNoDX [] = L"/nodx";
const WCHAR g_szNoApp [] = L"/noapp";
const WCHAR g_szSilent [] = L"/auto";


HWND gHWND;