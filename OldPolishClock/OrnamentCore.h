#pragma once
#ifndef _ORNAMENT_CORE_H_
#define _ORNAMENT_CORE_H_

#include "Patterns.h"
#include "MeshCore.h"

struct COrnamentMeshCreator : public CDestructable
{
	COrnamentMeshCreator();

	CMeshBuilderPlain mOrnament;
	CMeshBuilderTex mClockF;
};

struct CMissMeshCreator
{
	CMissMeshCreator();

	struct CMeshMiss : public CMeshBuilderTex
	{
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c );
	} mMiss;
};

#endif