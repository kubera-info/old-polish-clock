#pragma once
#ifndef _HOOK_CORE_H_
#define _HOOK_CORE_H_


#include "Patterns.h"
#include "MeshCore.h"

struct CHookMeshCreator : public CDestructable
{
	CHookMeshCreator();

	CMeshBuilderPlain hak0;
	struct CHookAddTransform : public CMeshBuilderPlain
	{
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c );
	} hook;
	struct CHookAddDeph : public CMeshBuilderPlain
	{
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c );
	} hook2;
	struct CHookAddFinalize : public CMeshBuilderPlain
	{
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c );
		float Y;
	} hook3;
};
#endif // _HOOK_CORE_H_