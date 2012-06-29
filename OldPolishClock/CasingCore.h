#pragma once
#ifndef _CASING_CORE_H_
#define _CASING_CORE_H_

#include "Patterns.h"
#include "MeshCore.h"

struct CWoodCasingMeshCreator : public CDestructable
{
	CWoodCasingMeshCreator();

	CMeshBuilderTex Casing;
	CMeshBuilderTex Front;
	CMeshBuilderTex Front2;
	CMeshBuilderTex Side;
	CMeshBuilderPlain Glass;
	CMeshBuilderPlain Glass2;
};

#endif