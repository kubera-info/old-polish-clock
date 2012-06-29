#pragma once
#ifndef _TESSELLATION_CORE_N_
#define _TESSELLATION_CORE_N_

#include "MeshCore.h"

struct CStaticTesselation
{
	CStaticTesselation(
		const float pWidth,
		const float pHeight, 
		const float pZ,
		const float pCntHorizontal, 
		const float pCntVertical );

	const float LerpXByWidth( const float pFactor ) const;
	const float LerpYByHeight( const float pFactor ) const;
	const float LerpXOneByWidth( const float pFactor ) const;
	const float LerpYOneByHeight( const float pFactor ) const;

	const float mWidth;
	const float mHeight;
	const float mCntHorizontal;
	const float mCntVertical;

	CMeshBuilderTex Box;
};
					 

#endif // _TESSELLATION_CORE_N_