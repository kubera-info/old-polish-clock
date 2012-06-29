#pragma once
#ifndef _MONAD_PTR_H_
#define _MONAD_PTR_H_

#include "MonadEvents.h"
#include <boost/shared_ptr.hpp>
#include "MonadRelease.h"

template
<
typename T
>
class CMonadAutoPtr : protected boost::shared_ptr< T >
{
public:
	explicit CMonadAutoPtr( T * ptr ) : boost::shared_ptr< T >( ptr )
	{
		assert( ptr != NULL );
	}
	CMonadAutoPtr()
	{
	}
	void Assign( T * pNewPtr )
	{
		assert( NULL != pNewPtr );
		assert( NULL == get() );
		reset( pNewPtr );
	}
	void Reset()
	{
		reset();
	}
	inline T * get()
	{
		return boost::shared_ptr< T >::get();
	}
};

template
<
typename T
>
class CMonadAutoDestroyPtr : protected CMonadAutoPtr<T>, protected MonadEvent::COnEvent
{
public:
	explicit CMonadAutoDestroyPtr( T * ptr ) : CMonadAutoPtr<T>( ptr )
	{
		assert( ptr != NULL );
	}
	CMonadAutoDestroyPtr()
	{
	}
	void Reset()
	{
		reset();
	}
	virtual void OnDestroyDevice()
	{
		reset();
	}
	inline void Assign( T * pNewPtr )
	{
		CMonadAutoPtr<T>::Assign( pNewPtr );
	}
};

typedef boost::shared_ptr< IUnknown > CMonadIUnknown;
class CMonadIPtrGeneric abstract : protected CMonadIUnknown
{
public:
	explicit CMonadIPtrGeneric( IUnknown * ptr ) : CMonadIUnknown( ptr, MonadRelease )
	{
		assert( ptr != NULL );
	}
	CMonadIPtrGeneric()
	{
	}
	void resetRelease( IUnknown * pNewPtr )
	{
		assert( NULL != pNewPtr );
		assert( NULL == get() );
		reset( pNewPtr, MonadRelease );
	}
	void Reset()
	{
		reset();
	}
};

template
<
typename T
>
class CMonadIPtrBasic : public CMonadIPtrGeneric
{
public:
	explicit CMonadIPtrBasic( T * ptr ) : CMonadIPtrGeneric( ptr )
	{
	}
	CMonadIPtrBasic()
	{
	}
	operator T*() const throw()
	{
		return reinterpret_cast< T * > ( get() );
	}
	inline T * GetInterface() const throw()
	{
		return reinterpret_cast< T * > ( get() );
	}
	T *operator->()
	{
		assert( NULL != get() );
		return GetInterface();
	}
};

class CMonadIPtrDestroyGeneric abstract : public CMonadIPtrGeneric, protected MonadEvent::COnEvent
{
public:
	explicit CMonadIPtrDestroyGeneric( IUnknown * ptr ) : CMonadIPtrGeneric( ptr )
	{
	}
	CMonadIPtrDestroyGeneric()
	{
	}
	virtual void OnDestroyDevice()
	{
		reset();
	}
};

template
<
typename T
>
class CMonadIPtr : public CMonadIPtrDestroyGeneric
{
public:
	explicit CMonadIPtr( T * ptr ) : CMonadIPtrDestroyGeneric( reinterpret_cast< IUnknown * > ( ptr ) )
	{
	}
	CMonadIPtr()
	{
	}
	operator T*() const throw()
	{
		return reinterpret_cast< T * > ( get() );
	}
	inline T * GetInterface() const throw()
	{
		return reinterpret_cast< T * > ( get() );
	}
	T *operator->()
	{
		assert( NULL != get() );
		return GetInterface();
	}
};

template
<
typename T
>
class CMonadLostableIPtr : public CMonadIPtrDestroyGeneric
{
public:
	explicit CMonadLostableIPtr( T * ptr ) : CMonadIPtrDestroyGeneric( reinterpret_cast< IUnknown * > ( ptr ) )
	{
		RegisterLostEvent();
	}
	CMonadLostableIPtr()
	{
		RegisterLostEvent();
	}
	virtual void OnLostDevice()
	{
		reset();
	}
	operator T*() const throw()
	{
		return reinterpret_cast< T * > ( get() );
	}
	inline T * GetInterface() const throw()
	{
		return reinterpret_cast< T * > ( get() );
	}
	T *operator->()
	{
		assert( NULL != get() );
		return GetInterface();
	}
};

#endif // _MONAD_PTR_H_