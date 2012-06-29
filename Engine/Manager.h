#pragma once
#ifndef _MONAD_MANAGER_H_
#define _MONAD_MANAGER_H_

#include "MonadSystem.h"
#include "Monad.h"
#include "MonadTemplates.h"

//
// CWorldGrCombine
//
class CWorldGrCombine : public CMonad
{
protected:
	typedef struct CMonadCoherentVector<CMonad *> CMonadArray;

public:
	CWorldGrCombine()
	{
		InvalidateSeq();
	}
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter ) { }
	virtual void OnFrameRender();
	virtual void AddRenderedObject( CMonad * pObject );
	const CMonadArray::size_type GetSize() const;
	const CMonadArray::size_type GetMonadSeq() const;
	float GetMonadFactor() const;

	struct CStack : public std::stack< XMFLOAT4X4 >
	{
		CStack();
	};

protected:
	CMonadArray m_rendered_objects;
	void InvalidateSeq();
	CMonadArray::size_type m_IMonadSeq;
};

#endif