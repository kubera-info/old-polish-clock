#include "DXUT.h"
#include "MonadMath.h"
#include "BoxCasingCore.h"
#include "MeshCore.h"

const float fRadius = .03f;
const float fFar = -.72f;
const float fNear = -.028f;
const float xMove = .38f + .016f;
const float fMinusY = -.685f;
const float fPlusY = .21f;

CBoxMeshCreator::CBoxMeshCreator()
{
	MonadMath::CEllipse Side( 16, fRadius, fRadius, XM_PI, XM_PIDIV2 );

	CMeshBuilderTex BlickBoxBuilder;
	for( DWORD i = 0; i < Side.GetSidesCnt(); i++ )
	{
		VERTEXTEX a, b, c, d;

		a.Normal.x = 0.0f;
		b.Normal.x = 0.0f;
		c.Normal.x = 0.0f;

		a.Normal.y = 1.0f;
		b.Normal.y = 1.0f;
		c.Normal.y = 1.0f;

		a.Normal.z = 0.0f;
		b.Normal.z = 0.0f;
		c.Normal.z = 0.0f;
		a.tu = 0.0f;
		a.tv = 0.0f;
		b.tu = 0.0f;
		b.tv = 0.0f;
		c.tu = 0.0f;
		c.tv = 0.0f;
		d.tu = 0.0f;
		d.tv = 0.0f;

		a.Position.x = -Side[ i ].x;
		b.Position.x = -Side[ i ].x;

		c.Position.x = -Side[ i + 1 ].x;
		d.Position.x = -Side[ i + 1 ].x;

		a.Position.y = Side[ i ].y;
		b.Position.y = Side[ i ].y;

		c.Position.y = Side[ i + 1 ].y;
		d.Position.y = Side[ i + 1 ].y;

		a.Position.z = d.Position.z = fFar;
		b.Position.z = c.Position.z = fNear;

		BlickBoxBuilder.AddTriangle( a, c, b, true );
		BlickBoxBuilder.AddTriangle( c, a, d, true );
	}
	BlickBoxBuilder.CommitMesh( false, false );
	BlickBoxBuilder.SaveMesh( L"box.$$$" );

	struct CBoxSide : public CMeshBuilderTex
	{
		void Transform( VERTEXTEX & a )
		{
			if( a.Position.y < 0.0f )
				a.Position.y += fMinusY;
			else
				a.Position.y += fPlusY;
			a.Position.x += xMove;
			a.tu = a.Position.y;
			a.tv = a.Position.z;
		}
		void Mirror( VERTEXTEX & a )
		{
			a.Position.x *= -1.0f;
		}

		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
		{
			VERTEXTEX newA( a ), newB( b ), newC( c );

			Transform( newA );
			Transform( newB );
			Transform( newC );

			AddTriangle( newA, newB, newC, false );

			Mirror( newA );
			Mirror( newB );
			Mirror( newC );

			AddTriangle( newA, newC, newB, false );
		}
	} mSides;

	mSides.ParseMesh( L"box.$$$" );
	mSides.CommitMesh();
	mSides.NormalizeMesh();
	mSides.SaveMesh( L"box.x" );
}

CTopMeshCreator::CTopMeshCreator()
{
	VERTEXTEX a, b, c, d;
	VERTEXTEX a2, b2, c2, d2;

	CMeshBuilderTex TopBuilder;

	a.Position.z = fFar;
	d.Position.z = fFar;
	b.Position.z = fNear;
	c.Position.z = fNear;
	a.Position.x = b.Position.x = xMove;
	c.Position.x = d.Position.x = -xMove;
	a.Position.y = b.Position.y = c.Position.y = d.Position.y = fPlusY + .03f;

	a.tu = 0.0f;
	a.tv = 0.0f;
	b.tu = 1.0f;
	b.tv = 0.0f;
	c.tu = 1.0f;
	c.tv = 1.0f;
	d.tu = 0.0f;
	d.tv = 1.0f;

	TopBuilder.AddTriangle( a, c, b, true );
	TopBuilder.AddTriangle( c, a, d, true );

	a2 = a; b2 = b; c2 = c; d2 = d;

	a.Position.y -= .05f;
	d.Position.y = c.Position.y = b.Position.y = a.Position.y;
	
	TopBuilder.AddTriangle( a, a2, b2, true );
	TopBuilder.AddTriangle( b2, b, a, true );

	TopBuilder.AddTriangle( d, c2, d2, true );
	TopBuilder.AddTriangle( c2, d, c, true );

	a.Position.y = b.Position.y = c.Position.y = d.Position.y = fMinusY - .027f;

	TopBuilder.AddTriangle( a, b, c, true );
	TopBuilder.AddTriangle( c, d, a, true );

	a2 = a; b2 = b; c2 = c; d2 = d;

	a.Position.y += .05f;
	d.Position.y = c.Position.y = b.Position.y = a.Position.y;

	TopBuilder.AddTriangle( a, b2, a2, true );
	TopBuilder.AddTriangle( b2, a, b, true );

	TopBuilder.AddTriangle( d, d2, c2, true );
	TopBuilder.AddTriangle( c2, c, d, true );

	TopBuilder.CommitMesh();
	TopBuilder.SaveMesh( L"top.x" );
}