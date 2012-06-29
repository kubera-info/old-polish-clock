#include "DXUT.h"
#include "MonadException.h"
#include "MonadLock.h"
#include "MonadMath.h"
#include "Patterns.h"
#include "MonadTime.h"

static const double START_STATE ( 0.0 );
static const double STOP_STATE ( -1.0 );

static MonadThread::CMutex gMultiLerp;

CTimeLerp::CTimeLerp()
{	
	RegisterEvent( MonadEvent::EventFrameMoveStub );
	SetLerpParams(60.0f, 0.0f, 1.0f ); // Dummy defaults
}

void CTimeLerp::SetLerpParams(
							  const double timeInSeconds, 
							  const double startValue, 
							  const double stopValue )
{
	gMultiLerp.Enter();
	assert( 0 < timeInSeconds );
	m_timeInSeconds = timeInSeconds;
	m_startValue = startValue;
	m_stopValue = stopValue;
	InternalReset();
	gMultiLerp.Leave();
}
void CTimeLerp::InternalReset()
{
	m_startTime = START_STATE;
	m_bIsFinished.ResetCalledFlag();
}
void CTimeLerp::Reset()
{
	gMultiLerp.Enter();
	InternalReset();
	gMultiLerp.Leave();
}
void CTimeLerp::Flush()
{
	MonadThread::CLock Lock( gMultiLerp );
	m_startTime = STOP_STATE;
	OnFlush();
}

void CTimeLerp::Start()
{
	MonadThread::CLock Lock( gMultiLerp );
	m_startTime = MonadTime::fTime;
	OnStart();
}

void CTimeLerp::OnFrameMoveStub()
{
	if( m_startTime > START_STATE )
	{
		GetCurrentValue( MonadTime::fTime );
		OnFrameMove();
	}
}

float CTimeLerp::GetStartValue() const
{
	return (float) m_startValue;
}

float CTimeLerp::GetStopValue() const
{
	return (float) m_stopValue;
}

float CTimeLerp::GetTimeInSeconds() const
{
	return (float) m_timeInSeconds;
}

float CTimeLerp::GetCurrentValue( const double CurrTime )
{
	if( START_STATE == m_startTime )
		return (float) m_startValue;
	if( STOP_STATE == m_startTime )
		return (float) m_stopValue;
	if( m_startTime + m_timeInSeconds > CurrTime )
		return (float) ( m_startValue 
		+ ( CurrTime - m_startTime ) / m_timeInSeconds
		* ( m_stopValue - m_startValue ) );
	else
	{
		Flush();
		if( !m_bIsFinished )
			OnStop();
		return (float) m_stopValue;
	}
}
bool CTimeLerp::IsFinished() const throw()
{
	return m_bIsFinished.GetFlagUnchanged();
}
bool CTimeLerp::IsRunning() const throw()
{
	return !m_bIsFinished.GetFlagUnchanged() && m_startTime != START_STATE && m_startTime != STOP_STATE;
}

void CRandomString::AddString( const std::wstring & pFilename )
{
	push_back( pFilename );
}

void CRandomString::RemoveString( const std::wstring & pFilename )
{
	CStringArray::const_iterator i = find( begin(), end(), pFilename );

	if ( i != end() )
		erase( i );
	else
		THROW_EXC( MonadException::NoDataFound, E_FAIL, L"DelStr" );
}

CRandomString::operator const std::wstring & () const
{
	return GetRandomString();
}

const std::wstring & CRandomString::GetRandomString() const
{
	if( empty() )
		THROW_EXC( MonadException::NullValue, E_FAIL, L"Record not found" );
	return ( *this )[ MonadMath::GetRandomULongPtr( 0, size() - 1 ) ];
}


/*CCounter::CCounter()
{
	SetCounterParams( 0, 1 );
}

void CCounter::SetCounterParams(
								const LONG_PTR startValue, 
								const LONG_PTR stopValue )
{
	m_startValue = m_currValue = startValue;
	m_stopValue = stopValue;
	CheckIfFinished();
}

void CCounter::MakeOneStep()
{
	if( m_startValue < m_stopValue )
		m_currValue ++;
	else if( m_startValue > m_stopValue )
		m_currValue --;
	CheckIfFinished();
}

void CCounter::MakeSteps( const LONG_PTR pCnt )
{
	m_currValue += pCnt;
	if( ( m_startValue < m_stopValue && m_stopValue < m_currValue )
		|| ( m_startValue > m_stopValue && m_stopValue > m_currValue ) )
		m_currValue = m_stopValue;
	CheckIfFinished();
}

LONG_PTR CCounter::GetStartValue() const
{
	return m_startValue;
}

LONG_PTR CCounter::GetStopValue() const
{
	return m_stopValue;
}

LONG_PTR CCounter::GetCurrentValue() const
{
	return m_currValue;
}

void CCounter::Reset()
{
	m_currValue = m_startValue;
	CheckIfFinished();
}
void CCounter::CheckIfFinished()
{
	if( m_currValue == m_stopValue )
		OnStop();
}*/