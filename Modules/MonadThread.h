// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MONADTHREAD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MONADTHREAD_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#pragma once
#ifndef _MONAD_THREAD_H_
#define _MONAD_THREAD_H_

#include <atlutil.h>

namespace MonadThread
{
	// Info
	DWORD NumberOfProcessCores() throw();
	DWORD GetDefaultThreadsNumber() throw();

	interface IQue abstract
	{
	public:
		virtual void ProcessQueue() throw(...) = 0;
	};

	//-----------------------------------------------------------------------------
	// CRunnable
	// Generic class for future processing in a thread pool
	//-----------------------------------------------------------------------------
	struct CRunnable abstract
	{
		// Processing function
		virtual void Run() throw(...) = 0;
		virtual void NotifyMainThread() throw();
	};

	struct CRunnableFromQueue abstract : public CRunnable, IQue
	{
		// Processing function
		virtual void NotifyMainThread() throw();
	};

	struct CThreadPoolManager sealed
	{
		CThreadPoolManager( const int pThreadsCnt = GetDefaultThreadsNumber() );
		~CThreadPoolManager();
		void InsertTask( CRunnable * pTask );

	private:
		//
		// CMonadWorker
		// Thread worker for processing runnable tasks
		//
		class CMonadWorker
		{
		public:
			typedef CRunnable* 	RequestType;

			BOOL Initialize(void* pvParam);
			void Terminate(void* pvParam);
			void Execute(RequestType	 dw, void*	pvParam, OVERLAPPED *pOverlapped) throw();
			BOOL GetWorkerData(DWORD /*dwParam*/, void** /*ppvData*/);
		};

		typedef CThreadPool<CMonadWorker>	CMonadThreadPool;
		CMonadThreadPool m_ThreadPool;
	};

	void PushMonadQueue( IQue * pMsg ) throw();
	bool OnDequeue();
}
#endif