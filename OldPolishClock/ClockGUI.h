// ###################################
// ClockGUI.h - the Application GUI
// ###################################

#pragma once
#ifndef _CLOCK_GUI_H_
#define _CLOCK_GUI_H_

#include "ExtDXUTgui.h"
#include "MonadAudio.h"
#include "Light.h"

/*
extern bool IsWM_CLOSE; // Disable an Exit Dialog, and stop the application?

HRESULT AddSemanticColorsToManager();

void CALLBACK OnGUIEventExit( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

class CLeftToolbar : public CDXUTExtWindow
{
public:
	CLeftToolbar();
	HRESULT OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
	virtual void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

protected:
	HRESULT AddMenuButton( int ID, int & CurrentYPos, LPCWSTR strText );
};

class CAboutDlg : public CDXUTExtDialog
{
public:
	CAboutDlg();
	virtual void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
};

class CMessageBox : public CDXUTExtDialog
{
public:
	CMessageBox();
	void SetupBox( LPCWSTR strText, LPCWSTR strTitle, PCALLBACKDXUTGUIEVENT pCallback );
};

class CHelpDialog : public CDXUTExtDialog
{
public:
	CHelpDialog();
	virtual void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
};

class CDisclaimerDialog : public CHelpDialog
{
public:
	CDisclaimerDialog();
};

/*class CSettingsDialog : public CDXUTExtDialog
{
public:
	CSettingsDialog();
	virtual void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

	int GetTextureID();
	int GetPreviousTextureID();

	class CAnimateChange : public CTimeLerp
	{
	public:
		bool IsNormal();
		bool IsAnimated();
	} AnimateChange;
	int iTextureID, iPreviousTextureID;
};*/

// Transparent pane with coloured buttons on front of the clock
//
/*class CPaneForChild : public CDXUTExtWindow
{
public:
	CPaneForChild();
	virtual void OnBackgroundColors( D3DCOLOR & c1, D3DCOLOR & c2, D3DCOLOR & c3, D3DCOLOR & c4 )
	{
		c1 = c2 = c3 = c4 = D3DCOLOR_ARGB( 0, 0, 0, 0 );
	}
	void ChangeButtonsColors();
	virtual void OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

	// New round
	virtual void OnOK();

	// Flush pane from the screen
	virtual void OnFlush()
	{
		WaitForSpeech.Flush();
		SetVisible( false );
	}
protected:
	LONG_PTR RandomHour;

	// Give Jacek chanse to finish speaking
	class CWaitForSpeech : public CTimeLerp
	{
	public:
		virtual void OnFrameMove( double & fTime );
		virtual void OnFlush()
		{
			// Rollback light game
			light = lightSepia;
		}

		virtual void OnStop();

		bool IsOK()
		{
			return GetTimeInSeconds() == 3.7f;
		}
	} WaitForSpeech;

	CRandomString SpeechDobrze, SpeechNiestety;
};* /

struct StructInstance : public CDestructable
{
	// CLeftToolbar LeftToolbar;
	//CSettingsDialog SettingsDialog;
	CAboutDlg About;
	CHelpDialog HelpDialog;
	// CPaneForChild PaneForChild;
	CMessageBox MsgBox, ExitMsgBox;
	CDisclaimerDialog DisclaimerDialog;
};

extern CMonadAutoPtr<StructInstance> Instance;
*/

#endif