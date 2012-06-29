#pragma once
#ifndef _STRING_CORE_H_
#define _STRING_CORE_H_


#include "Patterns.h"
#include "MeshCore.h"

struct COneMeshCreator : public CDestructable
{
	COneMeshCreator();

	struct COne : public CMeshBuilderPlain
	{
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c );
	} One;
	struct CParametrizedString : public CMeshBuilderPlain
	{
		CParametrizedString();
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c );
		void AlterPos( VERTEXPLAIN & a );
		void OnOne();

	protected:		
		bool m_bRotated;
		float m_fY;
	} mShort, mLong;
	struct CTorus : public CMeshBuilderPlain
	{
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c );
	} Torus;
};

#endif // _STRING_CORE_H_