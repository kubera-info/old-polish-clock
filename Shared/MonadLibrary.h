#pragma once
#ifndef _MONAD_LIBRARY_H_
#define _MONAD_LIBRARY_H_

struct CSystemLibrary
{
	CSystemLibrary( const std::wstring & pLibFileName );
	// ~CSystemLibrary() throw();

	FARPROC WINAPI GetProcAddress( __in LPCSTR lpProcName ) const throw()
	{
		return ::GetProcAddress( hLib, lpProcName );
	}

	operator const HMODULE () const throw() { return hLib; }

protected:
	HMODULE hLib;
};
#endif