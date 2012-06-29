#pragma once
#ifndef _MONAD_RESOURCE_H_
#define _MONAD_RESOURCE_H_

#include <atlbase.h>

class CTemporaryFolder sealed
{
public:
	CTemporaryFolder();
	~CTemporaryFolder();
	void Purge() const;
	operator const std::wstring & () const throw();
	static const std::wstring & FileNameSuffix();
	static const std::wstring GetUserTemporaryFolder();
	std::wstring GetFullPath( const std::wstring & pFilename ) const;

private:
	std::wstring CreateTmpFolder();
	const std::wstring m_szTmpFolder;
	volatile mutable bool bPurged;
};
extern CAutoPtr<const CTemporaryFolder> gTmp;

class CBlob sealed
{
public:
	CBlob( CONST INT pResID, LPCWSTR pFilename ) throw();
	void Validate() const;

private:
	HANDLE hRarProcess;
	const std::wstring szPath;
};

HANDLE ExecAppHandle( const std::wstring & pApp, const std::wstring & pParams );
DWORD ExitApp( HANDLE pAppHadnle, const std::wstring & pApp = L"" );
DWORD ExecApp( const std::wstring & pPath, const std::wstring & pParams );

#endif // _MONAD_RESOURCE_H_