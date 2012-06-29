#include "DXUT.H"
#include "PendulumCore.h"
#include "Patterns.h"
#include "MeshCore.h"

CMeshBuilderPlain mConverter;

XMFLOAT4X4 mTransform;

CPendulumMeshCreator::CPendulumMeshCreator()
{
	CMeshBuilderPlain mStick;
	CMeshBuilderPlain mCircle;
	CMeshBuilderPlain mThread;
	CMeshBuilderPlain mLock;

	struct CPendulumBuilder : public CMeshBuilderPlain
	{
		void TransformMe( VERTEXPLAIN & a )
		{
			static const XMMATRIX Translate = XMMatrixTranslation( .0f, -.48f, -.5f );
			static const float fWholeScale = 1.05f;
			static const XMMATRIX Scale = XMMatrixScaling( 1.05f, 1.05f, 1.05f );
			static const XMMATRIX Transform = XMMatrixMultiply( Scale, Translate );
			XMStoreFloat3( &a.Position, XMVector3Transform( XMLoadFloat3( &a.Position ), XMMatrixMultiply( XMLoadFloat4x4( &mTransform ), Transform ) ) );
		}
		virtual void OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
		{
			VERTEXPLAIN newA( a ), newB( b ), newC( c );

			TransformMe( newA );
			TransformMe( newB );
			TransformMe( newC );

			AddTriangle( newA, newB, newC, false );			
		}
	} mPendulum;

	struct CConverter : public CMeshBuilderTex
	{
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
		{
			mConverter.AddTriangle( a, b, c );
		}
	} mConverterTmp;

	mConverterTmp.ParseMesh( L"clockface.x" );
	mConverter.CommitMesh( false, false );

	{
		VERTEXPLAIN n1, n2, n3, n4;
		n1.Normal = n2.Normal, n3.Normal = n4.Normal = XMFLOAT3( 1.0f, 0.0f, 0.0f );

		const float PlusX = .012f;
		const float MinusX = - PlusX;
		const float Down = -1.25f;
		const float Back = -.002f;

		n1.Position = XMFLOAT3( PlusX, 0.0f, 0.0f );
		n2.Position = XMFLOAT3( MinusX, 0.0f, 0.0f );
		n3.Position = XMFLOAT3( MinusX, Down, 0.0f );
		n4.Position = XMFLOAT3( PlusX, Down, 0.0f );
		mStick.AddTriangle( n1, n2, n3, false );
		mStick.AddTriangle( n3, n4, n1, false );

		n1.Position.z = n4.Position.z = Back;
		n2.Position.z = n3.Position.z = 0.0f;
		n1.Position.x = n2.Position.x = n3.Position.x = n4.Position.x = PlusX;
		n1.Position.y = n2.Position.y = 0.0f;
		n3.Position.y = n4.Position.y = Down;
		mStick.AddTriangle( n1, n2, n3, false );
		mStick.AddTriangle( n3, n4, n1, false );

		n1.Position.z = n4.Position.z = Back;
		n2.Position.z = n3.Position.z = 0.0f;
		n1.Position.x = n2.Position.x = n3.Position.x = n4.Position.x = MinusX;
		n1.Position.y = n2.Position.y = 0.0f;
		n3.Position.y = n4.Position.y = Down;
		mStick.AddTriangle( n1, n3, n2, false );
		mStick.AddTriangle( n3, n1, n4, false );
	}
	mStick.CommitMesh( false );
	mStick.NormalizeMesh();
	const float fRarius = .0031f;
	LPD3DXMESH pMesh( NULL );
	THROW_EXC_D3D( D3DXCreateCylinder(
		DXUTGetD3D9Device(),
		fRarius,
		fRarius,
		.395f,
		8,
		1,
		&pMesh,
		NULL ), L"Cylinder" );
	mThread.ParseMesh( pMesh );
	mThread.CommitMesh( false, false );

	const float fScale = .2f;
	const XMMATRIX Scale( XMMatrixScaling( fScale, fScale, fScale ) );
	const XMMATRIX MoveCircle( XMMatrixTranslation( 0.0f, -1.1f, 0.0f ) );
	XMStoreFloat4x4( &mTransform, XMMatrixMultiply( Scale, MoveCircle ) );
	mConverter.SimplifyMesh( mConverter.mVertices.size() / 2 );
	mPendulum.ParseMesh( mConverter.GetMesh() );

	XMStoreFloat4x4( &mTransform, XMMatrixIdentity() );
	mPendulum.ParseMesh( mStick.GetMesh() );	

	const XMMATRIX Rot( XMMatrixRotationX( XM_PIDIV2 ) );
	const XMMATRIX MoveMe( XMMatrixTranslation( 0.0f, -1.36f, 0.0f ) );
	XMStoreFloat4x4( &mTransform, XMMatrixMultiply( Rot, MoveMe ) );
	mPendulum.ParseMesh( mThread.GetMesh() );

	const XMMATRIX Rot2( XMMatrixRotationX( XM_PIDIV2 ) );
	const XMMATRIX MoveMe2( XMMatrixTranslation( 0.0f, -1.4f, 0.0f ) );
	const float fRadius2 = 6.815f;
	const XMMATRIX Scale2( XMMatrixScaling( fRadius2, .019f, fRadius2 ) );
	XMStoreFloat4x4( &mTransform, XMMatrixMultiply( XMMatrixMultiply( Rot2, Scale2), MoveMe2 ) );
	mPendulum.ParseMesh( pMesh );

	const XMMATRIX Rot1( XMMatrixRotationX( XM_PIDIV2 ) );
	const XMMATRIX MoveMe1( XMMatrixTranslation( 0.0f, -1.4f, 0.0f ) );
	const float fRadius1 = 5.125f;
	const XMMATRIX Scale1( XMMatrixScaling( fRadius1, .045f, fRadius1 ) );
	XMStoreFloat4x4( &mTransform, XMMatrixMultiply( XMMatrixMultiply( Rot1, Scale1), MoveMe1 ) );
	mPendulum.ParseMesh( pMesh );

	const XMMATRIX Rot3( XMMatrixRotationX( XM_PIDIV2 ) );
	const XMMATRIX MoveMe3( XMMatrixTranslation( 0.0f, -1.415f, 0.0f ) );
	const float fRadius3 = 3.25f;
	const XMMATRIX Scale3( XMMatrixScaling( fRadius3, .045f, fRadius3 ) );
	XMStoreFloat4x4( &mTransform, XMMatrixMultiply( XMMatrixMultiply( Rot3, Scale3), MoveMe3 ) );
	mPendulum.ParseMesh( pMesh );
	
	mPendulum.CommitMesh( true, true );
	mPendulum.NormalizeMesh();
	mPendulum.SaveMesh( L"pendulum.x" );

}
