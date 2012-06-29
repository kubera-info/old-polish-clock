//-----------------------------------------------------------------------------
// File: D3DSaver.cpp
//
// Desc: Framework for screensavers that use Direct3D 8.0.
//
// Copyright (c) 2000-2001 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "DXUT.h"
#include "D3DSaver.h"
#include "MonadConvert.h"

HWND GetWindow( LPCWSTR pstrCommandLine )
{
	pstrCommandLine++;
	while ( *pstrCommandLine && !isdigit(*pstrCommandLine) )
		pstrCommandLine++;
	if ( 0 != *pstrCommandLine )
		return ( HWND ) ULongPtrFromString( pstrCommandLine );
	else
		return NULL;
}
//-----------------------------------------------------------------------------
// Name: ParseCommandLine()
// Desc: Interpret command-line parameters passed to this app.
//-----------------------------------------------------------------------------
SaverParams ParseCommandLine()
{
	LPCWSTR pstrCommandLine = GetCommandLine();

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
	switch ( toupper( *(++pstrCommandLine) ) )
	{
	case 'C':
		return SaverParams( sm_config, GetWindow( pstrCommandLine ) );

	case 'T':
		return sm_test;

	case 'P':
		// Preview-mode, so option is followed by the parent HWND in decimal
		return SaverParams( sm_preview, GetWindow( pstrCommandLine ) );

	case 'A':
		// Password change mode, so option is followed by parent HWND in decimal
		return sm_error;

	default:
		// All other options => run the screensaver (typically this is "/s")
		return sm_full;
	}
}

const SaverParams & GetSaverParams()
{
	static const SaverParams g_SaverMode = ParseCommandLine();
	return g_SaverMode;
}