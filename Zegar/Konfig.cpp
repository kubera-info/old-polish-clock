#include "stdafx.h"
#include <commctrl.h>
#include <locale.h>
#include "D3DSaver.h"
#include "Konfig.h"
#include "Dzwiek.h"
#include "Resource.h"

HINSTANCE g_hInst;

#define MAXLOADLEN (4096)
TCHAR str [MAXLOADLEN];


#define IL_OKIEN (10)
const UINT Zmiany [IL_OKIEN] = 
{ 
    IDCANCEL,
    IDC_CHECKSEC,
    IDC_BUTTONDZWIEK,
    IDC_RAMKA_GLOS,
    IDC_E_GLOS,
    IDC_JAKOSC,
	IDC_WIELKOSC,
	IDC_RAMKA_GRAF,
	IDC_WYDAJNOSC
};

static BYTE bJezyk;

inline BYTE Jezyk()
{
	const TCHAR * colate = _tsetlocale(LC_COLLATE, "");
 
	if(_tcscmp(_T("Polish_Poland.1250"), colate) == 0)
		return   0;
	else if(_tcscmp(_T("Russian_Russia.1251"), colate) == 0)
		return 200;
	else
		return 100;
}

//-----------------------------------------------------------------------------
// Name: OnInitDialog()
// Desc: Initializes the dialogs (sets up UI controls, etc.)
//-----------------------------------------------------------------------------
HRESULT OnInitDialog( HWND hDlg )
{
    // Load the icon
    HICON hIcon = LoadIcon( g_hInst, MAKEINTRESOURCE( IDI_SMALL ) );

	SetWindowText(hDlg, str);	
	for(DWORD dwInd = 0; dwInd < IL_OKIEN; dwInd ++)
	{
		LoadString(NULL, Zmiany[dwInd] + bJezyk, str, MAXLOADLEN);
		SetWindowText(GetDlgItem(hDlg, Zmiany[dwInd]), str);
	}
	g_pDzwiek = NULL;
	g_pDzwiek = new CDzwiek();
	if(g_pDzwiek)
		if(FAILED(g_pDzwiek->Create()))
			SAFE_DELETE(g_pDzwiek);

    // Set the icon for this dialog.
    SendMessage( hDlg, WM_SETICON, ICON_BIG,   (LPARAM) hIcon );  // Set big icon
    SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM) hIcon );  // Set small icon

    SendDlgItemMessage( hDlg, IDC_SLIDER_MID, TBM_SETRANGE, FALSE, MAKELONG( MINGLOS, MAXGLOS ) );
    SendDlgItemMessage( hDlg, IDC_SLIDER_MID, TBM_SETPOS,   TRUE, g_wGlosnosc );

	SendDlgItemMessage( hDlg, IDC_SLIDER_D3D, TBM_SETRANGE, FALSE, MAKELONG( MINJAKOSC, MAXJAKOSC ) );
    SendDlgItemMessage( hDlg, IDC_SLIDER_D3D, TBM_SETPOS,   TRUE, g_wJakoscGr );

	SendDlgItemMessage( hDlg, IDC_SLIDER_MAX, TBM_SETRANGE, FALSE, MAKELONG( MINWIELKOSC, MAXWIELKOSC ) );
    SendDlgItemMessage( hDlg, IDC_SLIDER_MAX, TBM_SETPOS,   TRUE, g_wWielkosc );

	CheckDlgButton(hDlg, IDC_CHECKSEC, g_bWskazSek ? BST_CHECKED : BST_UNCHECKED);

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: MainDlgProc()
// Desc: Handles dialog messages
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
    HRESULT hr;

    switch( msg ) 
    {
        case WM_INITDIALOG:
            if( FAILED( hr = OnInitDialog( hDlg ) ) )
            {
                DXTRACE_ERR( TEXT("OnInitDialog"), hr );
                EndDialog( hDlg, 0 );
                return TRUE;
            }
            break;

        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {

                case IDCANCEL:
					EndDialog(hDlg, wParam); 
                    break;
                case IDOK:
					g_bWskazSek = IsDlgButtonChecked(hDlg, IDC_CHECKSEC) == BST_CHECKED;
                    EndDialog(hDlg, wParam);
                    break;
				case IDC_BUTTONDZWIEK:
					if(g_pDzwiek)
						g_pDzwiek->GrajGong();
					break;
                default:
                    return FALSE; // Didnt handle message
					break;
            }
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
						g_pDzwiek->Glosnosc(m_wGlos);
				}				
//				g_pDzwiek->GrajGong();
				break;
			case IDC_SLIDER_MAX :
				g_wWielkosc = LOWORD((long)SendDlgItemMessage( hDlg, IDC_SLIDER_MAX, TBM_GETPOS, 0, 0 ));
				break;
			case IDC_SLIDER_D3D :
				g_wJakoscGr = LOWORD((long)SendDlgItemMessage( hDlg, IDC_SLIDER_D3D, TBM_GETPOS, 0, 0 ));
				break;
			default:
				return FALSE;
				break;
			}
			break;

        case WM_DESTROY:
            SAFE_DELETE(g_pDzwiek);
			break;

        default:
            return FALSE; // Didnt handle message
    }

    return TRUE; // Handled message
}

HRESULT MyDoConfig(HINSTANCE p_hinst, HWND p_par)
{
	bJezyk = Jezyk();
	LoadString(NULL, IDD_DIALOG_KONF + bJezyk, str, MAXLOADLEN);

	HWND hWndMe = FindWindow(NULL, str);
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

	g_hInst = p_hinst;
	INITCOMMONCONTROLSEX m_CC_Init;
	m_CC_Init.dwSize = sizeof(m_CC_Init);
	m_CC_Init.dwICC  = ICC_BAR_CLASSES;
	if(InitCommonControlsEx(&m_CC_Init) == TRUE)
		if(IDOK == DialogBox( NULL, MAKEINTRESOURCE(IDD_DIALOG_KONF), p_par, MainDlgProc ))
			return S_OK;
	return E_FAIL;
}
#undef MAXLOADLEN			

bool DirectX9OK()
{
	TCHAR m_Sciezka [_MAX_PATH];
	GetSystemDirectory(m_Sciezka, sizeof(m_Sciezka) / sizeof(TCHAR));
	_tcscat(m_Sciezka, _T("\\D3D8.DLL"));
	HMODULE hD3D9DLL = LoadLibraryEx( m_Sciezka, NULL, LOAD_LIBRARY_AS_DATAFILE ); 
	if(hD3D9DLL)
	{
		FreeLibrary(hD3D9DLL);
		return true;
	}
	else
		return false;
}
