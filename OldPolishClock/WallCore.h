#pragma once
#ifndef _WALL_CORE_H_
#define _WALL_CORE_H_

#include "Patterns.h"

#ifdef DEBUG
struct CBrickMeshCreator : public CDestructable
{
	CBrickMeshCreator();
};

struct CPlasterMeshCreator : public CDestructable
{
	CPlasterMeshCreator();
};
#endif

#endif