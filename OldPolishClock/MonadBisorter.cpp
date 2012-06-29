#include "DXUT.h"
#include "MonadBisorter.h"

CMonadLine CMonadLines2::GetLine( const VERTEXTEX & a, const VERTEXTEX & b ) const
{
	static CMonadLine l1, l2;
	l1.A = a.Position;
	l1.B = b.Position;
	l2.B = a.Position;
	l2.A = b.Position;
	return l1 < l2 ? l1 : l2;
}
void CMonadLines2::AddLine( const VERTEXTEX & a, const VERTEXTEX & b )
{
	insert( GetLine( a, b ) );
}
void CMonadLines2::AddTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
{
	AddLine( a, b );
	AddLine( a, c );
	AddLine( b, c );
}
bool CMonadLines2::IsSide( const VERTEXTEX & a, const VERTEXTEX & b ) const
{
	return count( GetLine( a, b ) ) == 1;
}