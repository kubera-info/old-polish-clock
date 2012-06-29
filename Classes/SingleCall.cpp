//#include "DXUT.h"
#include "SingleCall.h"

CAbstractSingleCall::CAbstractSingleCall( const bool pCalled ) throw()
{
	m_bCalled = pCalled;
}
CAbstractSingleCall::operator bool () throw()
{
	return WasCalled();
}
void CAbstractSingleCall::ResetCalledFlag() throw()
{
	m_bCalled = false;
}
bool CAbstractSingleCall::WasCalled() throw()
{
	if( m_bCalled )
		return true;
	else
	{
		m_bCalled = true;
		return false;
	}
}
bool CAbstractSingleCall::GetFlagUnchanged() const throw()
{
	return m_bCalled;
}
