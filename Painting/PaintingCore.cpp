#include "DXUT.h"
#include "PaintingCore.h"
#include "TessellationCore.h"

const float skala = .12f;
const float canvas_scale = 1.9f;
const float SizeFactor = 0.939f;
const float SizeJesusFactor = 1.0f;
const float SizeLandscapeFactor = 1.0f;
const float SizeNalopeonFactor = 1.0f;
const float SizeAdamFactor = .5f;
const float canvasX = 1107.0f;
const float canvasY = 767.0f;
const float canvasRatio = canvasY / canvasX;
const float biggest = 1.0f * canvas_scale;
const float biggesty = .71f * canvas_scale;
const float depth = -0.022f;
const float OuterEpsilon = 0.02f;	const float InnerEpsilon = 0.1f;
const float OuterZ = 0.03f;		const float InnerZ = - 0.07f;
const float BackZ = -.1525f;
const float TexFact = 0.73f;
const float PaintZ = .08f;

const float canvasJesusX = 2480.0f;
const float canvasJesusY = 3360.0f;
const float canvasJesusRatio = canvasJesusY / canvasJesusX;
const float canvasJesusScale = 2.0f;

const float canvasLandscapeX = 3740.0f;
const float canvasLandscapeY = 2552.0f;
const float canvasLandscapeRatio = canvasLandscapeY / canvasLandscapeX;
const float canvasLandscapeScale = 1.3f;

const float canvasNalopeonX = 3320.0f;
const float canvasNalopeonY = 2616.0f;
const float canvasNalopeonRatio = canvasLandscapeY / canvasLandscapeX;
const float canvasNalopeonScale = 1.15f;

const float canvasAdamX = 2100.0f;
const float canvasAdamY = 2970.0f;
const float canvasAdamRatio = canvasAdamY / canvasAdamX;
const float canvasAdamScale = .5f;

const float folderX = 3272.0f;
const float folderY = 4552.0f;
const float folderGirlRatio = folderY / folderX;
const float folderGirlScale = 1.4f;

const float fMetal = -32.0f;

struct CBorderMaker : public CMeshBuilderTex
{
	virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c ) throw(...)
	{
		VERTEXTEX newA( a ), newB( b ), newC( c );

		OnPaintingVertex( newA );
		OnPaintingVertex( newB );
		OnPaintingVertex( newC );

		if( fMetal == newA.tu )
			Metallic.AddTriangle( newA, newB, newC );
		else
			AddTriangle( newA, newB, newC );
	}
	virtual void OnPaintingVertex( VERTEXTEX & vertToAlter ) {}
	CMeshBuilderPlain Metallic;
};

float Big( float Input )
{
	return ( Input + 1.0f ) / 3.1f ;
}
void CPaintingMeshCreator::CBorderBuilder::AddTriangleHorizontal( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c )
{
	a.tu = Big( a.Position.z + a.Position.x );
	a.tv = Big( a.Position.z + a.Position.y );
	b.tu = Big( a.Position.z + b.Position.x );
	b.tv = Big( b.Position.y );
	c.tu = Big( a.Position.z + c.Position.x );
	c.tv = Big( c.Position.y );
	AddTriangle( a, b, c, true );
}
void CPaintingMeshCreator::CBorderBuilder::AddTriangleVertical( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c )
{
	a.tu = Big( a.Position.z + a.Position.y );
	a.tv = Big( a.Position.x );
	b.tu = Big( a.Position.z + b.Position.y );
	b.tv = Big( b.Position.x );
	c.tu = Big( a.Position.z + c.Position.y );
	c.tv = Big( c.Position.x );
	AddTriangle( a, b, c, true );
}
void CPaintingMeshCreator::CBorderBuilder::AddTriangleHorizontalDepth( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c )
{
	a.tu = Big( a.Position.z );
	a.tv = Big( a.Position.y );
	b.tu = Big( b.Position.z );
	b.tv = Big( b.Position.y );
	c.tu = Big( c.Position.z );
	c.tv = Big( c.Position.y );
	AddTriangle( a, b, c, true );
}
void CPaintingMeshCreator::CBorderBuilder::AddTriangleVerticalDepth( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c )
{
	a.tu = Big( a.Position.y );
	a.tv = Big( a.Position.z );
	b.tu = Big( b.Position.y );
	b.tv = Big( b.Position.z );
	c.tu = Big( c.Position.y );
	c.tv = Big( c.Position.z );
	AddTriangle( a, b, c, true );
}
void CPaintingMeshCreator::CBorderBuilder::AddTriangleMetallic( VERTEXTEX a, VERTEXTEX b, VERTEXTEX c )
{
	const XMFLOAT3 CanonicalNormal( 1.0f, 0.0f, 0.0f );;
	a.tu = fMetal;
	a.tv = fMetal;
	b.tu = fMetal;
	b.tv = fMetal;
	c.tu = fMetal;
	c.tv = fMetal;
	a.Normal = CanonicalNormal;
	b.Normal = CanonicalNormal;
	c.Normal = CanonicalNormal;
	AddTriangle( a, b, c, false );
}

CPaintingMeshCreator::CPaintingMeshCreator()
{
	MonadMath::CEllipse El(6, skala, skala /5, M_PI);
	for( float iR = 0; El.GetSidesCnt() > iR; iR ++ )
	{
		const float to_front = .12f;
		float const move = skala * iR / ( El.GetSidesCnt() * 2 );
		float const move2 = skala * ( iR + 1 ) / ( El.GetSidesCnt() * 2 );

		LeftUpper.Normal = RightUpper.Normal = LeftLower.Normal = RightLower.Normal = XMFLOAT3( 0.0f, 0.0f, 1.0f );
		LeftUpper.Position.z = RightUpper.Position.z = -El[ (DWORD) iR ].y + to_front; 
		LeftLower.Position.z = RightLower.Position.z = -El[ (DWORD) iR + 1 ].y + to_front;

		LeftUpper.Position.x = -biggest + move;
		RightUpper.Position.x = -LeftUpper.Position.x;
		LeftUpper.Position.y = RightUpper.Position.y = biggesty - move;

		LeftLower.Position.x = -biggest + move2;
		RightLower.Position.x = -LeftLower.Position.x;
		LeftLower.Position.y = RightLower.Position.y = biggesty - move2;

		/*LeftLower.tu = LeftLower.Position.x; LeftLower.tv = LeftLower.Position.y;
		LeftUpper.tu = LeftUpper.Position.x; LeftUpper.tv = LeftUpper.Position.y;
		RightLower.tu = RightLower.Position.x; RightLower.tv = RightLower.Position.y;
		RightUpper.tu = RightUpper.Position.x; RightUpper.tv = RightUpper.Position.y;*/

		Casing.AddTriangleMetallic( LeftUpper, LeftLower, RightUpper );
		Casing.AddTriangleMetallic( LeftLower, RightLower, RightUpper );

		LeftUpper2 = LeftUpper; RightUpper2 = RightUpper;
		RightLower2 = RightLower; LeftLower2 = LeftLower;

		LeftUpper2.Position.y = RightUpper2.Position.y = -RightUpper.Position.y;
		LeftLower2.Position.y = RightLower2.Position.y = -RightLower.Position.y;

		/*LeftLower2.tu = LeftLower2.Position.x; LeftLower2.tv = LeftLower2.Position.y;
		LeftUpper2.tu = LeftUpper2.Position.x; LeftUpper2.tv = LeftUpper2.Position.y;
		RightLower2.tu = RightLower2.Position.x; RightLower2.tv = RightLower2.Position.y;
		RightUpper2.tu = RightUpper2.Position.x; RightUpper2.tv = RightUpper2.Position.y;*/

		Casing.AddTriangleMetallic( LeftUpper2, RightUpper2, LeftLower2 );
		Casing.AddTriangleMetallic( LeftLower2, RightUpper2, RightLower2 );

		/*LeftLower.tv = LeftLower.Position.x; LeftLower.tu = LeftLower.Position.y;
		LeftUpper.tv = LeftUpper.Position.x; LeftUpper.tu = LeftUpper.Position.y;
		RightLower.tv = RightLower.Position.x; RightLower.tu = RightLower.Position.y;
		RightUpper.tv = RightUpper.Position.x; RightUpper.tu = RightUpper.Position.y;

		LeftLower2.tv = LeftLower2.Position.x; LeftLower2.tu = LeftLower2.Position.y;
		LeftUpper2.tv = LeftUpper2.Position.x; LeftUpper2.tu = LeftUpper2.Position.y;
		RightLower2.tv = RightLower2.Position.x; RightLower2.tu = RightLower2.Position.y;
		RightUpper2.tv = RightUpper2.Position.x; RightUpper2.tu = RightUpper2.Position.y;*/

		Casing.AddTriangleMetallic( LeftUpper, LeftLower2, LeftLower );
		Casing.AddTriangleMetallic( LeftLower2, LeftUpper, LeftUpper2 );

		Casing.AddTriangleMetallic( RightUpper, RightLower, RightLower2 );
		Casing.AddTriangleMetallic( RightLower2, RightUpper2, RightUpper );

		if( 0.0f == iR )
		{
			BackLeftUpper = LeftUpper;
			BackLeftUpper2 = LeftUpper2;
			BackRightUpper = RightUpper;
			BackRightUpper2 = RightUpper2;

			BackLeftLower = LeftLower;
			BackLeftLower2 = LeftLower2;
			BackRightLower = RightLower;
			BackRightLower2 = RightLower2;

			BackLeftUpper.Position.z = (BackLeftUpper2.Position.z += depth );
			BackRightUpper.Position.z = (BackRightUpper2.Position.z += depth );
			BackLeftLower.Position.z = (BackLeftLower2.Position.z += depth );
			BackRightLower.Position.z = (BackRightLower2.Position.z += depth );

			VERTEXPLAIN LeftUpperPlain = LeftUpper;
			VERTEXPLAIN BackLeftUpper2Plain = BackLeftUpper2;
			VERTEXPLAIN LeftUpper2Plain = LeftUpper2;
			VERTEXPLAIN BackLeftUpperPlain = BackLeftUpper;

			//------------------------

			VERTEXPLAIN RightUpperPlain = RightUpper;
			VERTEXPLAIN RightUpper2Plain = RightUpper2;
			VERTEXPLAIN BackRightUpper2Plain = BackRightUpper2;
			VERTEXPLAIN BackRightUpperPlain = BackRightUpper;

			//------------------------
			Proc2();
		}
		else if( (float) El.GetSidesCnt() -1 == iR )
		{
			BackLeftLower = LeftLower;
			BackLeftLower2 = LeftLower2;
			BackRightLower = RightLower;
			BackRightLower2 = RightLower2;

			BackLeftUpper = LeftUpper;
			BackLeftUpper2 = LeftUpper2;
			BackRightUpper = RightUpper;
			BackRightUpper2 = RightUpper2;

			BackLeftLower.Position.z = (BackLeftLower2.Position.z += depth );
			BackLeftUpper.Position.z = (BackLeftUpper2.Position.z += depth );
			BackRightLower.Position.z = (BackRightLower2.Position.z += depth );
			BackRightUpper.Position.z = (BackRightUpper2.Position.z += depth );

			Casing.AddTriangleVertical( LeftLower, LeftLower2, BackLeftLower2 );
			Casing.AddTriangleVertical( BackLeftLower2, BackLeftLower, LeftLower );

			//------------------------

			Casing.AddTriangleVertical( RightLower, BackRightLower2, RightLower2 );
			Casing.AddTriangleVertical( BackRightLower2, RightLower, BackRightLower );

			//------------------------

			Proc1();
			Proc3();
			Proc2();
			Proc1();
		}
	}

	LeftUpper2 = RightUpper2; LeftUpper2.Position.x *= -1;
	BackLeftUpper2 = BackRightUpper2; BackLeftUpper2.Position.x *= -1;
	LeftLower2 = RightLower2; LeftLower2.Position.x *= -1;
	BackLeftLower2 = BackRightLower2; BackLeftLower2.Position.x *= -1;
	Casing.AddTriangleVerticalDepth( RightUpper2, BackRightLower2, RightLower2 );
	Casing.AddTriangleVerticalDepth( BackRightLower2, RightUpper2, BackRightUpper2 );
	Casing.AddTriangleVerticalDepth( BackLeftLower2, LeftUpper2, LeftLower2 );
	Casing.AddTriangleVerticalDepth( LeftUpper2, BackLeftLower2, BackLeftUpper2 );

	Casing.AddTriangleHorizontalDepth( RightUpper2, LeftUpper2, BackRightUpper2 );
	Casing.AddTriangleHorizontalDepth( BackLeftUpper2, BackRightUpper2, LeftUpper2 );
	Casing.AddTriangleHorizontalDepth( LeftLower2, RightLower2, BackRightLower2 );
	Casing.AddTriangleHorizontalDepth( BackRightLower2, BackLeftLower2, LeftLower2 );

	Casing.AddTriangleVerticalDepth( BackRightLower, RightUpper, RightLower );
	Casing.AddTriangleVerticalDepth( RightUpper, BackRightLower, BackRightUpper );
	Casing.AddTriangleVerticalDepth( LeftUpper, BackLeftLower, LeftLower );
	Casing.AddTriangleVerticalDepth( BackLeftLower, LeftUpper, BackLeftUpper );

	Casing.AddTriangleHorizontalDepth( LeftUpper, RightUpper, BackRightUpper );
	Casing.AddTriangleHorizontalDepth( BackRightUpper, BackLeftUpper, LeftUpper );
	Casing.AddTriangleHorizontalDepth( RightLower, LeftLower, BackRightLower );
	Casing.AddTriangleHorizontalDepth( BackLeftLower, BackRightLower, LeftLower );

	Casing.CalculateCanonicalWallMesh();

	Casing.CommitMesh();
}
void CPaintingMeshCreator::Proc1()
{
	Casing.AddTriangleHorizontal( RightLower, LeftLower, BackLeftLower );
	Casing.AddTriangleHorizontal( RightLower, BackLeftLower, BackRightLower );

	//------------------------
	Casing.AddTriangleHorizontal( RightLower2, BackLeftLower2, LeftLower2 );
	Casing.AddTriangleHorizontal( RightLower2, BackRightLower2, BackLeftLower2 );
}
void CPaintingMeshCreator::Proc2()
{
	Casing.AddTriangleHorizontal( RightUpper2, LeftUpper2, BackLeftUpper2 );
	Casing.AddTriangleHorizontal( RightUpper2, BackLeftUpper2, BackRightUpper2 );

	//------------------------
	Casing.AddTriangleHorizontal( LeftUpper, BackRightUpper, BackLeftUpper );
	Casing.AddTriangleHorizontal( BackRightUpper, LeftUpper, RightUpper );
}

void CPaintingMeshCreator::Proc3()
{
	BackRightLower.Position.x = RightLower.Position.x = biggest + OuterEpsilon;
	BackRightUpper.Position.x = RightUpper.Position.x = biggest + OuterEpsilon;
	BackLeftLower.Position.x = LeftLower.Position.x = - biggest - OuterEpsilon;
	BackLeftUpper.Position.x = LeftUpper.Position.x = - biggest - OuterEpsilon;

	BackRightLower.Position.y = RightLower.Position.y = - biggesty - OuterEpsilon;
	BackRightUpper.Position.y = RightUpper.Position.y = biggesty + OuterEpsilon;
	BackLeftLower.Position.y = LeftLower.Position.y = - biggesty - OuterEpsilon;
	BackLeftUpper.Position.y = LeftUpper.Position.y = biggesty + OuterEpsilon;

	RightLower.Position.z += OuterZ;
	RightUpper.Position.z += OuterZ;
	LeftLower.Position.z += OuterZ;
	LeftUpper.Position.z += OuterZ;

	BackRightLower.Position.z = BackZ;
	BackRightUpper.Position.z = BackZ;
	BackLeftLower.Position.z = BackZ;
	BackLeftUpper.Position.z = BackZ;

	BackRightLower2.Position.x = RightLower2.Position.x = biggest - InnerEpsilon;
	BackRightUpper2.Position.x = RightUpper2.Position.x = biggest - InnerEpsilon;
	BackLeftLower2.Position.x = LeftLower2.Position.x = - biggest + InnerEpsilon;
	BackLeftUpper2.Position.x = LeftUpper2.Position.x = - biggest + InnerEpsilon;

	BackRightLower2.Position.y = RightLower2.Position.y = - biggesty + InnerEpsilon;
	BackRightUpper2.Position.y = RightUpper2.Position.y = biggesty - InnerEpsilon;
	BackLeftLower2.Position.y = LeftLower2.Position.y = - biggesty + InnerEpsilon;
	BackLeftUpper2.Position.y = LeftUpper2.Position.y = biggesty - InnerEpsilon;

	RightLower2.Position.z += InnerZ;
	RightUpper2.Position.z += InnerZ;
	LeftLower2.Position.z += InnerZ;
	LeftUpper2.Position.z += InnerZ;

	BackRightLower2.Position.z = BackZ;
	BackRightUpper2.Position.z = BackZ;
	BackLeftLower2.Position.z = BackZ;
	BackLeftUpper2.Position.z = BackZ;

	Casing.AddTriangleVertical( LeftUpper, LeftLower2, LeftUpper2 );
	Casing.AddTriangleVertical( LeftUpper, LeftLower, LeftLower2 );

	Casing.AddTriangleVertical( RightUpper2, RightLower, RightUpper );
	Casing.AddTriangleVertical( RightUpper2, RightLower2, RightLower );

	Casing.AddTriangleHorizontal( LeftUpper, RightUpper2, RightUpper );
	Casing.AddTriangleHorizontal( LeftUpper, LeftUpper2, RightUpper2 );

	//------------------------
	Casing.AddTriangleHorizontal( LeftLower2, LeftLower, RightLower );
	Casing.AddTriangleHorizontal( LeftLower2, RightLower, RightLower2 );

	Proc1();
	Proc2();
}

CCanvasMeshCreator::CCanvasMeshCreator( const std::wstring & pPath, const float pCanvasRatio, const float pCanvasScale )
{
	CStaticTesselation Tess( pCanvasScale * 2.0f, pCanvasRatio * pCanvasScale * 2.0f, PaintZ, 2.0f, 2.0f );

	Canvas.CloneFromMesh( Tess.Box.GetMesh() );
	Canvas.CommitMesh();
	Canvas.NormalizeMesh();
	Canvas.SaveMesh( pPath );
};

CPaintingOstapMeshCreator::CPaintingOstapMeshCreator()
{
	struct COstapBorder : public CBorderMaker
	{
	} OstapBorder;
	CPaintingMeshCreator x;
	OstapBorder.ParseMesh( x.Casing.GetMesh() );
	OstapBorder.CommitMesh();
	OstapBorder.Metallic.CommitMesh();
	OstapBorder.Metallic.NormalizeMesh();
	OstapBorder.SaveMesh( L"ostap_border.x" );
	OstapBorder.Metallic.SaveMesh( L"ostap_metallic.x" );
}

CPaintingJesusMeshCreator::CPaintingJesusMeshCreator()
{
	struct CJesusBorder : public CBorderMaker
	{
		void OnPaintingVertex( VERTEXTEX & vertToAlter )
		{
			static const float xFactor = .04f;
			if( vertToAlter.Position.x < 0.0f )
				vertToAlter.Position.x -= xFactor;
			else
				vertToAlter.Position.x += xFactor;
			static const float yFactor = .7f;
			if( vertToAlter.Position.y < 0.0f )
				vertToAlter.Position.y -= yFactor;
			else
				vertToAlter.Position.y += yFactor;
		}
	} JesusBorder;
	CPaintingMeshCreator x;
	JesusBorder.ParseMesh( x.Casing.GetMesh() );
	JesusBorder.CommitMesh();
	JesusBorder.Metallic.CommitMesh();
	JesusBorder.Metallic.NormalizeMesh();
	JesusBorder.SaveMesh( L"jesus_border.x" );
	JesusBorder.Metallic.SaveMesh( L"jesus_metallic.x" );
}

CImageMeshCreator::CImageMeshCreator()
: CCanvasMeshCreator( L"canvas.x", canvasRatio, SizeFactor )
{
};

CJesusMeshCreator::CJesusMeshCreator()
: CCanvasMeshCreator( L"jesus_canvas.x", canvasJesusRatio, SizeJesusFactor )
{
};

CPaintingLandscapeMeshCreator::CPaintingLandscapeMeshCreator()
{
	struct CJesusBorder : public CBorderMaker
	{
		void OnPaintingVertex( VERTEXTEX & vertToAlter )
		{
			static const float xFactor = .04f;
			if( vertToAlter.Position.x < 0.0f )
				vertToAlter.Position.x -= xFactor;
			else
				vertToAlter.Position.x += xFactor;
			static const float yFactor = .0f;
			if( vertToAlter.Position.y < 0.0f )
				vertToAlter.Position.y -= yFactor;
			else
				vertToAlter.Position.y += yFactor;
		}
	} JesusBorder;
	CPaintingMeshCreator x;
	JesusBorder.ParseMesh( x.Casing.GetMesh() );
	JesusBorder.CommitMesh();
	JesusBorder.Metallic.CommitMesh();
	JesusBorder.Metallic.NormalizeMesh();
	JesusBorder.SaveMesh( L"landscape_border.x" );
	JesusBorder.Metallic.SaveMesh( L"landscape_metallic.x" );
}

CLandscapeMeshCreator::CLandscapeMeshCreator()
: CCanvasMeshCreator( L"landscape_canvas.x", canvasLandscapeRatio, SizeLandscapeFactor )
{
};

CPaintingNapoleonLandscapeMeshCreator::CPaintingNapoleonLandscapeMeshCreator()
{
	struct CJesusBorder : public CBorderMaker
	{
		void OnPaintingVertex( VERTEXTEX & vertToAlter )
		{
			static const float xFactor = .04f;
			if( vertToAlter.Position.x < 0.0f )
				vertToAlter.Position.x -= xFactor;
			else
				vertToAlter.Position.x += xFactor;
			static const float yFactor = .023f;
			if( vertToAlter.Position.y < 0.0f )
				vertToAlter.Position.y -= yFactor;
			else
				vertToAlter.Position.y += yFactor;
		}
	} JesusBorder;
	CPaintingMeshCreator x;
	JesusBorder.ParseMesh( x.Casing.GetMesh() );
	JesusBorder.CommitMesh();
	JesusBorder.Metallic.CommitMesh();
	JesusBorder.Metallic.NormalizeMesh();
	JesusBorder.SaveMesh( L"napoleon_border.x" );
	JesusBorder.Metallic.SaveMesh( L"napoleon_metallic.x" );
}

CNapoleonLandscapeMeshCreator::CNapoleonLandscapeMeshCreator()
: CCanvasMeshCreator( L"napoleon.x", canvasRatio, SizeNalopeonFactor )
{
};

CPaintingAdamMeshCreator::CPaintingAdamMeshCreator()
{
	struct CJesusBorder : public CBorderMaker
	{
		void OnPaintingVertex( VERTEXTEX & vertToAlter )
		{
			static const float xFactor = -.44f;
			if( vertToAlter.Position.x < 0.0f )
				vertToAlter.Position.x -= xFactor;
			else
				vertToAlter.Position.x += xFactor;
			static const float yFactor = .023f;
			if( vertToAlter.Position.y < 0.0f )
				vertToAlter.Position.y -= yFactor;
			else
				vertToAlter.Position.y += yFactor;
		}
	} JesusBorder;
	CPaintingMeshCreator x;
	JesusBorder.ParseMesh( x.Casing.GetMesh() );
	JesusBorder.CommitMesh();
	JesusBorder.Metallic.CommitMesh();
	JesusBorder.Metallic.NormalizeMesh();
	JesusBorder.SaveMesh( L"adam_border.x" );
	JesusBorder.Metallic.SaveMesh( L"adam_metallic.x" );
}

CAdamMeshCreator::CAdamMeshCreator()
: CCanvasMeshCreator( L"adam.x", canvasAdamRatio, SizeAdamFactor )
{
};

CGirlMeshCreator::CGirlMeshCreator()
{
	CStaticTesselation Tess( folderGirlScale, folderGirlRatio * folderGirlScale, PaintZ, 16.0f, 128.0f );

	CMeshBuilderTex Folder;

	Folder.CloneFromMesh( Tess.Box.GetMesh() );
	Folder.CommitMesh();
	Folder.NormalizeMesh();
	Folder.SaveMesh( L"folder.x" );
}