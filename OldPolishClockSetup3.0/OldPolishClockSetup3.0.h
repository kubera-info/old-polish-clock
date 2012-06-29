
// OldPolishClockSetup3.0.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// COldPolishClockSetup30App:
// See OldPolishClockSetup3.0.cpp for the implementation of this class
//

class COldPolishClockSetup30App : public CWinApp
{
public:
	//COldPolishClockSetup30App();

// Overrides
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	//DECLARE_MESSAGE_MAP()
};

extern COldPolishClockSetup30App theApp;
