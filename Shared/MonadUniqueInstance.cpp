#include "DXUT.h"
#include "MonadUniqueInstance.h"

CAmIUnique::CAmIUnique( LPCWSTR pId )
: m_hMyMutex( CreateMutex( NULL, FALSE, pId ) )
{
	const DWORD res = GetLastError();
	if( ERROR_ALREADY_EXISTS == res )
	{
		CloseHandle( m_hMyMutex );
		m_hMyMutex = NULL;
	}
	else if( ERROR_SUCCESS != res )
		throw res;
}
void CAmIUnique::InvalidateUnique()
{
	CloseHandle( m_hMyMutex );
	m_hMyMutex = NULL;
}
CAmIUnique::~CAmIUnique() throw()
{
	InvalidateUnique();
}
CAmIUnique::operator bool () const throw()
{
	return NULL != m_hMyMutex;
}
