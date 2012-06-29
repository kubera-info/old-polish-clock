#pragma once
#ifndef _MIDDLE_HOOK_CORE_H_
#define _MIDDLE_HOOK_CORE_H_

#include "MeshCore.h"

struct CDumpMiddleHooh
{
	CDumpMiddleHooh();

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
	struct CHookAddFinalize2 : public CMeshBuilderPlain
	{
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c );
		float X;
	} hook4;
};

#endif // _MIDDLE_HOOK_CORE_H_