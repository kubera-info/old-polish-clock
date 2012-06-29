#pragma once
#ifndef _MONAD_FILE_TIMESTAMP_H_
#define _MONAD_FILE_TIMESTAMP_H_

#include <string>
#include <atlfile.h>

void MonadSetFileTime( const std::wstring & pFileName );
void MonadSetFileTime( CAtlFile & pAtlFile );

#endif // _MONAD_FILE_TIMESTAMP_H_