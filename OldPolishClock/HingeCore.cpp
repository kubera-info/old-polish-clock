#include "DXUT.h"
#include "HingeCore.h"
#include <vector>

static const float fScale( 0.073f );
static const float fDeph = .007f;
const float xMove = .38f;
const float fRadius = .03f;

CHingeMeshCreator::CHingeMeshCreator()
{
	LPD3DXMESH pMesh( NULL );
	THROW_EXC_D3D( D3DXCreateCylinder(
		DXUTGetD3D9Device(),
		1.0f,
		1.0f,
		1.0f,
		8,
		1,
		&pMesh,
		NULL ), L"Cylinder" );

	VERTEXPLAIN a, b, c, d;

	a.Position.x = .5;
	b.Position.x = .5;

	c.Position.x = 5;
	d.Position.x = 5;

	a.Position.y = -.5;
	b.Position.y = .5;
	c.Position.y = .5;
	d.Position.y = -.5;

	a.Position.z = 0;
	b.Position.z = 0;
	c.Position.z = 0;
	d.Position.z = 0;

	typedef std::vector<float> CFloats;

	const float shortH = .02f;
	const float longH = .03f;
	CFloats Heigths;
	Heigths.push_back( shortH );
	Heigths.push_back( longH );
	Heigths.push_back( longH );
	Heigths.push_back( shortH );

	for( int i = 0; i < 2; ++ i )
	{
		if( 1 == i )
			Hinge.y -= 1.3f;

		for( CFloats::const_iterator it = Heigths.begin(); it != Heigths.end(); ++ it )
		{
			Hinge.height = *it;

			Hinge.bRotate = false;
			Hinge.AddAndParseTriangle( a, c, b, true );
			Hinge.AddAndParseTriangle( c, a, d, true );
			Hinge.bRotate = true;
			Hinge.ParseMesh( pMesh );

			Hinge.y += .002f + *it;
		}
	}

	pMesh->Release();
	Hinge.CommitMesh();
	Hinge.SaveMesh( L"hinge.x" );
}

CHingeMeshCreator::CHinge::CHinge() :
radius( .005f ),
y( .51f ),
x( -.7645f ), 
z( 0.156f ),
bRotate( false )
{
}
void Rotate( XMFLOAT3 & NewVal )
{
	XMFLOAT3 tmp;
	tmp.x = NewVal.x;
	tmp.y = NewVal.y;
	tmp.z = NewVal.z;

	XMVECTOR res = XMVector3Transform(
		XMLoadFloat3( &tmp ),
		XMMatrixRotationX( M_PI / 2.0f ) );
	XMStoreFloat3( &tmp, res );

	NewVal.x = tmp.x;
	NewVal.y = tmp.y;
	NewVal.z = tmp.z;
}
void CHingeMeshCreator::CHinge::OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
{
	VERTEXPLAIN aNew( a ), bNew( b ), cNew( c );
	if( bRotate )
	{
		Rotate( aNew.Position );
		Rotate( aNew.Normal );
		Rotate( bNew.Position );
		Rotate( bNew.Normal );
		Rotate( cNew.Position );
		Rotate( cNew.Normal );
	}

	aNew.Position.x *= radius;
	bNew.Position.x *= radius;
	cNew.Position.x *= radius;

	aNew.Position.z *= radius;
	bNew.Position.z *= radius;
	cNew.Position.z *= radius;

	aNew.Position.y += .5f;
	bNew.Position.y += .5f;
	cNew.Position.y += .5f;

	aNew.Position.y *= height;
	bNew.Position.y *= height;
	cNew.Position.y *= height;

	aNew.Position.y += y;
	bNew.Position.y += y;
	cNew.Position.y += y;

	aNew.Position.z += z;
	bNew.Position.z += z;
	cNew.Position.z += z;

	aNew.Position.x += x;
	bNew.Position.x += x;
	cNew.Position.x += x;

	AddTriangle( aNew, bNew, cNew );
}

COvalCreator::COvalCreator()
{
	LPD3DXMESH pMesh( NULL );
	THROW_EXC_D3D( D3DXCreateSphere(
		DXUTGetD3D9Device(),
		1.0f,
		5,
		4,
		&pMesh,
		NULL ), L"Sphe" );

	struct CNail : public CMeshBuilderPlain
	{
		CNail()
		{
			const float scale = .011f;
			XMMATRIX Ident = XMMatrixIdentity();
			XMStoreFloat4x4( &mTranslation, Ident );
			XMStoreFloat4x4( &mRotation, 
				XMMatrixMultiply(
				XMMatrixScaling( scale * 1.0f, scale * 1.0f, scale * .41f ),
				XMMatrixRotationY( XM_PIDIV2 ) ) );
		}
		void OnVert( VERTEXPLAIN & a )
		{
			XMStoreFloat3( &a.Position, XMVector3Transform( XMLoadFloat3( &a.Position ), XMLoadFloat4x4( &mRotation ) ) );
			XMStoreFloat3( &a.Position, XMVector3Transform( XMLoadFloat3( &a.Position ), XMLoadFloat4x4( &mTranslation ) ) );
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
	} mOval;

	const float xNewMove( .0027f );
	const float zFront( .13f - .045f );
	const float yFront( +.0017f );
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( .767f + xNewMove, -.59f + yFront, zFront ) );
	mOval.ParseMesh( pMesh );
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( .767f + xNewMove, .59f + yFront, zFront ) );
	mOval.ParseMesh( pMesh );

	const float zMiddle( -.259f + -.053f );
	const float xPosMiddle( xMove + fDeph + fRadius + .017f + xNewMove );
	const float yMiddle( -0.0f );
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( xPosMiddle, yMiddle, zMiddle ) );
	mOval.ParseMesh( pMesh );
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( -xPosMiddle, yMiddle, zMiddle ) );
	mOval.ParseMesh( pMesh );

	float zBack( -.659f -.015f );
	float xPosBack( xMove + fDeph + fRadius + .017f + xNewMove );
	float yAlter = .012f;

	float yBack = -.52f + yAlter;
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( xPosBack, yBack, zBack ) );
	mOval.ParseMesh( pMesh );
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( -xPosBack, yBack, zBack ) );
	mOval.ParseMesh( pMesh );
	yBack = .085f + yAlter;
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( xPosBack, yBack, zBack ) );
	mOval.ParseMesh( pMesh );
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( -xPosBack, yBack, zBack ) );
	mOval.ParseMesh( pMesh );

	yAlter = .005f;
	zBack = -.5995f;
	yBack = -.52f + yAlter;
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( xPosBack, yBack, zBack ) );
	mOval.ParseMesh( pMesh );
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( -xPosBack, yBack, zBack ) );
	mOval.ParseMesh( pMesh );
	yBack = .085f + yAlter;
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( xPosBack, yBack, zBack ) );
	mOval.ParseMesh( pMesh );
	XMStoreFloat4x4( &mOval.mTranslation, XMMatrixTranslation( -xPosBack, yBack, zBack ) );
	mOval.ParseMesh( pMesh );

	mOval.CommitMesh( false, false );
	mOval.NormalizeMesh();
	mOval.SaveMesh( L"oval.x" );
}