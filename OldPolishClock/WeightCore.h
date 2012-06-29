#pragma once
#ifndef _WEIGHT_CORE_H_
#define _WEIGHT_CORE_H_

#include "Patterns.h"
#include "MeshCore.h"

struct CWeightMeshCreator : public CDestructable
{
	CWeightMeshCreator();

	CMeshBuilderTex mCanonical;
	struct CNormalsAndSimple : public CMeshBuilderTex
	{
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c );
	} mMetal;
	struct CConvert : public CMeshBuilderTex
	{
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c );

		CMeshBuilderPlain mNew;
	} mConvert;
};

#endif // _WEIGHT_CORE_H_