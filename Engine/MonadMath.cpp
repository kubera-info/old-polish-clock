#include "DXUT.h"
#define SystemFunction036 NTAPI SystemFunction036
#include <Ntsecapi.h>
#undef SystemFunction036
#include "MonadException.h"
#include "MonadMath.h"

namespace MonadMath
{
	CEllipse::CEllipse( const DWORD pSidesCnt, const float pRadius_x, float pRadius_y, const float pAngle, const float p_start )
		:
	mRadius_x( pRadius_x ),
		mRadius_y( NULL == pRadius_y ? pRadius_x : pRadius_y )
	{
		float mPhase = p_start;
		const float m_krok = pAngle / pSidesCnt;
		for( DWORD m_ind = 0; m_ind < pSidesCnt; m_ind ++ )
		{
			AddPoint( mPhase );
			mPhase += m_krok;
		}
		if( XM_2PI == pAngle )
			mPoints.push_back( mPoints[ 0 ] );
		else
			AddPoint( p_start + pAngle );
	}
	const XMFLOAT2& CEllipse::operator[]( size_t n ) const
	{
		return mPoints[ n ];
	}
	const DWORD CEllipse::GetPointsCnt() const
	{
		return ( DWORD ) mPoints.size();
	}
	const DWORD CEllipse::GetSidesCnt() const
	{
		return GetPointsCnt() - 1;
	}
	void CEllipse::AddPoint( const float p_faza )
	{
		mPoints.push_back( XMFLOAT2( cosf( p_faza ) * mRadius_x, -sinf( p_faza ) * mRadius_y ) );
	}

	LONG_PTR GetRandomLongPtr( const LONG_PTR pMin, const LONG_PTR pMax )
	{
		LONG_PTR randomValue;
		RtlGenRandom(
			&randomValue,
			sizeof( randomValue ) );

		return pMin + randomValue % ( pMax - pMin + 1 );
	}
	ULONG_PTR GetRandomULongPtr( const ULONG_PTR pMin, const ULONG_PTR pMax )
	{
		ULONG_PTR randomValue;
		RtlGenRandom(
			&randomValue,
			sizeof( randomValue ) );

		return pMin + randomValue % ( pMax - pMin + 1 );
	}

	CAligned16TypeGeneric::CAligned16TypeGeneric( const size_t pSize )
		: m_pAligned( AlignedAlloc( pSize ) )
	{
	}
	CAligned16TypeGeneric::~CAligned16TypeGeneric()
	{
#ifdef _WIN64
		delete [] m_pAligned;
#else
		_mm_free( m_pAligned );
#endif
	}
	void * CAligned16TypeGeneric::AlignedAlloc( size_t pSize )
	{
#ifdef _WIN64
		void * tmpRes = new char[ pSize ];
#else
		void * tmpRes = _mm_malloc( pSize, 16 );
		if( NULL == tmpRes )
			throw std::bad_alloc();
#endif
		return tmpRes;
	}
	const void * const CAligned16TypeGeneric::GetVar() const
	{
		return m_pAligned;
	}
	void * const CAligned16TypeGeneric::GetVar()
	{
		return m_pAligned;
	}
}