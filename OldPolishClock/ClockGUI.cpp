#include "DXUT.h"
#include "Singleton.h"
#include "OldPolishClock.h"
#include "MonadAudio.h"
#include "MonadMath.h"
#include "Hours.h"
#include "System.h"
#include "ClockGUI.h"
#include "World.h"

/*
#ifdef DEBUG
extern bool IsWM_CLOSE = true;
#else
extern bool IsWM_CLOSE = false;
#endif

const int iWide = 700;

#define BUTTON_HEIGH (80)

#define IDC_LEFT_TOOLBAR_EXIT (500)
#define IDC_LEFT_TOOLBAR_ABOUT (501)
#define IDC_LEFT_TOOLBAR_SETUP (502)
#define IDC_LEFT_TOOLBAR_PLAY (503)
#define IDC_LEFT_TOOLBAR_HELP (504)

#define IDC_ABOUT_IMAGE (0)
#define IDC_ABOUT_STATIC (1)

#define IDC_MESSAGE_BOX_TEXT (0)

#define IDC_HELP_TEXT (0)

CMonadAutoPtr<StructInstance> Instance;

HRESULT AddSemanticColorsToManager()
{
	HRESULT hr;

	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_STANDARD_OK, D3DCOLOR_ARGB( 255, 40, 255, 40 ) ) );
	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_STANDARD_CANCEL, D3DCOLOR_ARGB( 255, 255, 40, 40 ) ) );
	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_STANDARD_YES, D3DCOLOR_ARGB( 255, 40, 255, 40 ) ) );
	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_STANDARD_NO, D3DCOLOR_ARGB( 255, 255, 40, 40 ) ) );
	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_LEFT_TOOLBAR_EXIT, D3DCOLOR_ARGB( 255, 255, 40, 40 ) ) );
	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_LEFT_TOOLBAR_ABOUT, D3DCOLOR_ARGB( 255, 40, 40, 255 ) ) );
	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_LEFT_TOOLBAR_EXIT, D3DCOLOR_ARGB( 255, 255, 40, 40 ) ) );
	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_LEFT_TOOLBAR_ABOUT, D3DCOLOR_ARGB( 255, 40, 40, 255 ) ) );
	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_LEFT_TOOLBAR_SETUP, D3DCOLOR_ARGB( 255, 255, 255, 40 ) ) );
	V_RETURN( Singleton->MonadManager.AddSemanticColor( IDC_LEFT_TOOLBAR_PLAY, D3DCOLOR_ARGB( 255, 40, 255, 40 ) ) );
	return Singleton->MonadManager.AddSemanticColor( IDC_LEFT_TOOLBAR_HELP, D3DCOLOR_ARGB( 255, 255, 40, 255 ) );
}


void CALLBACK OnGUIEventExit( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	if( IDC_STANDARD_YES == nControlID )
	{
		IsWM_CLOSE = true;
		DXUTShutdown( 0 );
	}
	else
		Instance->ExitMsgBox.Show( false );
}

// Formatted buttons
HRESULT CLeftToolbar::AddMenuButton( int ID, int & CurrentYPos, LPCWSTR strText )
{
	HRESULT hr;

	hr = AddColorButton( ID, strText, 50, CurrentYPos, 200, 30 );
	CurrentYPos += 50;

	return hr;
}

CLeftToolbar::CLeftToolbar()
{
	SetSize( 300, 512 /* Dummy height value * / );

	int CurrentYPos = 82;
	AddMenuButton( IDC_LEFT_TOOLBAR_HELP, CurrentYPos, L"Bryk" );
	AddMenuButton( IDC_LEFT_TOOLBAR_SETUP, CurrentYPos, L"Ustawienia" );
	AddMenuButton( IDC_LEFT_TOOLBAR_PLAY, CurrentYPos, L"Gra" );
	AddMenuButton( IDC_LEFT_TOOLBAR_EXIT, CurrentYPos, L"Zamkniêcie Programu" );
	AddMenuButton( IDC_LEFT_TOOLBAR_ABOUT, CurrentYPos, L"O Programie" );
	AddTitle( L"Menu" );
}

HRESULT CLeftToolbar::OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	SetLocation( 9, 9 );
	SetSize( GetWidth(), pBackBufferSurfaceDesc->Height - 20 );

	return S_OK;
}

void CLeftToolbar::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch (nControlID)
	{
	case IDC_LEFT_TOOLBAR_ABOUT :
		{
			Instance->About.Show( true );
			break;
		}
	case IDC_LEFT_TOOLBAR_EXIT :
		{
			PostMessage( DXUTGetHWND(), WM_CLOSE, 0, 0 );
			break;
		}
	case IDC_LEFT_TOOLBAR_HELP :
		{
			Instance->HelpDialog.Show( true );
			break;
		}
	case IDC_LEFT_TOOLBAR_SETUP :
		{
			//Instance->SettingsDialog.Show( true );
			break;
		}
	/*case IDC_LEFT_TOOLBAR_PLAY :
		{
			if( !g_pWorld->m_clock.ZoomIn() ) // Zoom in progress?
				MessageBeep( MB_ICONASTERISK );
			else
			{
				SetVisible( false );

				MonadAudio::EnqueueVoice( L"zaraz" );
				IntializeToMidday();
				g_aktual_czas.wHour = 0;
				g_aktual_czas.wMinute = 0;
				g_aktual_czas.wSecond = 0;
			}
		}* /
	}
}

CAboutDlg::CAboutDlg()
{
	SetVisible( false );
	SetSize( iWide, 550 );
	AddImage( IDC_ABOUT_IMAGE, L"about", EXT_NEXT_VAL, 150, 67, 400 );

	CString AboutFormat( L"Wygaszacz Ekranu Stary Polski Zegar w wersji: 2011 (" );
	AboutFormat.Append( GetApplicationVersion() );
	AboutFormat.Append( L" PL)\n"
		L"Copyright © 2010 by kubera.info\n"
		L"Wszelkie prawa zastrze¿one\n"
		L"Autorzy: W³odzimierz Olgierd Kubera,\n"
		L"Kirill Ponomarev\n" );

	AddSpecialStatic( IDC_ABOUT_STATIC,
		AboutFormat, 10, 284, GetWidth() - 10, 280 );

	AddColorButton( IDC_STANDARD_OK, L"OK", 310, 506, BUTTON_HEIGH, 30, 13, true );

	// Declare title in this method
	AddTitle( L"O programie" );
}

void CAboutDlg::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch (nControlID)
	{
	case IDC_STANDARD_OK :
		{
			Instance->About.Show( false );
			break;
		}
	}
}

CMessageBox::CMessageBox()
{
	SetVisible( false );
	SetSize( iWide, 220 );
	AddTitle( NULL );
	AddSpecialStatic( IDC_MESSAGE_BOX_TEXT, NULL,
		10, 10, GetWidth() - 10, GetHeight() - 50 );
	AddColorButton( IDC_STANDARD_YES, L"Tak", 260, 156, BUTTON_HEIGH, 30, L'T', true );
	AddColorButton( IDC_STANDARD_NO, L"Nie", 380, 156, BUTTON_HEIGH, 30, L'N', false );
}

void CMessageBox::SetupBox( LPCWSTR strText, LPCWSTR strTitle, PCALLBACKDXUTGUIEVENT pCallback )
{
	AddTitle( strTitle );
	SetCallback( pCallback );
	CDXUTExtStatic * stat = GetSpecialStatic( IDC_MESSAGE_BOX_TEXT );
	stat->SetText( strText );
	Show( true );
}

CHelpDialog::CHelpDialog()
{
	SetVisible( false );
	SetSize( iWide, 550 );
	const WCHAR InfoPage01 [] =
		L"Program \"M³ody Zegarmistrz\" jest przeznaczony do nauki o budowie zegara analogowego\n"
		L"oraz o zapisie czasu na tarczy zegarowej.\n"
		L"\n"
		L"Celem jest zapoznanie dziecka z urz¹dzeniem za pomoc¹ æwiczenia.\n"
		L"\n";

	const WCHAR InfoPage01a [] =
		L"W celu wykonania æwiczenia z dzieckiem,\n"
		L"w pierwszej kolejnoœci Naciœnij przycisk \"Ustawienia\".\n"
		L"Wybierz, z których opcji aplikacja mo¿e korzystaæ.\n";

	const WCHAR InfoPage01b [] =
		L"Nastêpnie zamknij okienko i naciœnij przycisk \"Gra\".\n"
		L"Od tego momentu, program bêdzie dzia³a³ w trybie æwiczenia.\n"
		L"\n"
		L"W ka¿dej chwili Mo¿esz przerwaæ grê naciskaj¹c\n"
		L"klawisz \"Esc\".\n"
		L"Milej nauki i zabawy.\n";

	AddSpecialStatic( 0,
		InfoPage01, 10, 85, GetWidth() - 10, 150 );
	AddSpecialStatic( 1,
		InfoPage01a, 10, 160, GetWidth() - 10, 205 );
	AddSpecialStatic( 2,
		InfoPage01b, 10, 225, GetWidth() - 10, 340 );
	AddColorButton( IDC_STANDARD_OK, L"OK", 310, 506, BUTTON_HEIGH, 30, 13, true );

	// Declare title in this method
	AddTitle( L"Bryk" );
}

void CHelpDialog::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch (nControlID)
	{
	case IDC_STANDARD_OK :
		{
			Show( false );
			break;
		}
	}
}

/*CSettingsDialog::CSettingsDialog()
{
	AnimateChange.SetLerpParams( 1.0f, 0.0f, 1.0f );
	SetVisible( false );
	SetSize( 682, 330 );
	iTextureID = 2;
	const int top = 70;
	const int Y = 280;

	AddIconicButton(0, L"clockface01", EXT_NEXT_VAL, 19, top, 200, 200, 0, 0, 1023, 1023 );
	AddIconicButton(1, L"clockface02", EXT_NEXT_VAL, 239, top, 200, 200, 0, 0, 1023, 1023 );
	AddIconicButton(2, L"clockface03", EXT_NEXT_VAL, 459, top, 200, 200, 0, 0, 1023, 1023 );
	AddIconicButton(2, L"clockface04", EXT_NEXT_VAL, 459, top, 200, 200, 0, 0, 1023, 1023 );
	AddStatic(3, L"Tarcza £atwa", 19, Y, 200, 30 );
	AddStatic(4, L"Tarcza Œrednia", 239, Y, 200, 30 );
	AddStatic(5, L"Tarcza Trudna", 459, Y, 200, 30 );
	CenterWindow();

	// Declare title in this method
	AddTitle( L"Ustawienia" );
}

void CSettingsDialog::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	AnimateChange.Flush();
	iPreviousTextureID = iTextureID;
	iTextureID = nControlID;
	Show( false );
	AnimateChange.Start();
}

int CSettingsDialog::GetTextureID()
{
	return iTextureID;
}

int CSettingsDialog::GetPreviousTextureID()
{
	return iPreviousTextureID;
}

bool CSettingsDialog::CAnimateChange::IsAnimated()
{
	return !IsNormal();
}

bool CSettingsDialog::CAnimateChange::IsNormal()
{
	return m_startTime <= 0.0f;
}*/

/*CPaneForChild::CPaneForChild()
{
	SetVisible( false );

	SpeechDobrze.AddString( L"dobrze" );
	SpeechDobrze.AddString( L"dobrze1" );
	SpeechDobrze.AddString( L"dobrze2" );

	SpeechNiestety.AddString( L"niestety" );
	SpeechNiestety.AddString( L"niestety1" );

	// Circullar shape of button positions

	const int PaneSide = 677;
	SetLocation( 0, 0 );
	SetSize( PaneSide, PaneSide );
	const int side = 53;
	for( int iButton = 1; 12 >= iButton; iButton ++ )
		AddColorButton( iButton, NULL, 16, 16, side << 1, side << 1 );

	MonadMath::CEllipse cEllipse( 
		12, 
		0.33f * (float) PaneSide,
		0.32f * (float) PaneSide);


	for( int iButton = 1; 12 >= iButton; iButton ++ )
	{
		CDXUTExtColorButton * button = GetColorButton( iButton );
		float X = cEllipse[ (12 - iButton + 3 ) % 12 ].x;
		float Y = cEllipse[ (12 - iButton + 3 ) % 12 ].y;
		float relX;
		float relY;
		relX = relY	= (float) ( PaneSide >> 1 );
		switch( iButton )
		{
		case 10 :
			relX += side * .3f;
			relY += side * .3f;
			break;
			//		case 11 :
			/*default:;
			relX += X / abs(Y) * side - side;
			relY += Y / abs(X) * side - side;* /
		}

		relX += X - side;
		relY += Y - side;
		button->SetLocation(
			(int) relX,
			(int) relY );
	}
}

void CPaneForChild::ChangeButtonsColors()
{
	for( int iButton = 1; 12 >= iButton; iButton ++ )
	{
		CDXUTExtColorButton * button = GetColorButton( iButton );

		unsigned int ir = rand() % 255 + 1;
		unsigned int ig = rand() % 255 + 1;
		unsigned int ib = rand() % 255 + 1;

		button->SetBackgroundColor( 
			D3DCOLOR_ARGB(
			255,
			ir,
			ig,
			ib ) );
	}
	RandomHour = GetNewRandomHour12();
	static WCHAR * PolishHours [] =
	{ L"pierwsza",
	L"druga",
	L"trzecia",
	L"czwarta",
	L"piata",
	L"szosta",
	L"siodma",
	L"osma",
	L"dziewiata",
	L"dziesiata",
	L"jedenasta",
	L"dwunasta" };
	SetVisible( true );
	MonadAudio::EnqueueVoice( PolishHours[ RandomHour - 1 ] );
}

void CPaneForChild::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	MonadAudio::ResetQueue();

	if( RandomHour == nControlID )
	{
		g_aktual_czas.wHour = LOWORD( nControlID );
		OnOK();
	}
	else
	{
		MonadAudio::EnqueueVoice( SpeechNiestety );
		WaitForSpeech.SetLerpParams( 2.7f, 0.0f, M_PI );
		WaitForSpeech.Start();
	}

}

void CPaneForChild::OnOK()
{
	SetVisible( false );
	MonadAudio::EnqueueVoice( SpeechDobrze );
	WaitForSpeech.SetLerpParams( 3.7f, 0.0f, M_PI );
	WaitForSpeech.Start();
}

void CPaneForChild::CWaitForSpeech::OnFrameMove( double & fTime )
{
	light = XMVectorLerp(
		lightSepia,
		IsOK() ? lightGreen : lightRed,
		sinf( GetCurrentValue( (float) fTime ) ) );
}

void CPaneForChild::CWaitForSpeech::OnStop()
{
	if( IsOK() )
		Instance->PaneForChild.ChangeButtonsColors();
	// Rollback light game
	light = lightSepia;
}* /


CDisclaimerDialog::CDisclaimerDialog()
{
	CHelpDialog::OnCreateDevice();

	GetSpecialStatic( 0 )->SetText( 
		L"Wygaszacz ekranu Stary Polski Zegar 3 Alfa\n"
		L"Narzêdzie jest w fazie rozwojowej (Beta)." 
		L"Ta wersja jest w³asnoœci¹ twórców." );

	GetSpecialStatic( 1 )->SetText( 
		L"U¿yte zosta³y nastêpuj¹ce technologie\n"
		L"DirectX, TTS Ivona, TTS Ann w Vista\n"
		L"oraz w³asny Monad Engine 2.0" );

	GetSpecialStatic( 2 )->SetText( 
		L"Aplikacja ta jest chroniona Prawem Autorskim\n"
		L"oraz Prawem Copyright.\n"
		L"\"Stary Polski Zegar\" nie zosta³ w tej wersji wydany\n"
		L"i nie mo¿e byæ rozpowszechniany, u¿ytkowany lub kopiowany\n"
		L"bez zgody autorów" );

	//AddTitle( L"Info o Programie" );
}*/