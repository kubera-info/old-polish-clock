#include "DXUT.h"
#include "WeightCore.h"

CWeightMeshCreator::CWeightMeshCreator()
{
	/*mCanonical.ParseMesh( L"weight.$$$" );
	mCanonical.CalculateCanonicalMesh();
	mCanonical.CommitMesh( false, false );
	mCanonical.CleanMesh();
	mCanonical.SaveMesh( L"weight2.$$$" );

	mMetal.ParseMesh( L"weight2.$$$" );
	mMetal.CommitMesh( false, false );
	mMetal.WeldMesh( .001f, .001 );
	mMetal.SaveMesh( L"weight3.$$$" );

	mConvert.ParseMesh( L"weight3.$$$" );
	mConvert.mNew.CommitMesh();
	mConvert.mNew.SaveMesh( L"weight.x" );*/
}
void CWeightMeshCreator::CNormalsAndSimple::OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
{
	const XMVECTOR aVec = XMLoadFloat3( &a.Normal );
	const XMVECTOR bVec = XMLoadFloat3( &b.Normal );
	const XMVECTOR cVec = XMLoadFloat3( &c.Normal );
	const XMVECTOR nVec = ( aVec + bVec + cVec ) / 3.0f;

	const XMFLOAT3 TriangleNormal = CalculateTriangleNormal( a, b, c );
	const XMVECTOR tVec = XMLoadFloat3( &TriangleNormal );
	const XMVECTOR dot = XMVector3Dot( tVec, nVec );
	XMFLOAT3 dot2;
    XMStoreFloat3( &dot2, dot );
	if( dot2.x > .5 )
		AddTriangle( a, c, b );
	else
		AddTriangle( a, b, c );
}
void CWeightMeshCreator::CConvert::OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
{
	VERTEXPLAIN newA( a ), newB( b ), newC( c );
	newA.Normal.x = -newA.Normal.x;
	newB.Normal.x = -newB.Normal.x;
	newC.Normal.x = -newC.Normal.x;
	newA.Normal.y = -newA.Normal.y;
	newB.Normal.y = -newB.Normal.y;
	newC.Normal.y = -newC.Normal.y;
	newA.Normal.z = -newA.Normal.z;
	newB.Normal.z = -newB.Normal.z;
	newC.Normal.z = -newC.Normal.z;
	mNew.AddTriangle( newA, newB, newC );
}