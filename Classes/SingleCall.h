#pragma once
#ifndef _MONAD_SINGLE_CALL_H_
#define _MONAD_SINGLE_CALL_H_

#include "CProto.h"

//-----------------------------------------------------------------------------
// CAbstractSingleCall
// For the first time, it will return a false value
// It has resetting function also
//-----------------------------------------------------------------------------
class CAbstractSingleCall abstract : public CDestructable
{
public:
	CAbstractSingleCall( const bool pCalled = false ) throw();
	operator bool () throw();
	void ResetCalledFlag() throw();
	bool GetFlagUnchanged() const throw();

private:
	inline bool WasCalled() throw();
	volatile bool m_bCalled;
};

//
// Dummy implementation of single call logic
//
class CSealedSingleCall sealed : public CAbstractSingleCall
{
public:
	CSealedSingleCall( const bool pCalled = false ) throw() : CAbstractSingleCall( pCalled ) { };
};

#endif