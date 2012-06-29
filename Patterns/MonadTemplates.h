#pragma once
#ifndef _MONAD_TEMPLATES_H_
#define _MONAD_TEMPLATES_H_

#ifndef DEBUG
#define _SCL_SECURE 0
#endif

#include <set>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>
#include <utility>
#include <string>
#include <boost/foreach.hpp>
#include "CProto.h"
#include "MonadLock.h"

void ThrowNoDataFoundInColl1();

template<class T>
bool operator<( const T & pLeftFormat, const T & pRightFormat ) throw()
{
	return 0 < memcmp( &pLeftFormat, &pRightFormat, sizeof( T ) );
}

struct CWithMutex abstract
{
	void Enter() const
	{
		mSingleOp.Enter();
	}
	void Leave() const
	{
		mSingleOp.Leave();
	}
	MonadThread::CMutex & GetMutex() const
	{
		return mSingleOp;
	}

protected:
	mutable MonadThread::CMutex mSingleOp;
};

template
<
typename Key,
typename Val
>
struct CMonadCoherentMap : public CWithMutex, public std::map< const Key, Val >
{
};

template
<
typename Val
>
struct CMonadCoherentQueue : public CWithMutex, public std::queue<Val>
{
	void flush()
	{
		while( !std::queue<Val>::empty() )
			std::queue<Val>::pop();
	}
};

template
<
typename Val
>
struct CMonadCoherentVector : public CWithMutex, public std::vector<Val>
{
};

template
<
typename Val,
typename Less=std::less<Val>
>
struct CMonadSet : public std::set<Val, Less>
{
	typedef typename Val SetValue;
	typedef typename std::pair<typename iterator, bool> Query;

	struct CObjFunctor : public CDestructable
	{
		virtual void operator()( SetValue pObj ) = 0;
		// virtual void before() {};
	};
	void for_each( CObjFunctor * pFun )
	{
		assert( NULL != pFun );

		// pFun->before();

		BOOST_FOREACH( SetValue pObj, *this )
			( * pFun )( pObj );
	}
};

template
<
typename Val,
typename Less=std::less<Val>
>
struct CMonadCoherentSet : public CWithMutex, public CMonadSet<Val, Less>
{
};

typedef std::vector<const std::wstring> CStringArray;

#endif