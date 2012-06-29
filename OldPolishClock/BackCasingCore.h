#pragma once
#ifndef _BACK_CASING_CORE_H_
#define _BACK_CASING_CORE_H_

#include "Patterns.h"
#include "MeshCore.h"
#include "MonadBisorter.h"

struct CBackCasingMeshCreator : public CDestructable
{
	CBackCasingMeshCreator();

	struct CBack : public CMeshBuilderTex 
	{
	} back;
	struct CBack2 : public CMeshBuilderTex 
	{
		CBack2()
		{
			m_bStage = true;
		}
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c );

		void InsertSideIfNeeded( const VERTEXTEX & a, const VERTEXTEX & b );
		bool m_bStage;
		CMonadLines2 m_Lines;
	} back2;
	struct CBack3 : public CMeshBuilderTex 
	{
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c );
	} back3;
};

struct CBackCasingScaller
{
	CBackCasingScaller();
};
#endif // _BACK_CASING_CORE_H_