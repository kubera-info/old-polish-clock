#include "MyEncrypt.h"
#include "stdafx.h"
#include <Wincrypt.h>

const static int EncryptTable [] = {WERSJAPLIKUNUM};

ResourceCrypt::ResourceCrypt(const INT p_ID)
{
	m_Buffer = NULL;
	m_BufferSize = 0;
	LPCVOID m_TmpBuf = ZaladujZasob(p_ID, m_BufferSize);
	if(m_TmpBuf)
	{
		m_Buffer = new CHAR [m_BufferSize];
		if(m_Buffer)
			CopyMemory((VOID *) m_Buffer, m_TmpBuf, m_BufferSize);
		else
			m_BufferSize = 0;
	}
	else
		m_BufferSize = 0;
	const INT TmpKey = NewSalt(p_ID);
	if(FAILED(Decrypt((LPBYTE) &(TmpKey), sizeof(DWORD))))
	{
		SAFE_DELETE(m_Buffer);
		m_BufferSize = 0;
	}
}

#if defined(_DEBUG)
ResourceCrypt::ResourceCrypt(const PCHAR pFileName,
							 const PCHAR pEncryptedFileName,
							 const INT p_ID)
{
	m_Buffer = NULL;
	m_BufferSize = 0;
	DWORD dummy;
	HANDLE fhread = CreateFileA(pFileName,    // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 
	HANDLE fhwrite = CreateFileA(pEncryptedFileName,    // file to open
		GENERIC_WRITE,          // open for reading
		FILE_SHARE_WRITE,       // share for reading
		NULL,                  // default security
		CREATE_ALWAYS,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 
	if(NULL == fhread)
		ExitProcess(DXTRACE_ERR( TEXT("Error opening file for reading"), E_FAIL));
	if(NULL == fhwrite)
		ExitProcess(DXTRACE_ERR( TEXT("Error opening file for writing"), E_FAIL));
	m_BufferSize = GetFileSize(fhread, &dummy);
	if (INVALID_FILE_SIZE == m_BufferSize && NULL == dummy)
		ExitProcess(DXTRACE_ERR( TEXT("Error getting file size"), E_FAIL));

	m_Buffer = new CHAR [m_BufferSize];
	if(m_Buffer == NULL)
		ExitProcess(DXTRACE_ERR( TEXT("Error allocating encryption buffer"), E_FAIL));

	DWORD EncReadTst;
	if(!ReadFile(
		fhread, //HANDLE hFile,
		(LPVOID) m_Buffer, //LPVOID lpBuffer,
		m_BufferSize, //DWORD nNumberOfBytesToRead,
		&EncReadTst, //LPDWORD lpNumberOfBytesRead,
		NULL //LPOVERLAPPED lpOverlapped
		))
		ExitProcess(DXTRACE_ERR( TEXT("Error reading file"), E_FAIL));

	if(dummy)
		ExitProcess(DXTRACE_ERR( TEXT("Encryption file to long"), E_FAIL));
	const INT TmpKey = NewSalt(p_ID);
	if(FAILED(Encrypt((LPBYTE) &(TmpKey), sizeof(DWORD))))
		ExitProcess(DXTRACE_ERR( TEXT("Error Encrypting File"), E_FAIL));

	//LONG dwPtr = SetFilePointer (fh, 0L, NULL, FILE_BEGIN); 

	//if (INVALID_SET_FILE_POINTER == dwPtr) // Test for failure
		//ExitProcess(DXTRACE_ERR( TEXT("Error seeking dump file"), E_FAIL));

	if(!WriteFile(
		fhwrite, //HANDLE hFile,
		m_Buffer, //LPVOID lpBuffer,
		m_BufferSize, //DWORD nNumberOfBytesToRead,
		&EncReadTst, //LPDWORD lpNumberOfBytesRead,
		NULL //LPOVERLAPPED lpOverlapped
		))
		ExitProcess(DXTRACE_ERR( TEXT("Error writing file"), E_FAIL));

	if(!CloseHandle(fhread))
		ExitProcess(DXTRACE_ERR( TEXT("Error closing file"), E_FAIL));
	if(!CloseHandle(fhwrite))
		ExitProcess(DXTRACE_ERR( TEXT("Error closing file"), E_FAIL));
}
#endif

ResourceCrypt::~ResourceCrypt()
{
	SAFE_DELETE(m_Buffer);
}
LPCVOID ResourceCrypt::GetBuffer()
{
	return m_Buffer;
}
DWORD ResourceCrypt::GetBufferSize()
{
	return m_BufferSize;
}
HRESULT ResourceCrypt::Encrypt(const LPBYTE bKey, const DWORD dwKeySize)
{
	if(m_Buffer == NULL || m_BufferSize == 0)
		return DXTRACE_ERR( TEXT("No plain text buffer"), E_FAIL);
	HRESULT hr;
	HCRYPTPROV hProv;
	HCRYPTKEY  hKey;
	HCRYPTHASH hHash;

	DWORD dwBuff = m_BufferSize;
	V_RETURN (!CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT));
	V_RETURN (!CryptCreateHash(hProv,CALG_MD5,0,0,&hHash));
	V_RETURN (!CryptHashData(hHash,bKey,dwKeySize, 0));
	V_RETURN (!CryptHashData(hHash,(LPBYTE)EncryptTable,sizeof(EncryptTable), 0));
	V_RETURN (!CryptDeriveKey(hProv,CALG_RC4,hHash,CRYPT_EXPORTABLE,&hKey));
	V_RETURN (!CryptEncrypt(hKey,0,TRUE,0,(LPBYTE) m_Buffer,&dwBuff,m_BufferSize));

	if (hKey)  CryptDestroyKey(hKey);
	if (hHash) CryptDestroyHash(hHash);
	if (hProv) CryptReleaseContext(hProv,0);

	return S_OK;
}
HRESULT ResourceCrypt::Decrypt(const LPBYTE bKey, const DWORD dwKeySize)
{
	if(m_Buffer == NULL || m_BufferSize == 0)
		return DXTRACE_ERR( TEXT("No encrypted text buffer"), E_FAIL);

	HRESULT hr;
	HCRYPTPROV hProv;
	HCRYPTKEY  hKey;
	HCRYPTHASH hHash;

	DWORD dwBuff = m_BufferSize;
	V_RETURN (!CryptAcquireContext(&hProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT));
	V_RETURN (!CryptCreateHash(hProv,CALG_MD5,0,0,&hHash));
	V_RETURN (!CryptHashData(hHash,bKey,dwKeySize, 0));
	V_RETURN (!CryptHashData(hHash,(LPBYTE)EncryptTable,sizeof(EncryptTable), 0));
	V_RETURN (!CryptDeriveKey(hProv,CALG_RC4,hHash,CRYPT_EXPORTABLE,&hKey));
	V_RETURN (!CryptDecrypt(hKey,0,TRUE,0,(LPBYTE) m_Buffer,&m_BufferSize));

	if (hKey)  CryptDestroyKey(hKey);
	if (hHash) CryptDestroyHash(hHash);
	if (hProv) CryptReleaseContext(hProv,0);

	return S_OK;
}
int ResourceCrypt::NewSalt(INT pIn) // Obfuscator
{
	for(BYTE i = 132; i; i += 2)
		pIn += (i + EncryptTable[0] + EncryptTable[((i >> 1) + EncryptTable[2] * 2 - EncryptTable[3] * 7) % (sizeof(EncryptTable) / sizeof(int))]);
	return (pIn * pIn * pIn + 1) % salt;
}
