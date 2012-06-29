#pragma once
#ifndef _NUT_CORE_H_
#define _NUT_CORE_H_

#include "MeshCore.h"

struct CNutMeshCreator : public CDestructable
{
	CNutMeshCreator();

	CMeshBuilderTex mNut;
};

struct CSteelMeshCreator : public CDestructable
{
	CSteelMeshCreator();

	CMeshBuilderTex mSteel;
};

#endif // _NUT_CORE_H_