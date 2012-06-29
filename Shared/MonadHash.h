#pragma once
#ifndef _MONAD_HASH_H_
#define _MONAD_HASH_H_

#include <string>

// MonadFileVersion alters the check sum for being compatible only with the one version of engine
enum
{
	MonadFileVersion = 2
};

ULONG HashString( const std::wstring & pStrForHash );

ULONG HashFromFileHolder( const std::wstring & pFilename, const std::wstring & pExt, const char * pBuffer, const size_t pFileSize );

ULONG HashFromMemory( const char * pAddrOfVar, const size_t pSizeOfVar );

#endif