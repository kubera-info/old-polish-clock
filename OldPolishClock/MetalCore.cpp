#include "DXUT.h"
#include "MetalCore.h"

static float TextPos;

void SetTex( VERTEXTEX & pVert )
{
	pVert.tu /= 2.0f;
	pVert.tu += TextPos;
}

CMetalMeshCreator::CMetalMeshCreator()
{
	TextPos = 0.0f;
	mMetal.ParseMesh( L"brass2.x" );
	TextPos = 0.5f;
	mMetal.ParseMesh( L"steel2.x" );
	mMetal.CommitMesh();
	mMetal.NormalizeMesh();
	mMetal.SaveMesh( L"metal.x" );
}

void CMetalMeshCreator::CMetal::OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
{
	VERTEXTEX newA( a ), newB( b ), newC( c );
	SetTex( newA );
	SetTex( newB );
	SetTex( newC );

	AddTriangle( newA, newB, newC );
}