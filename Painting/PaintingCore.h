#pragma once
#ifndef _PAINTING_CORE_H_
#define _PAINTING_CORE_H_

#include "Patterns.h"
#include "MeshCore.h"

struct CPaintingMeshCreator : public CDestructable
{
	CPaintingMeshCreator();
	void Proc1();
	void Proc2();
	void Proc3();

	struct CBorderBuilder : public CMeshBuilderTex
	{
		void AddTriangleHorizontal( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c );
		void AddTriangleVertical( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c );
		void AddTriangleHorizontalDepth( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c );
		void AddTriangleVerticalDepth( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c );
		void AddTriangleMetallic( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c );
	} Casing;

	VERTEXTEX LeftUpper, RightUpper;
	VERTEXTEX RightLower, LeftLower;
	VERTEXTEX LeftUpper2, RightUpper2;
	VERTEXTEX RightLower2, LeftLower2;

	VERTEXTEX BackLeftUpper, BackRightUpper;
	VERTEXTEX BackRightLower, BackLeftLower;
	VERTEXTEX BackLeftUpper2, BackRightUpper2;
	VERTEXTEX BackRightLower2, BackLeftLower2;
};

struct CCanvasMeshCreator abstract : public CDestructable
{
	CCanvasMeshCreator( const std::wstring & pPath, const float pCanvasRatio, const float pCanvasScale );

	CMeshBuilderTex Canvas;
};

struct CImageMeshCreator : public CCanvasMeshCreator
{
	CImageMeshCreator();
};

struct CPaintingOstapMeshCreator : public CDestructable
{
	CPaintingOstapMeshCreator();
};

struct CPaintingJesusMeshCreator : public CDestructable
{
	CPaintingJesusMeshCreator();
};

struct CJesusMeshCreator : public CCanvasMeshCreator
{
	CJesusMeshCreator();

	CMeshBuilderTex Canvas;
};

struct CPaintingLandscapeMeshCreator : public CDestructable
{
	CPaintingLandscapeMeshCreator();
};

struct CLandscapeMeshCreator : public CCanvasMeshCreator
{
	CLandscapeMeshCreator();

	CMeshBuilderTex Canvas;
};

struct CPaintingNapoleonLandscapeMeshCreator : public CDestructable
{
	CPaintingNapoleonLandscapeMeshCreator();
};

struct CNapoleonLandscapeMeshCreator : public CCanvasMeshCreator
{
	CNapoleonLandscapeMeshCreator();
};

struct CPaintingAdamMeshCreator : public CDestructable
{
	CPaintingAdamMeshCreator();
};

struct CAdamMeshCreator : public CCanvasMeshCreator
{
	CAdamMeshCreator();
};

struct CGirlMeshCreator
{
	CGirlMeshCreator();
};

#endif