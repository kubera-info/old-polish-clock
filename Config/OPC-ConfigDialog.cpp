#include "DXUT.h"
#include <commctrl.h>
#include <shellapi.h>
#include <string>
#include "OPC-ConfigDialog.h"
#include "DictOPC.h"
#include "resource.h"
#include "System.h"
#include "MonadAudio.h"
#include "FileManager.h"
#include "MonadRegistry.h"
#include "D3DSaver.h"
#include "MonadException.h"

using namespace MonadStrings;
#define POMIEDZY(a, x, y, b) (((a) >= (x) && (a) <= (y)) ? (a) : (b))

static HFONT URLfont = NULL;

static const std::wstring ClockVer( L"http://screensaver.kubera.info/?Language=" );
LPCWSTR Profile( L"SOFTWARE\\www.kubera.info\\Screen Saver\\Clock3D\\Config-3.0" );
static LPCWSTR SoundVolume( L"SoundVolume" );
static LPCWSTR I18N( L"I18N" );

#define MINGLOS ( 0 )
#define MAXGLOS ( 1000 )

static UINT_PTR gTimer( 0 );

long g_wGlosnosc( MAXGLOS );

//-----------------------------------------------------------------------------
// Name: DXUtil_ReadIntRegKey()
// Desc: Helper function to read a registry key int
//-----------------------------------------------------------------------------
void DXUtil_ReadIntRegKey( HKEY hKey, const WCHAR* strRegName, DWORD* pdwValue, 
						  DWORD dwDefault )
{
	DWORD dwType;
	DWORD dwLength = sizeof(DWORD);

	if( ERROR_SUCCESS != RegQueryValueEx( hKey, strRegName, 0, &dwType, 
		(BYTE*)pdwValue, &dwLength ) )
	{
		*pdwValue = dwDefault;
	}
}

static void SetConfigVolume()
{
	const float fVolume( Windows2Volume( g_wGlosnosc ) );
	MonadAudio::SetVoiceVolume( fVolume, 1 );
	MonadAudio::SetVoiceVolume( fVolume, 2 );
}

static void SetMasterVolume()
{
	const float fVolume( Windows2Volume( g_wGlosnosc ) );
	MonadAudio::SetMasterVolume( fVolume );
}

void SetVolumeFromRegistry()
{
	struct CReadProfileRegistry : public CRegistry
	{
		CReadProfileRegistry()
			: CRegistry( HKEY_CURRENT_USER, Profile, KEY_READ )
		{
			DWORD dw_tmp( 0 ), dw_I18N( 0 );
			DXUtil_ReadIntRegKey( m_hKey, SoundVolume, &dw_tmp, g_wGlosnosc );
			g_wGlosnosc = POMIEDZY( dw_tmp, MINGLOS, MAXGLOS, g_wGlosnosc );

			DXUtil_ReadIntRegKey( m_hKey, I18N, &dw_I18N, GetDefaultLanguage() );
			if( LANG_PL == dw_I18N || LANG_RU == dw_I18N || LANG_EN == dw_I18N )
			{
				SetLanguage( ( MonadI18N::MONAD_DEF_LANG ) dw_I18N );
				SetIULocale();
			}

		}
	} mReadReg;
	if( GetSaverParams().m_SaverMode == sm_config )
		SetConfigVolume();
	else
		SetMasterVolume();
}

float Windows2Volume( long pdwVolume )
{
	return float( pdwVolume ) / MAXGLOS;
}
static long Volume2Windows( float pVolume )
{
	return long( pVolume * MAXGLOS );
}

static void OnI18N( HWND hDlg )
{
	int iItem;
	switch( GetLanguage() )
	{
	case LANG_PL:
		iItem = IDC_RADIO_POL;
		break;
	case LANG_RU:
		iItem = IDC_RADIO_RUS;
		break;
	default:
		iItem = IDC_RADIO_ENG;
	}
	PostMessage( GetDlgItem( hDlg, iItem ), BM_SETCHECK, BST_CHECKED, NULL );	
}

static void Focus()
{
	MonadAudio::ResetQueue();
	MonadAudio::EnqueueVoice( L"focus" );
}

static void SetLabel( HWND hDlg, int pDialogItem, const std::wstring & pLabel )
{
	SetWindowText( GetDlgItem( hDlg, pDialogItem ), pLabel.c_str() );
}

static void SetLabel( HWND hDlg, int pDialogItem, const CI18NString & pLabel )
{
	SetLabel( hDlg, pDialogItem, ( LPCWSTR ) pLabel );
}

static void SetTranslation( HWND hDlg )
{
	SetWindowText( hDlg, gConfigTitle );

	SetLabel( hDlg, IDC_BORDER_LANG, LanguageSel() );
	SetLabel( hDlg, IDC_BORDER_VOICE, gVoice );
	SetLabel( hDlg, IDC_BORDER_SITE, gConfigSite );

	SetLabel( hDlg, IDCANCEL, GetCancel() );
	SetLabel( hDlg, IDOK, GetOK() );
	SetLabel( hDlg, IDC_LABEL_VOICE, gSoundVolume );
	SetLabel( hDlg, IDC_BUTTON_TEST, gTest );
}

static void OnCancel( HWND hDlg )
{
	MonadAudio::EnqueueVoice( L"no" );
	EndDialog(hDlg, IDCANCEL);
}
//-----------------------------------------------------------------------------
// Name: OnInitDialog()
// Desc: Initializes the dialogs (sets up UI controls, etc.)
//-----------------------------------------------------------------------------
static void OnInitDialog( HWND hDlg )
{
	URLfont = CreateFont(
		0,               // height of font
		0,                // average character width
		0,           // angle of escapement
		0,          // base-line orientation angle
		0,              // font weight
		TRUE,           // italic attribute option
		TRUE,        // underline attribute option
		FALSE,        // strikeout attribute option
		0,          // character set identifier
		0,  // output precision
		0,    // clipping precision
		0,          // output quality
		FF_DONTCARE,   // pitch and family
		L""           // typeface name
		); // Czcionka dowi¹zania do strony.
	SendMessage( GetDlgItem ( hDlg, IDC_URL ), WM_SETFONT, ( WPARAM ) URLfont, TRUE );
	SendMessage( GetDlgItem ( hDlg, IDC_URL2 ), WM_SETFONT, ( WPARAM ) URLfont, TRUE );

	OnI18N( hDlg );

	SetTranslation( hDlg );

	// Load the icon
	HICON hIcon = LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_MAIN_ICON ) );

	// Set the icon for this dialog.
	SendMessage( hDlg, WM_SETICON, ICON_BIG,   (LPARAM) hIcon );  // Set big icon
	SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM) hIcon );  // Set small icon

	SendDlgItemMessage( hDlg, IDC_SLIDER_VOICE, TBM_SETRANGE, FALSE, MAKELONG( MINGLOS, MAXGLOS ) );
	SendDlgItemMessage( hDlg, IDC_SLIDER_VOICE, TBM_SETPOS,   TRUE, g_wGlosnosc );

	MonadAudio::EnqueueVoice( L"vopros" );

	MonadException::SetWindowParent( hDlg );
}
void EnableOK( HWND hDlg )
{
	const HWND hOK( GetDlgItem ( hDlg, IDOK ) );
	if( !IsWindowEnabled( hOK ) )
		EnableWindow( hOK, TRUE );
}
void SetConfigTranslation( MONAD_DEF_LANG pLang, HWND hDlg )
{
	Focus();
	if( pLang != GetLanguage() )
	{
		EnableOK( hDlg );
		SetLanguage( pLang );
		SetTranslation( hDlg );
	}
}

void Tic()
{
	MonadAudio::EnqueueVoice( L"tic", 1 );
}

//-----------------------------------------------------------------------------
// Name: MainDlgProc()
// Desc: Handles dialog messages
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static int ZliczTiki;
	switch( msg ) 
	{
	case WM_CLOSE:
		MonadException::SetWindowParent( NULL );
		break;
	case WM_INITDIALOG:
		OnInitDialog( hDlg );
		break;
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDCANCEL:
			OnCancel( hDlg );
			break;
		case IDOK:
			{
				MonadAudio::EnqueueVoice( L"yes" );
				EndDialog(hDlg, IDOK);
				struct CWriteProfileRegistry : public CRegistry
				{
					CWriteProfileRegistry()
						: CRegistry( HKEY_CURRENT_USER, Profile, KEY_WRITE )
					{
						SetDWORD( SoundVolume, g_wGlosnosc );
						SetDWORD( I18N, GetLanguage() );
					}
				} mWriteReg;
				break;
			}
		case IDC_BUTTON_TEST:
			if(ZliczTiki != 0)
				KillTimer( hDlg, gTimer );
			gTimer ++;
			ZliczTiki = 4;
			MonadAudio::ResetQueue( 1 );
			MonadAudio::ResetQueue( 2 );
			Tic();
			MonadAudio::EnqueueVoice( L"gong", 2 );
			SetTimer( hDlg, gTimer, 1000, NULL );
			break;
		case IDC_RADIO_POL:
			SetConfigTranslation( LANG_PL, hDlg );
			break;
		case IDC_RADIO_RUS:
			SetConfigTranslation( LANG_RU, hDlg );
			break;
		case IDC_RADIO_ENG:
			SetConfigTranslation( LANG_EN, hDlg );
			break;
		default:
			return FALSE; // Didnt handle message
		}
		break;

	case WM_TIMER:
		if( wParam == gTimer )
		{
			Tic();
			ZliczTiki --;
			if( 0 == ZliczTiki )
				KillTimer( hDlg, gTimer );
		}
		break;

	case WM_NOTIFY:
		switch( wParam )
		{
		case IDC_SLIDER_VOICE :
			{
				const long m_wGlos = ( long ) SendDlgItemMessage( hDlg, IDC_SLIDER_VOICE, TBM_GETPOS, 0, 0 );
				if( m_wGlos != g_wGlosnosc )
				{
					g_wGlosnosc = m_wGlos;
					EnableOK( hDlg );
					SetConfigVolume();
				}
				break;
			}
		case IDC_URL:
			if( NM_CLICK == ( ( LPNMHDR ) lParam )->code )
			{
				Focus();
				ShellExecute( hDlg,
					L"open",
					( ClockVer + MonadI18N::GetLanguageNameString() + L"&Version=" + GetApplicationVersion() ).c_str(),
					L"",
					L"",
					SW_NORMAL );
				break;
			}
		case IDC_URL2:
			if( NM_CLICK == ( ( LPNMHDR ) lParam )->code )
			{
				Focus();
				ShellExecute( hDlg,
					L"open",
					L"https://www.facebook.com/OldPolishClock",
					L"",
					L"",
					SW_NORMAL);
				break;
			}
		default:
			return FALSE;
			break;
		}
		break;
	case WM_SYSCOMMAND:
		switch( ( wParam & 0xFFF0 ) )
		{
		case SC_CLOSE:
			OnCancel( hDlg );
			break;
		}
	case WM_DESTROY:
		KillTimer( hDlg, gTimer );
		DeleteObject( URLfont );
		break;
	default:
		return FALSE; // Didnt handle message
	}

	return TRUE; // Handled message
}

void FindWindowByTitle( HWND & hWndMe, MONAD_DEF_LANG pLang )
{
	const HWND hLocalWndMe = FindWindow( NULL, gConfigTitle.GetI18NString( pLang ).c_str() );
	if( NULL != hLocalWndMe )
		hWndMe = hLocalWndMe;
}
int MyDoConfig( const HINSTANCE p_hinst )
{
	HWND hWndMe = NULL;
	FindWindowByTitle( hWndMe, LANG_PL );
	FindWindowByTitle( hWndMe, LANG_RU );
	FindWindowByTitle( hWndMe, LANG_EN );
	if( hWndMe )
	{
		MessageBeep( MB_ICONHAND );
		if ( IsIconic( hWndMe ) ) 
			ShowWindow( hWndMe, SW_RESTORE );
		SetForegroundWindow( hWndMe );
		return 0;
	}

	CPaths mSearchPaths;
	mSearchPaths.add_path( L"Clock" );
	mSearchPaths.add_path( L"Config" );

	g_pFileManager.Attach( new CFileSearcher() );
	g_pFileManager->Run( mSearchPaths );

	// Aplikacja stosuje Common Controls.
	INITCOMMONCONTROLSEX m_CC_Init;
	m_CC_Init.dwSize = sizeof(m_CC_Init);
	m_CC_Init.dwICC  = ICC_BAR_CLASSES|ICC_LINK_CLASS;
	while( !g_pFileManager->IsFinished() )
		MonadThread::OnDequeue();
	g_pFileManager.Free();

	if( InitCommonControlsEx(&m_CC_Init) == TRUE )
		switch ( DialogBox( NULL, MAKEINTRESOURCE( IDD_DIALOG_KONF ), GetSaverParams().m_hWndParent, MainDlgProc ) )
	{
		case IDCANCEL:
		case IDOK:
			while( MonadAudio::IsPlaying() )
				Sleep( 100 );
			return ERROR_SUCCESS;
		default:
			return EXIT_FAILURE;
	}
	else
		return EXIT_FAILURE;
}