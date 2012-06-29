//-----------------------------------------------------------------------------
// File: D3DSaver.h
//
// Desc: Framework for screensavers that use Direct3D 8.0.
//
// Copyright (c) 2000 Microsoft Corporation. All rights reserved.
// Copyright (c) 2003-2012 W³odzimierz O. Kubera
//-----------------------------------------------------------------------------
#ifndef _D3DSAVER_H_
#define _D3DSAVER_H_

#include <windows.h>
#include <boost/utility.hpp>

//***************************************************************************************
// Modes of operation for screensaver
enum SaverMode
{
	sm_config,         // Config dialog box
	sm_preview,        // Mini preview window in Display Properties dialog
	sm_full,           // Full-on screensaver mode
	sm_test,           // Test mode
	sm_error		   // Change password - legacy
};

struct SaverParams : boost::noncopyable
{
	SaverParams( SaverMode pSaverMode, HWND pWndParent = NULL ) throw() : m_SaverMode( pSaverMode ), m_hWndParent( pWndParent ) {}
	bool IsFullScreen() const throw() { return sm_full == m_SaverMode || sm_test == m_SaverMode; }
	const SaverMode m_SaverMode;
	const HWND m_hWndParent;
};

const SaverParams & GetSaverParams();

#endif // _D3DSAVER_H_
