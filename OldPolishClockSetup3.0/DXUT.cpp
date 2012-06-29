
// stdafx.cpp : source file that includes just the standard includes
// OldPolishClockSetup3.0.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "DXUT.h"
#include "I18N.h"
#include "Dict.h"
#include "MonadLog.h"

int MonadUIMessageBox( LPCTSTR lpText, UINT uType )
{
	return MonadI18N::MonadMessageBox( gHWND, lpText, GetSetupTitle().c_str(), uType );
}

BOOL GlobalErrorMessage( LPCWSTR pErrorMessage )
{
	static std::wstring Comment;
	std::wstring ToLog( pErrorMessage );
	Comment = UnknownError();
	Comment += L"\n\n";
	Comment += ToLog;
	clog << endl;
	QuotedPrintable( ToLog );
	clog << ToLog;
	clog << endl;
	return PostMessage( gHWND, WM_ERROR_MESSAGE, ( WPARAM ) Comment.c_str(), NULL );
}