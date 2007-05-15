//-----------------------------------------------------------------------------
// File: D3DSaver.h
//
// Desc: Framework for screensavers that use Direct3D 8.0.
//
// Copyright (c) 2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef _D3DSAVER_H
#define _D3DSAVER_H


//***************************************************************************************
// Modes of operation for screensaver
enum SaverMode
{
    sm_config,         // Config dialog box
    sm_preview,        // Mini preview window in Display Properties dialog
    sm_full,           // Full-on screensaver mode
    sm_test,           // Test mode
    sm_passwordchange  // Change password
};

extern HWND g_hWndParent;
SaverMode ParseCommandLine( LPWSTR pstrCommandLine );

#endif
