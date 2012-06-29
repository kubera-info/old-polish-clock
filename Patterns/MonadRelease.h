#pragma once
#ifndef _MONAD_RELEASE_H_
#define _MONAD_RELEASE_H_

inline void MonadRelease( IUnknown * pInterface )
{
	assert( NULL != pInterface );
	pInterface->Release();
}

#endif