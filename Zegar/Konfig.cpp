// ###################################################################
// Konfig.cpp - kod okienka konfigurayjnego oraz t³umaczeñ jêzykowych.
// Kod zawarty w tym pliku umo¿liwia otwarcie okienka dialogowego.
// Inn¹ w³aœciwoœci¹ jest implementacja ³añcuchów w kilku jêzykach.
// £añcuchy tekstowe w zasobach posiadaj¹ kodowonie "NEUTRAL",
// dziêki temu system operacyjny nie decyduje o ³adowaniu zasobów.
// Te same teksty w trzech jêzykach s¹ zawarte w pewnych przedzia³ach
// identyfikatorów. Dodaj¹c pewne przesuniêcie mo¿na uzyskaæ
// analogiczny tekst w wybranym jêzyku.
// ###################################################################

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <locale.h>
#include "Konfig.h"
#include "Dzwiek.h"
#include "Resource.h"

class CDzwiek * g_pDzwiek;

HFONT URLfont = NULL;

namespace MiniWinApi
{
	static const UINT g_uCntDialogElements = 7;
	static const UINT LangPolish = 117; // Przesuniêcie
	static const UINT LangRussian = 126; // Przesuniêcie
	static const UINT LangEnglish = 134; // Przesuniêcie
	// W ka¿dym jêzyku jest LangRussian - LangPolish
	// ³añcuchów.

	const inline DWORD Jezyk()
	{
		const TCHAR * collate = _wsetlocale(LC_COLLATE, L"");

		if(wcscmp(L"Polish_Poland.1250", collate) == 0) // Polska
			return LangPolish;
		else if(wcscmp(L"Russian_Russia.1251", collate) == 0) // Rosja
			return LangRussian;
		else // Dowolny inny kraj
			return LangEnglish;
	}

	static UINT GetResIDFromIndex (const UINT pIndToResID)
	{
		const static DWORD dwJezyk = Jezyk();
		return pIndToResID + dwJezyk;
	}

	static UINT GetItemIDFromIndex (const UINT pIndToDlgID)
	{
		static const UINT Zmiany [g_uCntDialogElements] =
		{
			IDCANCEL,
			IDC_CHECKSEC,
			IDC_BUTTONDZWIEK,
			IDC_RAMKA_GLOS,
			IDC_E_GLOS,
			IDC_RAMKA_GRAF,
			IDC_CHECKOPAD
		};
		if(pIndToDlgID >= g_uCntDialogElements)
			ExitProcess(DXTRACE_ERR( TEXT("GetItemIDFromIndex: pInd18toResID too big"), E_FAIL ));
		return Zmiany [pIndToDlgID];
	}

	static const int maxloadlen = 256;
	static TCHAR str [maxloadlen]; 

	TCHAR* LoadStringI18N(const UINT pStrId)
	{
		const UINT m_StrId = GetResIDFromIndex(pStrId);
		const int ret = LoadString(GetModuleHandle(NULL), m_StrId, str, maxloadlen);
		if(0 == ret)
		{
			str[0] = 0;
			DXTRACE_ERR( TEXT("LoadStringI18N"), GetLastError() );
		}
		return str;
	}

	HWND GetDlgItem(HWND hDlg, int nIDDlgItem)
	{
		HWND hItem = ::GetDlgItem(hDlg, nIDDlgItem);
		if(0 == hItem)
		{
			DXTRACE_ERR( TEXT("GetDlgItem"), GetLastError() );
		}
		return hItem;
	}

	BOOL SetWindowText( 
		HWND hWnd, 
		const UINT pStrId)
	{
		if(0 == LoadStringI18N(pStrId))
			return FALSE;
		BOOL ret = ::SetWindowText(hWnd, str);
		if(!ret)
		{
			DXTRACE_ERR( TEXT("SetWindowText"), GetLastError() );
		}
		return ret;
	}
}

//-----------------------------------------------------------------------------
// Name: OnInitDialog()
// Desc: Initializes the dialogs (sets up UI controls, etc.)
//-----------------------------------------------------------------------------
HRESULT OnInitDialog( HWND hDlg )
{
	FUNC_TRACE;
	HRESULT hr;
	HWND m_URLstatic;
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
	m_URLstatic = MiniWinApi::GetDlgItem (hDlg, IDC_URL);
	SendMessage(m_URLstatic, WM_SETFONT, (WPARAM) URLfont, TRUE);

	// Load the icon
	HICON hIcon = LoadIcon( GetModuleHandle(NULL), MAKEINTRESOURCE( IDI_SMALL ) );

	// Tytu³ okienka (I18N).
	V_RETURN(MiniWinApi::SetWindowText(hDlg,I18N_KONFIG_TYTUL));
	for(DWORD dwInd = 0; dwInd < MiniWinApi::g_uCntDialogElements; dwInd ++)
	{
		V_RETURN(MiniWinApi::SetWindowText(
			MiniWinApi::GetDlgItem(hDlg, MiniWinApi::GetItemIDFromIndex(dwInd)), 
			dwInd + 2));
	}
	g_pDzwiek = new CDzwiek(hDlg);
	if(g_pDzwiek)
		if(FAILED(g_pDzwiek->OnCreateDevice(NULL, NULL)))
			SAFE_DELETE(g_pDzwiek);
	if(NULL == g_pDzwiek)
		MessageBox(0, TEXT("Error creating sound manager"), MiniWinApi::LoadStringI18N(I18N_KONFIG_TYTUL), 0);

	// Set the icon for this dialog.
	SendMessage( hDlg, WM_SETICON, ICON_BIG,   (LPARAM) hIcon );  // Set big icon
	SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM) hIcon );  // Set small icon

	SendDlgItemMessage( hDlg, IDC_SLIDER_MID, TBM_SETRANGE, FALSE, MAKELONG( MINGLOS, MAXGLOS ) );
	SendDlgItemMessage( hDlg, IDC_SLIDER_MID, TBM_SETPOS,   TRUE, g_wGlosnosc );

	CheckDlgButton(hDlg, IDC_CHECKSEC, g_bWskazSek ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, IDC_CHECKOPAD, g_bOpadanie ? BST_CHECKED : BST_UNCHECKED);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: MainDlgProc()
// Desc: Handles dialog messages
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	FUNC_TRACE;

	HRESULT hr;

	static int ZliczTiki;
	switch( msg ) 
	{
	case WM_INITDIALOG:
		if( FAILED( hr = OnInitDialog( hDlg ) ) )
		{
			EndDialog( hDlg, 0 );
			return TRUE;
		}
		break;

	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL); 
			break;
		case IDOK:
			g_bWskazSek = IsDlgButtonChecked(hDlg, IDC_CHECKSEC) == BST_CHECKED;
			g_bOpadanie = IsDlgButtonChecked(hDlg, IDC_CHECKOPAD) == BST_CHECKED;
			EndDialog(hDlg, IDOK);
			break;
		case IDC_BUTTONDZWIEK:
			if(g_pDzwiek)
			{
				ZliczTiki = 4;
				g_pDzwiek->GrajGong();
				g_pDzwiek->GrajTik();
				SetTimer( hDlg, 1, 1000, NULL );
			}
			break;
		default:
			return FALSE; // Didnt handle message
			break;
		}
		break;

	case WM_TIMER:
		if(g_pDzwiek)
			g_pDzwiek->GrajTik();
		ZliczTiki --;
		if(ZliczTiki == 0)
			KillTimer(hDlg, 1);
		break;

	case WM_NOTIFY:
		switch( wParam )
		{
		case IDC_SLIDER_MID :
			WORD m_wGlos;
			m_wGlos = LOWORD((long)SendDlgItemMessage( hDlg, IDC_SLIDER_MID, TBM_GETPOS, 0, 0 ));
			if(m_wGlos != g_wGlosnosc)
			{
				g_wGlosnosc = m_wGlos;
				if(g_pDzwiek)
				{
					g_pDzwiek->Ustaw(m_wGlos);
					g_pDzwiek->Glosnosc();
				}
			}
			break;
		case IDC_URL:
			if(NM_CLICK == ((LPNMHDR) lParam)->code)
			{
				const TCHAR PLClick [] = L"http://screensaver.kubera.info/?Language=Polish&Version=2.0.0.3";
				const TCHAR RUClick [] = L"http://screensaver.kubera.info/?Language=Russian&Version=2.0.0.3";
				const TCHAR ENClick [] = L"http://screensaver.kubera.info/?Language=English&Version=2.0.0.3";
				const TCHAR * Click;
				switch (MiniWinApi::Jezyk())
				{
				case MiniWinApi::LangPolish :
					Click = PLClick;
					break;
				case MiniWinApi::LangRussian :
					Click = RUClick;
					break;
				default:
					Click = ENClick;
				}
				ShellExecute(hDlg,
					L"open",
					Click,
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
	case WM_DESTROY:
		KillTimer( hDlg, 1 );
		SAFE_DELETE(g_pDzwiek);
		DeleteObject(URLfont);
		break;

	default:
		return FALSE; // Didnt handle message
	}

	return TRUE; // Handled message
}
HRESULT MyDoConfig(const HINSTANCE p_hinst, const HWND p_parent)
{
	FUNC_TRACE;
	const HWND hWndMe = FindWindow(NULL, MiniWinApi::LoadStringI18N(I18N_KONFIG_TYTUL));
	if (hWndMe)
	{
		MessageBeep(MB_ICONHAND);
		if (IsIconic(hWndMe)) 
		{
			ShowWindow(hWndMe, SW_RESTORE);
		}
		SetForegroundWindow(hWndMe);
		return 0;
	}

	// Aplikacja stosuje Common Controls.
	INITCOMMONCONTROLSEX m_CC_Init;
	m_CC_Init.dwSize = sizeof(m_CC_Init);
	m_CC_Init.dwICC  = ICC_BAR_CLASSES|ICC_LINK_CLASS;
	if(InitCommonControlsEx(&m_CC_Init) == TRUE)
		switch (DialogBox( NULL, MAKEINTRESOURCE(IDD_DIALOG_KONF), p_parent, MainDlgProc ))
	{
		case IDOK:
			return IDOK;
		case IDCANCEL:
			return IDCANCEL;
	}
	return E_FAIL;
}

/*CGrowableArray<TCHAR *> CreatedTemps;

TCHAR * TmpFile(const TCHAR * pPrefix, const INT pID)
{
TCHAR szDir[MAX_PATH];
if(GetTempPath(sizeof(szDir)/sizeof(TCHAR), szDir) == 0)
return NULL;

static TCHAR szFileName[MAX_PATH];
if(GetTempFileName(szDir, pPrefix, 0, szFileName) == 0)
return NULL;

HANDLE hTemp = CreateFile(szFileName,
GENERIC_WRITE,
FILE_SHARE_READ,	// Tryb wy³¹czonoœci
NULL, // Domyœlny deskryptor zabezpieczeñ
CREATE_ALWAYS,
FILE_ATTRIBUTE_NOT_CONTENT_INDEXED |
FILE_ATTRIBUTE_NORMAL,
NULL);
if( hTemp == INVALID_HANDLE_VALUE )
return NULL;
DWORD Written, len;
LPCVOID lpRsrc = ZaladujZasob(pID, len);
if(lpRsrc == NULL)
return NULL;
WriteFile (hTemp,
lpRsrc,
len,
&Written,
NULL );
if(!CloseHandle(hTemp))
return NULL;

TCHAR * tmp = new TCHAR[wcslen(szFileName) + 1];
if(tmp == NULL)
return NULL;
if(SUCCEEDED(wcscpy_s(tmp, wcslen(szFileName) + 1, szFileName)))
{
CreatedTemps.Add(szFileName);
return len != Written ? NULL : szFileName;	
}
return NULL;
}*/