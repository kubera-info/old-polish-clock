#include "DXUT.h"
#include "MonadException.h"
#include "MonadThread.h"
#include "MonadTemplates.h"

void ThrowNoDataFoundInColl1()
{
	THROW_EXC( MonadException::NoDataFound, E_FAIL, L"Collection element does not exists" );
}