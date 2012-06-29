#include "DXUT.h"
#include "MonadExternalUI.h"
#include "Dict.h"
#include "Resource.h"

//
//  FUNCTION: TestMyBasicUIHandler()
//
//  PURPOSE: Demonstrates usage of an external user interface handler for MSI
//
//  COMMENTS:
//

static BOOL bFirstTime = TRUE;

int _stdcall MonadUIHandler(LPVOID pvContext, UINT iMessageType, LPCTSTR szMessage)
{
	pvContext;

	// File costing is skipped when applying Patch(es) and INSTALLUILEVEL is NONE.
	// Workaround: Set INSTALLUILEVEL to anything but NONE only once.
	if( bFirstTime == TRUE )
	{
		/*UINT r1 =*/ MsiSetInternalUI( INSTALLUILEVEL_BASIC, NULL);
		bFirstTime = FALSE;
	}

	if (!szMessage)
		return 0;

	INSTALLMESSAGE mt;
	UINT uiFlags;

	mt = (INSTALLMESSAGE)(0xFF000000 & (UINT)iMessageType);
	uiFlags = 0x00FFFFFF & iMessageType;

	switch (mt)
	{
		//Premature termination
	case INSTALLMESSAGE_FATALEXIT:
		GlobalErrorMessage( szMessage );
		return 0;

	case INSTALLMESSAGE_ERROR:
		GlobalErrorMessage( szMessage );
		return 0;

	case INSTALLMESSAGE_WARNING:
		/* Get warning message here and display it */
		return MonadUIMessageBox( szMessage, uiFlags );

	case INSTALLMESSAGE_USER:
		/* Get user message here */
		// parse uiFlags to get Message Box Styles Flag and return appopriate value, IDOK, IDYES, etc.
		return MonadUIMessageBox( szMessage, uiFlags );

	case INSTALLMESSAGE_INFO:
		return IDOK;

	case INSTALLMESSAGE_FILESINUSE:
		return MonadUIMessageBox( szMessage, uiFlags );

	case INSTALLMESSAGE_RESOLVESOURCE:
		/* ALWAYS return 0 for ResolveSource */
		return 0;

	case INSTALLMESSAGE_OUTOFDISKSPACE:
		return MonadUIMessageBox( szMessage, uiFlags );

		/*case INSTALLMESSAGE_ACTIONSTART:
		/* New action started, any action data is sent by this new action * /
		g_bEnableActionData = FALSE;
		return IDOK;

		/*case INSTALLMESSAGE_ACTIONDATA:
		// only act if progress total has been initialized
		if (0 == g_iProgressTotal)
		return IDOK;
		SetDlgItemText(/ *handle to your dialog* /,/*identifier of your actiontext control* /, szMessage);
		if(g_bEnableActionData)
		{
		SendMessage(/*handle to your progress control* /,PBM_STEPIT,0,0);
		}
		return IDOK;*/

	case INSTALLMESSAGE_PROGRESS:
		{
			/*if(ParseProgressString(const_cast<LPSTR>(szMessage)))
			{
			// all fields off by 1 due to c array notation
			switch(iField[0])
			{
			case 0: // Reset progress bar
			{
			//field 1 = 0, field 2 = total number of ticks, field 3 = direction, field 4 = in progress

			/* get total number of ticks in progress bar * /
			g_iProgressTotal = iField[1];

			/* determine direction * /
			if (iField[2] == 0)
			g_bForwardProgress = TRUE;
			else // iField[2] == 1
			g_bForwardProgress = FALSE;

			/* get current position of progress bar, depends on direction * /
			// if Forward direction, current position is 0
			// if Backward direction, current position is Total # ticks
			g_iProgress = g_bForwardProgress ? 0 : g_iProgressTotal;
			SendMessage(/*handle to your progress control* /, PBM_SETRANGE32, 0, g_iProgressTotal);

			// if g_bScriptInProgress, finish progress bar, else reset (and set up according to direction)
			SendMessage(/*handle to your progress control* /, PBM_SETPOS, g_bScriptInProgress ? g_iProgressTotal : g_iProgress, 0);

			iCurPos = 0;

			/* determine new state * /
			// if new state = 1 (script in progress), could send a "Please wait..." msg
			// new state = 1 means the total # of progress ticks is an estimate, and may not add up correctly
			g_bScriptInProgress = (iField[3] == 1) ? TRUE : FALSE;

			break;
			}* /
			case 1:  // ActionInfo
			{
			//field 1 = 1, field 2 will contain the number of ticks to increment the bar
			//ignore if field 3 is zero
			if(iField[2])
			{
			// movement direction determined by g_bForwardProgress set by reset progress msg
			SendMessage(/*handle to your progress control* /, PBM_SETSTEP, g_bForwardProgress ? iField[1] : -1*iField[1], 0);
			g_bEnableActionData = TRUE;
			}
			else
			{
			g_bEnableActionData = FALSE;
			}

			break;
			}
			case 2: //ProgressReport
			{
			// only act if progress total has been initialized
			if (0 == g_iProgressTotal)
			break;

			iCurPos += iField[1];

			//field 1 = 2,field 2 will contain the number of ticks the bar has moved
			// movement direction determined by g_bForwardProgress set by reset progress msg
			SendMessage(/*handle to your progress control* /, PBM_SETPOS, g_bForwardProgress ? iCurPos : -1*iCurPos, 0);

			break;
			}
			case 3: // ProgressAddition - fall through (we don't care to handle it -- total tick count adjustment)
			default:
			{
			break;
			}
			}
			}*/

			if( g_bCancelInstall )
			{
				return IDCANCEL;
			}
			else
				return IDOK;
		}


		/*case INSTALLMESSAGE_COMMONDATA:
		{
		if (ParseCommonDataString(const_cast<LPTSTR>(szMessage)))
		{
		// all fields off by 1 due to c array notation
		switch (g_rgiField[0])
		{
		case 0:
		// field 1 = 0, field 2 = LANGID, field 3 = CodePage
		g_wLANGID = g_rgiField[1];
		break;
		case 1:
		// field 1 = 1, field 2 = CAPTION
		/* you could use this as the caption for MessageBoxes * /
		break;
		case 2:
		// field 1 = 2, field 2 = 0 (hide cancel button) OR 1 (show cancel button)
		ShowWindow(/*handle to cancel button control on the progress indicator dialog box* /, g_rgiField[1] == 0 ? SW_HIDE : SW_SHOW);
		break;
		default:
		break;
		}
		}
		return IDOK;
		}*/

		// this message is received prior to internal UI initialization, no string data
	case INSTALLMESSAGE_INITIALIZE:
		return IDOK;

		// Sent after UI termination, no string data
	case INSTALLMESSAGE_TERMINATE:
		return IDOK;

		//Sent prior to display of authored dialog or wizard
	case INSTALLMESSAGE_SHOWDIALOG:
		return IDOK;

	default:
		return 0;
	}
}