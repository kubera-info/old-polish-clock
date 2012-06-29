#pragma once
#ifndef _MONAD_UNIQUE_INSTANCE_H_
#define _MONAD_UNIQUE_INSTANCE_H_

#include <boost/utility.hpp>

struct CAmIUnique sealed : boost::noncopyable
{
	CAmIUnique( LPCWSTR pId );
	~CAmIUnique() throw();
	operator bool () const throw();
	void InvalidateUnique();

private:
	HANDLE m_hMyMutex;
};

#endif // _MONAD_UNIQUE_INSTANCE_H_