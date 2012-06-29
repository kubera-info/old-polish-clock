#pragma once
#ifndef _MONAD_CATCH_H_
#define _MONAD_CATCH_H_

#include <exception>
#include <tchar.h>
#include <atlexcept.h>
#include "MonadException.h"

#define CATCH \
	catch( const std::bad_alloc & Exc ) \
	{ \
		Exc; \
		ExitProcess( EXIT_FAILURE ); \
	} \
	catch( const std::exception & Exc ) \
	{ \
		MonadException::CGenericMonadException tmpExc( TEXT(__FILE__) L"; " TEXT(__FUNCTION__), Exc.what() ); \
		MonadException::RegisterException( tmpExc ); \
	} \
	catch( const MonadException::CGenericMonadException & Exc ) \
	{ \
		MonadException::RegisterException( Exc ); \
	} \
	catch( const ATL::CAtlException & Exc ) \
	{ \
		MonadException::RegisterException( CRE_EXC( MonadException::ATLFailed, Exc, L"Library Exception" ) ); \
	} \
	catch( ... ) \
	{ \
		MonadException::RegisterException( CRE_EXC( MonadException::Unknown, E_FAIL, L"Unknown Exception" ) ); \
	}
#endif