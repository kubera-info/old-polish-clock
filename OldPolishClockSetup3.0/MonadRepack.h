#pragma once
#ifndef _MONAD_REPACK_H_
#define _MONAD_REPACK_H_

#include "MonadThreadBuilder.h"

struct CRepacker : public CMonadThread
{
	CRepacker( const std::wstring & pSubPath );

protected:
	virtual unsigned Routine();
	unsigned ProcessFile( const WIN32_FIND_DATA & pFindFileData );

private:	
	const std::wstring mSubPath;
};
#endif // _MONAD_REPACK_H_