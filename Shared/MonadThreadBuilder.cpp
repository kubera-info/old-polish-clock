#include "DXUT.h"
#include "MonadThreadBuilder.h"
#include <process.h>
#include "I18N.h"

CMonadThread::CMonadThread() throw()
: hThread( NULL ), dwThreadId( 0 )
{
}
void CMonadThread::Run() throw()
{
	const uintptr_t mThread = _beginthreadex( 
		NULL,
		0,
		&Wrapper,
		( void *) this,
		NULL,
		reinterpret_cast< unsigned int *> ( &dwThreadId )
		);
	if( -1l != mThread )
		hThread = ( HANDLE ) mThread;
	else
	{
#ifndef MONAD_ENGINE
		clog << "Can not create a thread." << endl;
#endif
	}
}

unsigned __stdcall CMonadThread::Wrapper( void* lpThreadParameter ) throw()
{
	try
	{
		CMonadThread * pThreadClass( reinterpret_cast< CMonadThread * > ( lpThreadParameter ) );
		assert( pThreadClass );
		MonadI18N::SetIULocale();
		unsigned res ( pThreadClass->Routine() );
#ifndef MONAD_ENGINE
		if( ERROR_SUCCESS != res )
			clog << "A thread's routine has returned with an error." << endl;
#endif
		return res;
	}
	catch( unsigned res )
	{
#ifndef MONAD_ENGINE
		clog << "An exception has been thrown inside a thread routine." << endl;
#endif
		return res;
	}
	catch( ... )
	{
#ifndef MONAD_ENGINE
		clog << "An unknown exception has been thrown inside a thread routine." << endl;
#endif
		return EXIT_FAILURE;
	}
}

DWORD CMonadThread::ExitCode() const throw()
{
	if( NULL == hThread )
		return EXIT_FAILURE;

	DWORD mExitThread( ERROR_SUCCESS );
	WaitForSingleObject( hThread, INFINITE );
	if( GetExitCodeThread(
		hThread,
		&mExitThread ) )
	{
#ifndef MONAD_ENGINE
		if( ERROR_SUCCESS != mExitThread )
			clog << "Thread's exit code: STATUS(" << mExitThread << ")." << endl;
#endif
		return mExitThread;
	}
	else
	{
#ifndef MONAD_ENGINE
		clog << "No thread's exit code." << endl;
#endif
		return EXIT_FAILURE;
	}
}

CMonadThread::~CMonadThread() 
{
	WaitForSingleObject( hThread, INFINITE );
	CloseHandle( hThread );
};