#pragma once
#ifndef _MONAD_PATTERNS_H_
#define _MONAD_PATTERNS_H_

#include <string>
#include "MonadTime.h"
#include "CProto.h"
#include "MonadTemplates.h"
#include "SingleCall.h"
#include "MonadEvents.h"

//-----------------------------------------------------------------------------
// CRandomStrings class
// Class chooses a random string
// It may be used instead of simple std::wstring
// All objects of this class for speech should begin with Speech, ie. SpeechYesNo, etc.
//-----------------------------------------------------------------------------
class CRandomString sealed : private CStringArray
{
public:
	void AddString( const std::wstring & pFilename );
	void RemoveString( const std::wstring & pFilename );
	operator const std::wstring & () const; // GetRandomString()

protected:
	const std::wstring & GetRandomString() const;
};

class CMonadEvent abstract
{
public:
	virtual void OnStart() { }
	virtual void OnStop() {	}
	virtual void OnFlush() { }
	virtual void Reset() { }
	virtual void Flush() { }
	virtual void Start() { }
};
//-----------------------------------------------------------------------------
// CTimeLerp
// Moves it's value from start to ending trough choosen period of time
// This is a linear interpolation, but calculating method is virtual
//-----------------------------------------------------------------------------
class CTimeLerp : public CMonadEvent, public MonadEvent::COnEvent
{
public:
	CTimeLerp();
	//virtual ~CTimeLerp();

	void SetLerpParams(
		const double timeInSeconds, 
		const double startValue, 
		const double stopValue );

	virtual void Reset();
	virtual void Flush();
	virtual void Start();

	float GetStartValue() const;
	float GetStopValue() const;
	float GetTimeInSeconds() const;
	virtual float GetCurrentValue( const double CurrTime = MonadTime::fTime );
	bool IsFinished() const throw();
	bool IsRunning() const throw();


protected:
	void InternalReset();
	virtual void OnFrameMoveStub();
	virtual void OnFrameMove() {};
	class CSealedSingleCall m_bIsFinished;

	volatile double m_startTime;
	volatile double m_timeInSeconds;
	volatile double m_startValue;
	volatile double m_stopValue;
};

/*class CCounter : public CMonadEvent
{
public:
	CCounter();
	void SetCounterParams(
		const LONG_PTR startValue, 
		const LONG_PTR stopValue );
	void MakeOneStep();
	void MakeSteps( const LONG_PTR pCnt );

	void Reset();

	LONG_PTR GetStartValue() const;
	LONG_PTR GetStopValue() const;
	LONG_PTR GetCurrentValue() const;

protected:
	void CheckIfFinished();
	LONG_PTR m_startValue, m_stopValue, m_currValue;
};

class CFlipFlop : public CMonadEvent
{
public:
	CFlipFlop( const bool pbStartState = false )
		: m_bState( pbStartState ), m_bStartState( pbStartState )
	{
		OnStart();
	}
	operator bool ()
	{
		m_bState = !m_bState;
		OnFlush();
		return m_bState;
	}
	void Reset()
	{
		OnStart();
		m_bState = m_bStartState;
	}

protected:
	volatile bool m_bState;
	volatile bool m_bStartState;
};*/
#endif