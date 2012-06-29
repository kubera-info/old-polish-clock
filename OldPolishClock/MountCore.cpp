#include "DXUT.h"
#include "MountCore.h"

CMeshBuilderTex mMount2;

CMount::CMount()
{
	LPD3DXMESH pMesh( NULL );
	THROW_EXC_D3D( D3DXCreateCylinder(
		DXUTGetD3D9Device(),
		.036f,
		.036f,
		.3f,
		13,
		1,
		&pMesh,
		NULL ), L"Cylinder" );

	struct CMount1 : public CMeshBuilderPlain
	{
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
		{
			mMount2.AddTriangle( a, b, c );
		}
	} mMount1;
	mMount1.ParseMesh( pMesh );
	mMount2.CommitMesh();
	struct CMountTex : public CMeshBuilderTex
	{
		void Compute( VERTEXTEX & a )
		{
			a.tu = a.Position.z * 8 + a.Position.x / 29.0f;
			a.tv = a.Position.y * 32 + a.Position.x / 24.0f;

			XMFLOAT4X4 mTranslation, mRotation;
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( 0.0f, 0.39f, -0.96f ) );
			XMStoreFloat4x4( &mRotation, XMMatrixRotationX( -XM_PIDIV4 ) );

			XMStoreFloat3( &a.Position, XMVector3Transform( XMLoadFloat3( &a.Position ), XMLoadFloat4x4( &mRotation ) ) );
			XMStoreFloat3( &a.Position, XMVector3Transform( XMLoadFloat3( &a.Position ), XMLoadFloat4x4( &mTranslation ) ) );
			XMStoreFloat3( &a.Normal, XMVector3Transform( XMLoadFloat3( &a.Normal ), XMLoadFloat4x4( &mRotation ) ) );
		}
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
		{
			VERTEXTEX newA( a ), newB( b ), newC( c );

			Compute( newA );
			Compute( newB );
			Compute( newC );

			AddTriangle( newA, newB, newC );
		}
	} mMount3;
	mMount3.ParseMesh( mMount2.GetMesh() );
	mMount3.CommitMesh( false, false );
	mMount3.SaveMesh( L"mount.x" );
}