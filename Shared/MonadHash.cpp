#include "DXUT.h"
#include "MonadHash.h"

ULONG HashString( const std::wstring & pStrForHash )
{
	return HashFromMemory( ( const char * ) pStrForHash.c_str(), pStrForHash.length() * sizeof( WCHAR ) );
}

ULONG HashFromFileHolder( const std::wstring & pFilename, const std::wstring & pExt, const char * pBuffer, const size_t pFileSize )
{
	const ULONG ulKey( HashString( pFilename ) );
	const ULONG ulExt( HashString( pExt ) );
	const ULONG ulBody( HashFromMemory( pBuffer, pFileSize ) );
	return ulKey + ulExt + ulBody + MonadFileVersion;
}
ULONG HashFromMemory( const char * pAddrOfVar, const size_t pSizeOfVar )
{
	union unionResult
	{
		unionResult()
		{
			Hash = 0L;
		}
		ULONG Hash;
		BYTE AsBuffer[ sizeof( ULONG ) ];
	} register Result;

	for( size_t pos = 0; pos < pSizeOfVar; pos ++ )
		Result.AsBuffer[ ( pos % sizeof( ULONG ) ) ] += ( pAddrOfVar[ pos ] + ( byte ) pos );

	return Result.Hash;
}
