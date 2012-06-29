#include "DXUT.h"
#include <boost/algorithm/string.hpp>
#include "MonadParameter.h"

bool IsParameter( const std::wstring & pParam ) throw()
{
	for( int i = 1; i < ::__argc; i ++ )
		if( boost::iequals( pParam, ::__wargv[ i ] ) )
			return true;

	return false;
}