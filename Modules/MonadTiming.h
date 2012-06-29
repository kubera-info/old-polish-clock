#pragma once
#ifndef _MONAD_TIMING_H_
#define _MONAD_TIMING_H_

#include "Patterns.h"
#include "MonadEvents.h"
#include "MonadStreaming.h"

extern CWaveStreaming * Streaming;

struct CTicToc sealed : protected MonadEvent::COnEvent
{
	CTicToc();

private:
	virtual void OnNewSecond();
};
extern CTicToc * TicToc;

struct CBell sealed : protected MonadEvent::COnEvent
{
	CBell();

private:
	virtual void OnNewSecond();
};
extern CBell * Bell;

struct CMonadTiming sealed : public CTimeLerp
{
	CMonadTiming();

	bool IsWarming() throw();
	virtual void OnStop();
};

void SetAllVolumes( float pStreamVolume );

void CalculateFallingVolume();

struct CMonadVolumeRestore sealed : public CTimeLerp
{
	CMonadVolumeRestore();
	virtual void OnFrameMove();
	virtual void OnStop();
};

#endif // _MONAD_TIMING_H_