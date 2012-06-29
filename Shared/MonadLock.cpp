#include "DXUT.h"
#include "MonadLock.h"

namespace MonadThread
{
	inline void MonadReleaseMutex( CRITICAL_SECTION * pCS )
	{
		DeleteCriticalSection( pCS );
		delete( pCS );
	}
	//
	// Mutex
	//
	CMutex::CMutex()
		: mCS( new CRITICAL_SECTION(), MonadReleaseMutex )
#ifdef DEBUG
		, mId( new DWORD( 0 ) )
#endif
	{
		if( !InitializeCriticalSectionAndSpinCount( mCS.get(), 0x80000400 ) ) 
			throw std::bad_alloc( "Bad alloc: Mutex" );
	}
	void CMutex::Enter()
	{
		EnterCriticalSection( mCS.get() );
#ifdef DEBUG
		assert( 0 == *mId.get() );
		*mId.get() = GetCurrentThreadId();
#endif
	}
	void CMutex::Leave()
	{
#ifdef DEBUG
		assert( 0 != *mId.get() );
		*mId.get() = 0;
#endif
		LeaveCriticalSection( mCS.get() );
	}
	CLock::CLock( CMutex & Source ) : mMutex( &Source )
	{
		mMutex->Enter();
	}
	CLock::~CLock()
	{
		mMutex->Leave();
	}
}