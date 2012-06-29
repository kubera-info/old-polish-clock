#include "DXUT.h"
#include "BackHookCore.h"
#include "BackHookCore-$$$.h"

static const float fScale( 0.08f );
static const float fDeph = .007f;
const float xMove = .38f;
const float fRadius = .03f;

static void TransformHook( XMFLOAT3 & NewVal )
{
	XMFLOAT3 tmp;
	tmp.x = NewVal.x;
	tmp.y = -NewVal.y;
	tmp.z = NewVal.z;

	XMVECTOR res = XMVector3Transform(
		XMLoadFloat3( &tmp ),
		XMMatrixMultiply( XMMatrixRotationY( XM_PIDIV2 ), XMMatrixScaling( fScale, fScale, fScale ) ) );
	XMStoreFloat3( &tmp, res );

	NewVal.x = tmp.x;
	NewVal.y = tmp.y;
	NewVal.z = tmp.z;
}

static void TransformDephHook( XMFLOAT3 & NewVal )
{
	NewVal.x -= fDeph;
}

static void TransformFinalHook( XMFLOAT3 & NewVal, const float Y )
{
	NewVal.x += xMove + fDeph + fRadius + .017f;
	NewVal.y += Y;
	NewVal.z += -.659f;
}

CDumpHooh::CDumpHooh()
{
	struct COne : public CMeshBuilderPlain
	{
		COne()
		{
			XMMATRIX Ident = XMMatrixIdentity();
			XMStoreFloat4x4( &mTranslation, XMMatrixTranslation( 0.93f, 0.125f, 0.0f ) );
			XMStoreFloat4x4( &mRotation, XMMatrixRotationX( XM_PIDIV2 ) );
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
		XMFLOAT4X4 mTranslation, mRotation;
	} One;
	LPD3DXMESH pMesh( NULL );
	const float scale = .191f;
	THROW_EXC_D3D( D3DXCreateTorus(
		DXUTGetD3D9Device(),
		.256f * scale,
		.96f * scale,
		8,
		12,
		&pMesh,
		NULL ), L"Torus" );

	One.ParseMesh( pMesh );
	SAFE_RELEASE( pMesh );
	One.CommitMesh();
	One.NormalizeMesh();

	CMeshBuilderPlain tmp;
	MakeBackHook( tmp );
	tmp.CalculateCanonicalMesh();
	tmp.CommitMesh( false );
	hook.ParseMesh( tmp.GetMesh() );
	hook.ParseMesh( One.GetMesh() );
	hook.CommitMesh( false );
	hook2.CloneFromMesh( hook.GetMesh() );
	hook2.ParseMesh( hook.GetMesh() );
	hook2.CommitMesh( false );

	hook3.Y = -.52f;
	hook3.ParseMesh( hook2.GetMesh() );
	hook3.Y = .085f;
	hook3.ParseMesh( hook2.GetMesh() );
	hook3.CommitMesh( true, false );
	hook3.CleanMesh();

	hook4.X = 1.0f;
	hook4.ParseMesh( hook3.GetMesh() );
	hook4.X = -1.0f;
	hook4.ParseMesh( hook3.GetMesh() );
	hook4.CommitMesh( true, false );
	hook4.SaveMesh( L"back_hook.x" );
}

void CDumpHooh::CHookAddTransform::OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
{
	VERTEXPLAIN aNew( a ), bNew( b ), cNew( c );

	TransformHook( aNew.Position );
	TransformHook( bNew.Position );
	TransformHook( cNew.Position );
	TransformHook( aNew.Normal );
	TransformHook( bNew.Normal );
	TransformHook( cNew.Normal );

	AddTriangle( aNew, bNew, cNew );
}

void CDumpHooh::CHookAddDeph::OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
{
	VERTEXPLAIN aNew( a ), bNew( b ), cNew( c );

	TransformDephHook( aNew.Position );
	TransformDephHook( bNew.Position );
	TransformDephHook( cNew.Position );

	AddTriangle( a, aNew, bNew, true );
	AddTriangle( bNew, b, a, true );
	AddTriangle( a, bNew, aNew, true );
	AddTriangle( bNew, a, b, true );

	AddTriangle( a, aNew, cNew, true );
	AddTriangle( cNew, c, a, true );
	AddTriangle( a, cNew, aNew, true );
	AddTriangle( cNew, a, c, true );

	AddTriangle( b, bNew, cNew, true );
	AddTriangle( cNew, c, b, true );
	AddTriangle( b, cNew, bNew, true );
	AddTriangle( cNew, b, c, true );
}
void CDumpHooh::CHookAddFinalize::OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
{
	VERTEXPLAIN aNew( a ), bNew( b ), cNew( c );

	TransformFinalHook( aNew.Position, Y );
	TransformFinalHook( bNew.Position, Y );
	TransformFinalHook( cNew.Position, Y );

	AddTriangle( aNew, bNew, cNew, true );
}
void CDumpHooh::CHookAddFinalize2::OnParseTriangle( const VERTEXPLAIN & a, const VERTEXPLAIN & b, const VERTEXPLAIN & c )
{
	VERTEXPLAIN aNew( a ), bNew( b ), cNew( c );

	aNew.Position.x *= X;
	bNew.Position.x *= X;
	cNew.Position.x *= X;

	if( X < 0.0f && a.Normal.x > .5f )
		AddTriangle( aNew, cNew, bNew, true );
	else
		AddTriangle( aNew, bNew, cNew, true );
}