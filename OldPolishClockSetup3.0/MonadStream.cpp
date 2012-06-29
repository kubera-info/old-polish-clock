#include "DXUT.h"
#include "MonadStream.h"
#include "I18N.h"

using namespace std;

ostream & operator<< ( ostream &Out, const WCHAR * SrcVer )
{
	std::wstring Converter( SrcVer );
	Out << Converter;

	return Out;
}

ostream & operator<< ( ostream &Out, const std::wstring &SrcVer )
{
	Out << MonadI18N::UnicodeToAnsi( SrcVer );

	return Out;
}

static void LogWithPadding( char pSeparator, int pIntVal, ostream & pOutStream )
{
	pOutStream << pSeparator;
	if( 10 > pIntVal )
		pOutStream << '0';
	pOutStream << pIntVal;
}

ostream & operator<< ( ostream &Out, const CTime &SrcVer )
{
	Out << SrcVer.GetYear();
	LogWithPadding( '-', SrcVer.GetMonth(), Out );
	LogWithPadding( '-', SrcVer.GetDay(), Out );
	LogWithPadding( 'T', SrcVer.GetHour(), Out );
	LogWithPadding( ':', SrcVer.GetMinute(), Out );
	LogWithPadding( ':', SrcVer.GetSecond(), Out );

	return Out;
}