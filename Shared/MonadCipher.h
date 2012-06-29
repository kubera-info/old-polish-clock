#pragma once
#ifndef _MONAD_CIPHER_H_
#define _MONAD_CIPHER_H_

HRESULT Decrypt( LPCBYTE bKey, const DWORD dwKeySize, const LPBYTE bEncryptedData, const DWORD dwDataSize );

#endif // _MONAD_CIPHER_H_
