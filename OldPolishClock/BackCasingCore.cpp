#include "DXUT.h"
#include "BackCasingCore.h"
#include "BackCasingCore-$$$.h"
#include "BackCasingCore-$$$2.h"

#define BACK_X (2143.0f)
#define BACK_Y (3355.0f)
const float fScale = .87f;
const float fZ = -.71f;
const float fDown = -.07f;
const float fDepth = .05f;

VERTEXTEX Tex( const VERTEXTEX & a )
{
	VERTEXTEX newA( a );
	newA.tu = ( a.Position.y + 1.0f ) / 2.0f;
	newA.tv = ( a.Position.x + 1.0f ) / 2.0f;

	return newA;
}

VERTEXTEX Place( const VERTEXTEX & a )
{
	VERTEXTEX newA( a );
	newA.Position.y *= fScale;
	newA.Position.y += fDown;
	newA.Position.x *= fScale;
	newA.Position.z += fZ;

	return newA;
}

VERTEXTEX MoveToBack( const VERTEXTEX & a )
{
	VERTEXTEX newA( a );
	newA.Position.z -= fDepth;

	return newA;
}

CBackCasingMeshCreator::CBackCasingMeshCreator()
{
	MakeBack( back );
	MakeBack2( back );
	back.CalculateCanonicalMesh();
	back.CommitMesh( false );
	back.NormalizeMesh();
	back.CleanMesh();
	back.WeldMesh( 0.003, 0.003 );
	back.SimplifyMesh( 32768 / 2 );
	back.WeldMesh( 0.003, 0.003 );

	back2.ParseMesh( back.GetMesh() );
	back2.m_bStage = false;
	back2.ParseMesh( back.GetMesh() );
	back2.CommitMesh( false, false );
	back2.SaveMesh( L"back.$$$" );

	back3.ParseMesh( L"back.$$$" );
	back3.CommitMesh( true, false );
	back3.SaveMesh( L"back2.$$$" );
}

void CBackCasingMeshCreator::CBack2::OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
{
	VERTEXTEX texA( Tex( a ) ), texB( Tex( b ) ), texC( Tex( c ) );
	if( m_bStage )
	{
		m_Lines.AddTriangle( texA, texB, texC );
		AddTriangle( texA, texB, texC, true );
	}
	else
	{
		InsertSideIfNeeded( texA, texB );
		InsertSideIfNeeded( texA, texC );
		InsertSideIfNeeded( texB, texC );
	}
}
void CBackCasingMeshCreator::CBack2::InsertSideIfNeeded( const VERTEXTEX & a, const VERTEXTEX & b )
{
	if( m_Lines.IsSide( a, b ) )
	{
		VERTEXTEX backA( MoveToBack( a ) ), backB( MoveToBack( b ) );
		AddTriangle( a, b, backB, true );
		AddTriangle( backB, backA, a, true );
		AddTriangle( b, a, backB, true );
		AddTriangle( backA, backB, a, true );
	}
}
void CBackCasingMeshCreator::CBack3::OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
{
	AddTriangle( Place( a ), Place( b ), Place( c ), true );
}

CBackCasingScaller::CBackCasingScaller()
{
	struct CScalledBack : public CMeshBuilderTex 
	{
		void Transform( VERTEXTEX & a )
		{
			const float fScale = 0.77f;
			a.Position.x *= fScale;
			a.Position.y *= fScale;
			// a.Position.y -= .08f;
		}
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
		{
			VERTEXTEX newA( a ), newB( b ), newC( c );

			Transform( newA );
			Transform( newB );
			Transform( newC );
			AddTriangle( newA, newB, newC, false );
		}
	} backScalled;

	backScalled.ParseMesh( L"back2.$$$" );
	backScalled.CommitMesh( false, false );
	backScalled.SaveMesh( L"back.x" );
}