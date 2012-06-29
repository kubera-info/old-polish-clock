#include "DXUT.h"
#include "MonadSetupReg.h"
#include "Dict.h"
#include "Requirements.h"
#include "MonadRegistry.h"

#define GUID_A L"4EE4E7F2"
#define GUID_B L"04CE"
#define GUID_C L"4221"
#define GUID_D L"AA00"
#define GUID_E L"522FAB47F814"

#define APP_GUID L"{" GUID_A L"-" GUID_B L"-" GUID_C L"-" GUID_D L"-" GUID_E L"}"
#define APP_GUID_SHORT GUID_A GUID_B GUID_C GUID_D GUID_E
const WCHAR MonadAppGuid [] = APP_GUID;
/*static const WCHAR MonadSetupRegPath [] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"APP_GUID;*/
//static const WCHAR MonadSetupRegPath2 [] = L"SOFTWARE\\Classes\\Installer\\Products\\"APP_GUID_SHORT;
const WCHAR MonadSetupRegContinuePath [] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce";

/*void AlterRegistry()
{
	if( g_bInstalled )
	{
		{
			struct CDisplayName : private CRegistry
			{
				CDisplayName()
					: CRegistry( HKEY_LOCAL_MACHINE, MonadSetupRegPath, KEY_WRITE + KEY_WOW64_64KEY )
				{
					SetString( L"DisplayName", GetOPC() );
					SetDWORD( L"NoRepair", 1 );
					SetDWORD( L"NoModify", 1 );
				}
			} DisplayName;
		}
		{
			struct CProductName : private CRegistry
			{
				CProductName()
					: CRegistry( HKEY_LOCAL_MACHINE, MonadSetupRegPath2, KEY_WRITE + KEY_WOW64_64KEY )
				{
					SetString( L"ProductName", GetOPC() );
				}
			} ProductName;
		}
	}
}*/

void RegisterContinuation()
{
	struct CRegisterContinuation : private CRegistry
	{
		CRegisterContinuation()
			: CRegistry( HKEY_CURRENT_USER, MonadSetupRegContinuePath, KEY_WRITE + KEY_WOW64_64KEY )
		{
			std::wstring Call( L"\"" );
			Call += ::__wargv[ 0 ];
			Call += L"\" ";
			Call += g_szContinue;
			Call += L" /" + GetLanguageString();
			Call += L' ';
			Call += g_szNoDX;
			if( g_bInstalled )
			{
				Call += L' ';
				Call += g_szNoApp;
			}
			SetString( APP_GUID, Call );
		}
	} RegisterContinuation;
}