#pragma once
#ifndef _MONAD_SYSTEM_H_
#define _MONAD_SYSTEM_H_

#include <string>

std::wstring GetApplicationVersion();

struct CMonadWindowsVersion
{
	bool IsWindows_XP_SP2() const throw();
	bool IsWindows_Vista_SP2() const throw();
	bool IsWindows_7() const throw();
	bool IsWindows_8() const throw();
	//bool IsSSE() const throw();

private:
	bool TestVer(
		const DWORD dwMajorVersion,
		const DWORD dwMinorVersion,
		const WORD wServicePackMajor,
		const WORD wServicePackMinor ) const throw();
};

#endif
