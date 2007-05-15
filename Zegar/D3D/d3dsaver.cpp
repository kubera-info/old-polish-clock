//-----------------------------------------------------------------------------
// File: D3DSaver.cpp
//
// Desc: Framework for screensavers that use Direct3D 8.0.
//
// Copyright (c) 2000-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "D3DSaver.h"

HWND g_hWndParent;

//-----------------------------------------------------------------------------
// Name: ParseCommandLine()
// Desc: Interpret command-line parameters passed to this app.
//-----------------------------------------------------------------------------
SaverMode ParseCommandLine( LPWSTR pstrCommandLine )
{
    g_hWndParent = NULL;

    // Skip the first part of the command line, which is the full path 
    // to the exe.  If it contains spaces, it will be contained in quotes.
    if (*pstrCommandLine == L'\"')
    {
        pstrCommandLine++;
        while (*pstrCommandLine != L'\0' && *pstrCommandLine != L'\"')
            pstrCommandLine++;
        if( *pstrCommandLine == L'\"' )
            pstrCommandLine++;
    }
    else
    {
        while (*pstrCommandLine != L'\0' && *pstrCommandLine != L' ')
            pstrCommandLine++;
        if( *pstrCommandLine == L' ' )
            pstrCommandLine++;
    }

    // Skip along to the first option delimiter "/" or "-"
    while ( *pstrCommandLine != L'\0' && *pstrCommandLine != L'/' && *pstrCommandLine != L'-' )
        pstrCommandLine++;

    // If there wasn't one, then must be config mode
    if ( *pstrCommandLine == L'\0' )
        return sm_config;

    // Otherwise see what the option was
    switch ( *(++pstrCommandLine) )
    {
        case 'c':
        case 'C':
            pstrCommandLine++;
            while ( *pstrCommandLine && !isdigit(*pstrCommandLine) )
                pstrCommandLine++;
            if ( isdigit(*pstrCommandLine) )
            {
#ifdef _WIN64
//                CHAR strCommandLine[2048];
                //DXUtil_ConvertGenericStringToAnsi(strCommandLine, pstrCommandLine, 2048);
                g_hWndParent = HWND(_wtoi64(pstrCommandLine));
#else
                g_hWndParent = HWND(_wtol(pstrCommandLine));
#endif
            }
            else
            {
                g_hWndParent = NULL;
            }
            return sm_config;

        case 't':
        case 'T':
            return sm_test;

        case 'p':
        case 'P':
            // Preview-mode, so option is followed by the parent HWND in decimal
            pstrCommandLine++;
            while ( *pstrCommandLine && !isdigit(*pstrCommandLine) )
                pstrCommandLine++;
            if ( isdigit(*pstrCommandLine) )
            {
#ifdef _WIN64
//                CHAR strCommandLine[2048];
//                DXUtil_ConvertGenericStringToAnsi(strCommandLine, pstrCommandLine, 2048);
                g_hWndParent = HWND(_wtoi64(strCommandLine));
#else
                g_hWndParent = HWND(_wtol(pstrCommandLine));
#endif
            }
            return sm_preview;

        case 'a':
        case 'A':
            // Password change mode, so option is followed by parent HWND in decimal
            pstrCommandLine++;
            while ( *pstrCommandLine && !isdigit(*pstrCommandLine) )
                pstrCommandLine++;
            if ( isdigit(*pstrCommandLine) )
            {
#ifdef _WIN64
//                CHAR strCommandLine[2048];
                //DXUtil_ConvertGenericStringToAnsi(strCommandLine, pstrCommandLine, 2048);
                g_hWndParent = HWND(_wtoi64(strCommandLine));
#else
                g_hWndParent = HWND(_wtol(pstrCommandLine));
#endif
            }
            return sm_passwordchange;

        default:
            // All other options => run the screensaver (typically this is "/s")
            return sm_full;
    }
}


