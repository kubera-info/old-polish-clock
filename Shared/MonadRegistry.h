#pragma once
#ifndef _MONAD_REGISTRY_H_
#define _MONAD_REGISTRY_H_

#include <string>

struct CRegistry abstract
{
protected:
	CRegistry( HKEY phKey, const WCHAR * plpSubkey, DWORD pdwOptions );
	bool SetString( const WCHAR * pRegName, const std::wstring & pValue );
	bool SetDWORD( const WCHAR * pRegName, DWORD pValue );
	virtual ~CRegistry();

	HKEY m_hKey;
};

#endif _MONAD_REGISTRY_H_