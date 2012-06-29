#pragma once
#ifndef _MONAD_STATE_H_
#define _MONAD_STATE_H_

#include "DeviceLevel.h"
#include "Monad.h"

struct CSampler : boost::noncopyable 
{
	CSampler( D3D11_FILTER pFilter, UINT pMaxAnizotropy = 1 );
	~CSampler() throw() { if( NULL != m_pSampler ) m_pSampler->Release(); }
	operator ID3D11SamplerState** () throw();

private:
	ID3D11SamplerState * m_pSampler;
};

struct CBlendState : boost::noncopyable
{
	CBlendState();
	~CBlendState() throw() { if( NULL != m_pBlendState ) m_pBlendState->Release(); }
	operator ID3D11BlendState* () throw();

private:
	ID3D11BlendState * m_pBlendState;
};

class CMonadState : public CMonad
{
public:
	virtual bool IsEnabled( ){ return DXUT_D3D11_DEVICE == DeviceLevel; }
};

class CMonadResetBlendState sealed : public CMonadState
{
public:
	virtual void OnFrameRender();
};

class CMonadSetBlendState sealed : public CMonadState
{
public:
	virtual void OnFrameRender();

private:
	CBlendState mBlend;
};

#endif // _MONAD_STATE_H_