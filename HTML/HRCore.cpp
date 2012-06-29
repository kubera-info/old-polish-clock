#include "DXUT.h"
#include <vector>
#include "HRCore.h"
#include "MonadMath.h"

void Skew( VERTEXPLAIN & a )
{
	const float fSkewFactor = powf( abs( a.Position.x ), 3.0f );

	// a.Position.z *= fSkewFactor;
	a.Position.x *= fSkewFactor;
}
float x;
void Move(VERTEXPLAIN & Vert )
{
	Vert.Position.x += x;
}
CHRMeshCreator::CHRMeshCreator()
{
	CMeshBuilderPlain Collector;
	struct CMetal : public CMeshBuilderPlain
	{
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
		{			
			VERTEXPLAIN newA( a ), newB( b ), newC( c );

			Skew( newA );
			Skew( newB );
			Skew( newC );

			AddTriangle( newA, newB, newC, false );
		}
	} mMetal;


	VERTEXPLAIN center, top, down, left, right;
	center.Normal.x = 1.0f;
	center.Normal.y = 0.0f;
	center.Normal.z = 0.0f;
	center.Position.x = 0.0f;
	center.Position.y = 0.0f;
	center.Position.z = 0.0f;

	top = center;
	top.Position.y = .05f;

	left = center;
	left.Position.x = -1.0f;

	const float steps = 300.0f;

	std::vector<VERTEXPLAIN> VecOfVert;

	for( float i = 0.0f; i <= steps; i++ )
	{
		VERTEXPLAIN Vertex;

		Vertex.Normal = center.Normal;
		Vertex.Position.z = -0.05f;

		Vertex.Position.x = - left.Position.x * ( i / steps );
		Vertex.Position.y = top.Position.y * ( ( steps - i ) / steps );

		VecOfVert.push_back( Vertex );
	}

	for( int i = 0; i < steps; i++ )
	{
		VERTEXPLAIN x, y;
		x = VecOfVert[ i ];
		y = VecOfVert[ i + 1 ];
		Collector.AddTriangle( y, x, center );

		x.Position.y *= - 1.0f;
		y.Position.y *= - 1.0f;
		Collector.AddTriangle( x, y, center );

		x = VecOfVert[ i ];
		y = VecOfVert[ i + 1 ];
		x.Position.x *= - 1.0f;
		y.Position.x *= - 1.0f;
		Collector.AddTriangle( x, y, center );

		x.Position.y *= - 1.0f;
		y.Position.y *= - 1.0f;
		Collector.AddTriangle( y, x, center );
	}
	Collector.CommitMesh();

	mMetal.ParseMesh( Collector.GetMesh() );
	mMetal.CommitMesh( true, false );
	mMetal.NormalizeMesh();
	mMetal.SaveMesh( L"hr.x" );
}

CLIMeshCreator::CLIMeshCreator()
{
	CMeshBuilderPlain Collector;
	LPD3DXMESH pMesh( NULL );
	THROW_EXC_D3D( D3DXCreateSphere(
		DXUTGetD3D9Device(),
		.05f,
		64,
		64,
		&pMesh,
		NULL ), L"Sphere" );

	struct CMetal : public CMeshBuilderPlain
	{
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
		{			
			VERTEXPLAIN newA( a ), newB( b ), newC( c );

			Move( newA );
			Move( newB );
			Move( newC );

			AddTriangle( newA, newB, newC, false );
		}
	} mMetal;
	for( x = -2.0f; x < 2.0f; x += .13f)
		mMetal.ParseMesh( pMesh );
	mMetal.CommitMesh( true, false );
	mMetal.SaveMesh( L"li.x" );
};

void Buttonize( VERTEXTEX & Vert )
{
	Vert.Position.x = powf(abs(Vert.Position.x), .45) * MonadMath::SGN(Vert.Position.x);
	Vert.Position.y = powf(abs(Vert.Position.y), .45) * MonadMath::SGN(Vert.Position.y);
	Vert.tv = -( Vert.Position.y + 1.0f ) / 2.0f;
	Vert.tu = -( Vert.Position.x + 1.0f ) / 2.0f;
	Vert.Position.x *= 2;
}

CFlagCreator::CFlagCreator()
{
	struct CMetal : public CMeshBuilderTex
	{
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
		{			
			VERTEXTEX newA( a ), newB( b ), newC( c );

			Buttonize( newA );
			Buttonize( newB );
			Buttonize( newC );

			AddTriangle( newA, newB, newC, false );
		}
	} mMetal;

	mMetal.ParseMesh( L"clockface.x" );
	mMetal.CommitMesh( false, false );
	mMetal.NormalizeMesh();
	mMetal.SaveMesh( L"button.x" );
}

