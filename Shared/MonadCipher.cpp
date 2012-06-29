#include "DXUT.h"
#include "MonadCipher.h"
#include <Wincrypt.h>
#include <string>

const char g_szSalt [] = "8ca6116e-61a2-4036-819b-48872f25bd10";

HRESULT Decrypt( LPCBYTE bKey, const DWORD dwKeySize, const LPBYTE bEncryptedData, const DWORD dwDataSize )
{
	HRESULT hr;
	HCRYPTPROV hProv;
	HCRYPTKEY  hKey;
	HCRYPTHASH hHash;

	DWORD dwBuff = dwDataSize;

	V (!CryptAcquireContext( &hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT ) );
	if( SUCCEEDED( hr ) )
	{
		V (!CryptCreateHash( hProv, CALG_SHA, 0, 0, &hHash ) );
		if( SUCCEEDED( hr ) )
		{
			V (!CryptHashData( hHash, bKey, dwKeySize, 0 ) );
			if( SUCCEEDED( hr ) )
			{
				V (!CryptHashData( hHash, reinterpret_cast<LPCBYTE> ( g_szSalt ), sizeof( g_szSalt ), 0 ) );
				if( SUCCEEDED( hr ) )
				{
					V (!CryptDeriveKey( hProv, CALG_RC4, hHash, CRYPT_EXPORTABLE, &hKey ) );
					if( SUCCEEDED( hr ) )
					{
						V (!CryptDecrypt( hKey, 0, TRUE, 0, bEncryptedData, &dwBuff ) );
						CryptDestroyKey( hKey );
					}
				}

			}
			CryptDestroyHash( hHash );
		}
		CryptReleaseContext( hProv, 0 );
	}

	return hr;
}