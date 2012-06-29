#include "DXUT.h"
#include <XNAMath.h>
#include "NailCore.h"
#include "MonadMath.h"
#include "MonadTemplates.h"

typedef std::vector<VERTEXPLAIN> CVerts;
CVerts a( 6 );
float radius, top, bottom;

static const float side_x = .753f;
static const float side_z = .168f;

static const float wooden_x = .726f;
static const float wooden_z = 0.164f;
static const float wooden_y = 0.896f;

void Pyramid( CMeshBuilderPlain & NailCreator, bool bAddBottom = false )
{
	VERTEXPLAIN center;

	CVerts newA = a;
	center.Position.z = top;
	center.Position.x = 0.0;
	center.Position.y = 0.0;

	BOOST_FOREACH( VERTEXPLAIN & Vert, newA )
	{
		Vert.Position.x *= radius;
		Vert.Position.y *= radius;
		Vert.Position.z = bottom;
	}

	NailCreator.AddTriangle( center, newA[ 0 ], newA[ 1 ], true );
	NailCreator.AddTriangle( center, newA[ 2 ], newA[ 3 ], true );
	NailCreator.AddTriangle( center, newA[ 4 ], newA[ 5 ], true );

	if( bAddBottom )
	{
		NailCreator.AddTriangle( newA[ 2 ], newA[ 0 ], newA[ 4 ], true );
	}
}

CCreateNail::CCreateNail()
{
	MonadMath::CEllipse Triangle( 3, 1.0f, 1.0f );
	CMeshBuilderPlain Nail;

	int iTriangleSide( 0 );
	for( int iSide = 0; iSide < 6; iSide += 2 )
	{
		a[ iSide ].Position.x = Triangle[ iTriangleSide ].x;
		a[ iSide ].Position.y = Triangle[ iTriangleSide ].y;

		a[ iSide + 1 ].Position.x = Triangle[ ( iTriangleSide + 1 ) % 3 ].x;
		a[ iSide + 1 ].Position.y = Triangle[ ( iTriangleSide + 1 ) % 3 ].y;

		++ iTriangleSide;
	}

	bottom = .9f;
	top = -.2f;
	radius = .09f;
	Pyramid( Nail );
	bottom = 1.0f;
	top = .78f;
	radius = .29f;
	Pyramid( Nail, true );
	Nail.CalculateCanonicalMesh();
	Nail.CommitMesh();


	struct CScaleMesh : public CMeshBuilderPlain
	{
		void ScaleVert( VERTEXPLAIN & pVert )
		{
			const float fScale = 1.0f / 35.0f;
			pVert.Position.x *= fScale;
			pVert.Position.y *= fScale;
			pVert.Position.z *= fScale;
		}
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
		{
			VERTEXPLAIN newA( a );
			VERTEXPLAIN newB( b );
			VERTEXPLAIN newC( c );

			ScaleVert( newA );
			ScaleVert( newB );
			ScaleVert( newC );

			AddTriangle( newA, newB, newC );
		}
	} ScalledNail;
	ScalledNail.ParseMesh( Nail.GetMesh() );
	ScalledNail.CommitMesh();
	ScalledNail.SaveMesh( L"nails.$$$" );

	struct CNail : public CMeshBuilderPlain
	{
		CNail()
		{
			XMMATRIX Ident = XMMatrixIdentity();
			XMStoreFloat4x4( &mTranslation, Ident );
			XMStoreFloat4x4( &mRotation, Ident );
		}
		void OnVert( VERTEXPLAIN & a )
		{
			XMStoreFloat3( &a.Position, XMVector3Transform( XMLoadFloat3( &a.Position ), XMLoadFloat4x4( &mRotation ) ) );
			XMStoreFloat3( &a.Position, XMVector3Transform( XMLoadFloat3( &a.Position ), XMLoadFloat4x4( &mTranslation ) ) );
			XMStoreFloat3( &a.Normal, XMVector3Transform( XMLoadFloat3( &a.Normal ), XMLoadFloat4x4( &mRotation ) ) );
		}
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
		{
			VERTEXPLAIN newA( a ), newB( b ), newC( c );
			OnVert( newA );
			OnVert( newB );
			OnVert( newC );

			AddTriangle( newA, newB, newC );
		}

		XMFLOAT4X4 mTranslation;
		XMFLOAT4X4 mRotation;
	};
	/*struct CTopBack : public CNail
	{
		CTopBack()
		{
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( 0.0f, 0.154f, -.0141f ) );
		}
	} TopBack;
	struct CBottomBack : public CNail
	{
		CBottomBack()
		{
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( 0.0f, -0.612f, -.0142f ) );
			XMStoreFloat4x4( &mRotation, XMMatrixRotationX( -.38f ) );
		}
	} BottomBack;
	TopBack.ParseMesh( ScalledNail.GetMesh() );
	TopBack.CommitMesh();
	BottomBack.ParseMesh( ScalledNail.GetMesh() );
	BottomBack.CommitMesh();

	CMeshBuilderPlain Back;
	Back.ParseMesh( TopBack.GetMesh() );
	Back.ParseMesh( BottomBack.GetMesh() );
	Back.CommitMesh();
	Back.SaveMesh( L"nails.x" );*/
	/*struct CTopBack : public CNail
	{
		CTopBack()
		{
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( side_x, 0.6f, side_z ) );
			XMStoreFloat4x4( &mRotation, XMMatrixRotationY( XM_PIDIV2 ) );
		}
	} TopBack;
	struct CBottomBack : public CNail
	{
		CBottomBack()
		{
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( side_x, -0.587f, side_z ) );
			XMStoreFloat4x4( &mRotation, XMMatrixRotationY( XM_PIDIV2 ) );
		}
	} BottomBack;
	TopBack.ParseMesh( ScalledNail.GetMesh() );
	TopBack.CommitMesh();
	BottomBack.ParseMesh( ScalledNail.GetMesh() );
	BottomBack.CommitMesh();

	CMeshBuilderPlain Back;
	Back.ParseMesh( TopBack.GetMesh() );
	Back.ParseMesh( BottomBack.GetMesh() );
	Back.CommitMesh();
	Back.SaveMesh( L"side_nails.x" );*/


	struct CLeftWooden : public CNail
	{
		CLeftWooden()
		{
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( wooden_x, 0.0f, wooden_z ) );
			XMStoreFloat4x4( &mRotation, XMMatrixMultiply( XMMatrixRotationY( - XM_PIDIV2 ), XMMatrixRotationZ( - XM_PIDIV4 ) ) );
		}
	} LeftWooden;
	struct CRightWooden : public CNail
	{
		CRightWooden()
		{
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( -wooden_x, 0.0f, wooden_z ) );
			XMStoreFloat4x4( &mRotation, XMMatrixMultiply( XMMatrixRotationY( - XM_PIDIV2 ), XMMatrixRotationZ( - XM_PIDIV4 + XM_PI ) ) );
		}
	} RightWooden;
	struct CTopWooden : public CNail
	{
		CTopWooden()
		{
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( 0.0f, wooden_y, wooden_z ) );
			XMStoreFloat4x4( &mRotation, XMMatrixMultiply( XMMatrixRotationY( - XM_PIDIV2 ), XMMatrixRotationZ( + XM_PIDIV4 + XM_PI + XM_PI ) ) );
		}
	} TopWooden;
	struct CBottomWooden : public CNail
	{
		CBottomWooden()
		{
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( 0.0f, -wooden_y, wooden_z ) );
			XMStoreFloat4x4( &mRotation, XMMatrixMultiply( XMMatrixRotationY( - XM_PIDIV2 ), XMMatrixRotationZ( + XM_PIDIV4 + XM_PI + XM_PI + XM_PI ) ) );
		}
	} BottomWooden;
	LeftWooden.ParseMesh( ScalledNail.GetMesh() );
	LeftWooden.CommitMesh();
	RightWooden.ParseMesh( ScalledNail.GetMesh() );
	RightWooden.CommitMesh();
	TopWooden.ParseMesh( ScalledNail.GetMesh() );
	TopWooden.CommitMesh();
	BottomWooden.ParseMesh( ScalledNail.GetMesh() );
	BottomWooden.CommitMesh();
	CMeshBuilderPlain Wooden;
	Wooden.ParseMesh( LeftWooden.GetMesh() );
	Wooden.ParseMesh( RightWooden.GetMesh() );
	Wooden.ParseMesh( TopWooden.GetMesh() );
	Wooden.ParseMesh( BottomWooden.GetMesh() );
	Wooden.CommitMesh();
	Wooden.SaveMesh( L"wooden_nails.x" );
}