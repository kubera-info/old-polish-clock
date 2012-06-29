#pragma once
#ifndef _MONAD_LOCK_H_
#define _MONAD_LOCK_H_

#include "DXUT.h"
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

namespace MonadThread
{
	// Types
	struct CMutex sealed
	{
		CMutex();
		void Enter();
		void Leave();

	private:
#ifdef DEBUG
		typedef boost::shared_ptr<volatile DWORD> CAutoId;
		CAutoId mId; 
#endif
		typedef boost::shared_ptr<CRITICAL_SECTION> CAutoCriticalSection;
		CAutoCriticalSection mCS;
	};

	struct CLock sealed : boost::noncopyable
	{
		explicit CLock( CMutex &Source );
		~CLock();

	private:
		CMutex * mMutex;
	};
}

#endif // _MONAD_LOCK_H_