#pragma once
#ifndef _HINGE_CORE_H_
#define _HINGE_CORE_H_

#include "Patterns.h"
#include "MeshCore.h"

struct CHingeMeshCreator : public CDestructable
{
	CHingeMeshCreator();

	struct CHinge : public CMeshBuilderPlain
	{
		CHinge();
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c );
		float height, radius, x, y, z;
		bool bRotate;
	} Hinge;
};

struct COvalCreator
{
	COvalCreator();
};
#endif // _HINGE_CORE_H_