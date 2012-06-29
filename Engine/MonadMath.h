#pragma once
#ifndef _MONAD_MATH_H_
#define _MONAD_MATH_H_

#include <boost/utility.hpp>
#include <xnamath.h>
#include "MonadTemplates.h"

namespace MonadMath
{
	template<typename NumVal>
	NumVal SGN( const NumVal pValue )
	{
		if( 0 == pValue )
			return NumVal( 0 );
		return ( ( pValue < 0 ) ? NumVal( -1 ) : NumVal( 1 ) );
	}

	class CEllipse
	{
	public:
		CEllipse( const DWORD pSidesCnt, const float pRadius_x = NULL, float pRadius_y = 0.0f, const float pAngle = XM_2PI, const float p_start = 0.0f );
		const XMFLOAT2& operator[]( size_t n ) const;
		const DWORD GetPointsCnt() const;
		const DWORD GetSidesCnt() const;
		float mRadius_x;
		float mRadius_y;

	protected:
		std::vector<XMFLOAT2> mPoints;
		// Calculates one point for the arc
		void AddPoint( const float p_faza );
	};

	LONG_PTR GetRandomLongPtr( const LONG_PTR pMin, const LONG_PTR pMax );
	ULONG_PTR GetRandomULongPtr( const ULONG_PTR pMin, const ULONG_PTR pMax );

	// A helper
	struct CAligned16TypeGeneric abstract : boost::noncopyable
	{
		CAligned16TypeGeneric(); // Denied
		CAligned16TypeGeneric( const size_t pSize );
		virtual ~CAligned16TypeGeneric();

	protected:
		const void * const GetVar() const;
		void * const GetVar();

	private:
		//const size_t mBufferSize;
		void * const m_pAligned;
		void * AlignedAlloc( size_t pSize );
	};

	// This template allocates aligned structure, but does not call a constructor
	// The structure should be initialized after creation manually
	// Purpose: aligned structures for XNA Math and Constant Buffers in DX10, etc...
	// Do not implement default constructors, desctructors and virtual methods for boxed values
	// Or do alter this code :)
	template
		<
		typename SrcType,
		typename Derive = CAligned16TypeGeneric
		>
	struct CAligned16Type : public Derive
	{
		CAligned16Type()
			: Derive( sizeof( SrcType ) ) {}

		inline operator const SrcType & () const { return GetVal(); }
		inline operator SrcType & () { return GetVal(); }
		inline const SrcType & GetVal() const { return * reinterpret_cast< const SrcType * > ( GetVar() ); };
		inline SrcType & GetVal() { return * reinterpret_cast< SrcType * > ( GetVar() ); };
	};
}

#endif // _MONAD_MATH_H_