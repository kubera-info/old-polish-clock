#include "DXUT.h"
#include "MonadLibrary.h"
#include "MonadFile.h"

CSystemLibrary::CSystemLibrary( const std::wstring & pLibFileName )
: hLib( LoadLibrary( ( MonadFile::GetSystemFolder() + L"\\" + pLibFileName ).c_str() ) )
{
}
/*CSystemLibrary::~CSystemLibrary()
{
	if( hLib )
		FreeLibrary( hLib );
}*/