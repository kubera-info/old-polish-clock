#pragma once
#ifndef _METAL_CORE_H_
#define _METAL_CORE_H_


#include "Patterns.h"
#include "MeshCore.h"

struct CMetalMeshCreator : public CDestructable
{
	CMetalMeshCreator();

	struct CMetal : public CMeshBuilderTex
	{
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c );
	} mMetal;
};

#endif // _METAL_CORE_H_