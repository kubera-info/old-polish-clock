#pragma once
#ifndef _MONAD_BISORTER_H_
#define _MONAD_BISORTER_H_

#include "DXUT.h"
#include <set>
#include "MeshCore.h"

struct CMonadLine
{
	XMFLOAT3 A, B;
};

inline bool operator<( const CMonadLine & pLeftFormat, const CMonadLine & pRightFormat )
{
	return 0 < memcmp( &pLeftFormat, &pRightFormat, sizeof( WAVEFORMATEX ) );
}

typedef std::multiset<const CMonadLine> CMonadLines;

struct CMonadLines2 : public CMonadLines
{
	CMonadLine GetLine( const VERTEXTEX & a, const VERTEXTEX & b ) const;
	void AddLine( const VERTEXTEX & a, const VERTEXTEX & b );
	void AddTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c );
	bool IsSide( const VERTEXTEX & a, const VERTEXTEX & b ) const;
};

#endif // _MONAD_BISORTER_H_