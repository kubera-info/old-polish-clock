#include "DXUT.h"
#include "MonadException.h"
#include "MonadCOM.h"

CCoInitialize::CCoInitialize() throw(...)
{
	THROW_EXC_IFFAILED( MonadException::COMInitialization, CoInitializeEx( NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE ), L"COM initialization" );
}
CCoInitialize::~CCoInitialize() throw()
{
	CoUninitialize();
}
