// MonadThread.cpp : Defines the exported functions for the DLL application.
//

#include "DXUT.h"
#include <atlbase.h>
#include "SingleCall.h"
#include "..\..\Standard\MonadCatch.h"
#include "..\MonadException.h"
#include "..\MonadThread.h"
#include "..\..\Patterns\MonadTemplates.h"
#include "CpuTopology.h"

namespace MonadThread
{
	typedef CMonadCoherentQueue<IQue *> CRunnableQueue;
	static CRunnableQueue mRunnableQueue;

	CThreadPoolManager::CThreadPoolManager( const int pThreadsCnt )
	{
		THROW_EXC_IFFAILED( MonadException::OutOfMemory, m_ThreadPool.Initialize( NULL, pThreadsCnt ), "Can not create Thread Pool" );
	}
	CThreadPoolManager::~CThreadPoolManager()
	{
		m_ThreadPool.Shutdown( INFINITE );
	}
	void CThreadPoolManager::InsertTask( CRunnable * pTask )
	{
		THROW_EXC_IFZ( m_ThreadPool.QueueRequest( pTask ), MonadException::OutOfMemory, L"Insert task" );
	}

	BOOL CThreadPoolManager::CMonadWorker::Initialize(void* pvParam)
	{
		pvParam;

		return TRUE;
	}

	void CThreadPoolManager::CMonadWorker::Terminate(void* pvParam)
	{
		pvParam;
	}

	BOOL CThreadPoolManager::CMonadWorker::GetWorkerData(DWORD /*dwParam*/, void** /*ppvData*/)
	{
		return FALSE;
	}

	void CThreadPoolManager::CMonadWorker::Execute(RequestType	 dw, void*	pvParam, OVERLAPPED *pOverlapped) throw()
	{
		pvParam;
		pOverlapped;

		try
		{
			dw->Run();
		}
		CATCH;
		dw->NotifyMainThread();
	}

	// Info
	DWORD NumberOfProcessCores() throw()
	{
		class CpuTopology tmp;
		return tmp.NumberOfProcessCores();
	}
	DWORD GetDefaultThreadsNumber() throw()
	{
		const DWORD cores = MonadThread::NumberOfProcessCores();
		switch( cores )
		{
		case 1 : ;
		case 2 :
			return 2;
		case 3 : ;
		case 4 : ;
			return 4;
		default :
			return 7;
		}
	}

	void CRunnable::NotifyMainThread() throw()
	{
		// ;)
	}

	void CRunnableFromQueue::NotifyMainThread() throw()
	{
		PushMonadQueue( this );
	}

	void PushMonadQueue( IQue * pMsg ) throw()
	{
		CLock tmpLock( mRunnableQueue.GetMutex() );
		mRunnableQueue.push( pMsg );
	}

	bool OnDequeue( )
	{
		bool bD( false );
		IQue * pCommand( NULL );

		{
			CLock tmpLock( mRunnableQueue.GetMutex() );
			if( !mRunnableQueue.empty() )
			{
				pCommand = mRunnableQueue.front();
				mRunnableQueue.pop();
				bD = true;
			}
		}
		if( NULL != pCommand )
			pCommand->ProcessQueue();

		return bD;
	}
}