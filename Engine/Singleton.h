#pragma once
#ifndef _MONAD_SINGLETON_H_
#define _MONAD_SINGLETON_H_

#include "Manager.h"
#include "Factory.h"
#include "MonadEvents.h"
#include "MonadStreaming.h"
#include "FVF.h"
#include "World.h"
#include "MonadTiming.h"
#include "MonadState.h"
#include "OPC-Struct.h"
#include "MonadWindow.h"

struct StructSingleton : public CWorldGrCombine
{
	StructSingleton();
	virtual ~StructSingleton();

	double GetMonadTime() const;
	static void CallResetDevice( );
	static void CallResetLogic( );
	static void CallResetLogicDX9( );
	static void CallResetLogicDX11( );
	static void CallLostDevice( );

	void OnCreateDeviceAndShadow();

    ID3D11RenderTargetView* pRTV;
    ID3D11DepthStencilView* pDSV;
	CWorldGrCombine::CStack mMatrixStack;

	structDeclarations Declarators;	
	CSampler mSamplerLinear, mSamplerAnizotropic;
	CBlendState mAlpha;
	CCBufferSlot::CCBufferRepository Slots;	
	CShadowHelper mShadowHelper, mShadowHelperDyn;	
	CMonadShaderGeneric::CShaders mShaders11;	
	CMonadFactory MonadFactory;
	CAutoPtr<CBufferV> LightBuffer;
	CAutoPtr<CWorld> mWorld;	
	CMonadTiming mTiming;	
	CMonadVolumeRestore mRestoreVolume;
	CAutoPtr<CSatelliteManager> m_pSatellites;
};

extern CAutoPtr<StructSingleton> Singleton;

#endif