#include "DXUT.h"
#include "StringCore.h"

COneMeshCreator::COneMeshCreator()
{
	LPD3DXMESH pMesh( NULL );
	THROW_EXC_D3D( D3DXCreateTorus(
		DXUTGetD3D9Device(),
		.233f,
		1.0f,
		6,
		10,
		&pMesh,
		NULL ), L"Torus" );

	One.ParseMesh( pMesh );
	SAFE_RELEASE( pMesh );
	One.CommitMesh();
	One.NormalizeMesh();
	One.SaveMesh( L"string.$$$" );
	
	/*for( int i = 0; i < 42; i ++ )	
	{
		mShort.ParseMesh( L"string.$$$" );
		mShort.OnOne();
	}
	mShort.CommitMesh();
	mShort.NormalizeMesh();
	mShort.SaveMesh( L"string.x" );*/

	for( int i = 0; i < 200; i ++ )	
	{
		mLong.ParseMesh( L"string.$$$" );
		mLong.OnOne();
	}
	mLong.CommitMesh( true, true );
	mLong.NormalizeMesh();
	mLong.SaveMesh( L"string_int.x" );

	/*THROW_EXC_D3D( D3DXCreateTorus(
		DXUTGetD3D9Device(),
		.175f,
		1.0f,
		12,
		20,
		&pMesh,
		NULL ), L"Torus2" );

	Torus.ParseMesh( pMesh );
	SAFE_RELEASE( pMesh );
	Torus.CommitMesh();
	Torus.NormalizeMesh();
	Torus.SaveMesh( L"torus.x" );*/	
}

void Rotate( XMFLOAT3 & NewVal )
{
	XMFLOAT3 tmp;
	tmp.x = NewVal.x;
	tmp.y = NewVal.y;
	tmp.z = NewVal.z;

	XMVECTOR res = XMVector3Transform(
		XMLoadFloat3( &tmp ),
		XMMatrixRotationZ( XM_PIDIV2 ) );
	XMStoreFloat3( &tmp, res );

	NewVal.x = tmp.x;
	NewVal.y = tmp.y;
	NewVal.z = tmp.z;
}
void RotateY( XMFLOAT3 & NewVal )
{
	XMFLOAT3 tmp;
	tmp.x = NewVal.x;
	tmp.y = NewVal.y;
	tmp.z = NewVal.z;

	XMVECTOR res = XMVector3Transform(
		XMLoadFloat3( &tmp ),
		XMMatrixRotationY( XM_PIDIV2 ) );
	XMStoreFloat3( &tmp, res );

	NewVal.x = tmp.x;
	NewVal.y = tmp.y;
	NewVal.z = tmp.z;
}
void Vertical( VERTEXPLAIN & a )
{
	Rotate( a.Position );
	const float fVerticalMovement = .4f;
	if( a.Position.y < 0.0f )
		a.Position.y -= fVerticalMovement;
	else
		a.Position.y += fVerticalMovement;
}
void COneMeshCreator::COne::OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
{
	VERTEXPLAIN aNew( a ), bNew( b ), cNew( c );
	Vertical( aNew );
	Vertical( bNew );
	Vertical( cNew );
	AddTriangle( aNew, bNew, cNew );
}
void ScaleTorus( VERTEXPLAIN & a )
{
	const float fScale = .06f;
	a.Position.x *= fScale;
	a.Position.y *= fScale;
	a.Position.z *= fScale;
	RotateY( a.Position );
}
void COneMeshCreator::CTorus::OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
{
	VERTEXPLAIN aNew( a ), bNew( b ), cNew( c );
	ScaleTorus( aNew );
	ScaleTorus( bNew );
	ScaleTorus( cNew );
	AddTriangle( aNew, bNew, cNew );
}
COneMeshCreator::CParametrizedString::CParametrizedString()
: m_bRotated( false ), m_fY( 0.0f )
{
}
void COneMeshCreator::CParametrizedString::OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
{
	VERTEXPLAIN aNew( a ), bNew( b ), cNew( c );
	AlterPos( aNew );
	AlterPos( bNew );
	AlterPos( cNew );
	AddTriangle( aNew, bNew, cNew );
}
void COneMeshCreator::CParametrizedString::AlterPos( VERTEXPLAIN & a )
{
	const float fScale = .0135f / .88f;
	a.Position.y -= m_fY;
	if( !m_bRotated )
		RotateY( a.Position );
	a.Position.x *= fScale;
	a.Position.y *= fScale;
	a.Position.z *= fScale;
}
void COneMeshCreator::CParametrizedString::OnOne()
{
	m_bRotated = !m_bRotated;
	m_fY += 2.27f;
}