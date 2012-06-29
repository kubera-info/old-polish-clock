#include "DXUT.h"
#include "OrnamentCore.h"

COrnamentMeshCreator::COrnamentMeshCreator()
{

	{
		LPD3DXMESH SourceMesh = NULL;
		HRESULT hr;
		LPD3DXBUFFER Dummy1 = NULL, Dummy2 = NULL, Dummy3 = NULL;
		DWORD NumMaterials;

		V ( D3DXLoadMeshFromX(
			L"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\C++\\Direct3D11\\Edu\\Meshes\\ornmnt.x",
			D3DXMESH_32BIT | D3DXMESH_IB_MANAGED | D3DXMESH_VB_MANAGED /*| m_flags*/,
			DXUTGetD3D9Device(),
			&Dummy1,
			&Dummy2,
			&Dummy3,
			&NumMaterials,
			&SourceMesh) );
		SAFE_RELEASE(Dummy1);
		SAFE_RELEASE(Dummy2);
		SAFE_RELEASE(Dummy3);

		mOrnament.CloneFromMesh( SourceMesh );
		SAFE_RELEASE( SourceMesh );
		mOrnament.CommitMesh();
		//mOrnament.WeldMesh( .00001f, .00001f );
		mOrnament.SaveMesh( L"ornmnt.x" );
	}

	{
		LPD3DXMESH SourceMesh = NULL;
		HRESULT hr;
		LPD3DXBUFFER Dummy1 = NULL, Dummy2 = NULL, Dummy3 = NULL;
		DWORD NumMaterials;

		V ( D3DXLoadMeshFromX(
			L"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\C++\\Direct3D11\\Edu\\Meshes\\clockface.x",
			D3DXMESH_32BIT | D3DXMESH_IB_MANAGED | D3DXMESH_VB_MANAGED /*| m_flags*/,
			DXUTGetD3D9Device(),
			&Dummy1,
			&Dummy2,
			&Dummy3,
			&NumMaterials,
			&SourceMesh) );
		SAFE_RELEASE(Dummy1);
		SAFE_RELEASE(Dummy2);
		SAFE_RELEASE(Dummy3);

		mClockF.CloneFromMesh( SourceMesh );
		SAFE_RELEASE( SourceMesh );
		mClockF.CommitMesh();
		mClockF.SaveMesh( L"clockface.x" );
	}
}

CMissMeshCreator::CMissMeshCreator()
{
	mMiss.ParseMesh( L"miss.$$$" );
	mMiss.CommitMesh();
	mMiss.SaveMesh( L"miss.x" );
}
void CMissMeshCreator::CMeshMiss::OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
{
#define tmpTransl XMMatrixTranslation( 0.0f, .495f, -.084f )
#define tmpScale XMMatrixScaling( .185f, .35f, 1.0f )
	const static XMMATRIX tmpPicture = XMMatrixMultiply( tmpScale, tmpTransl );

	VERTEXTEX newA( a ), newB( b ), newC( c );

	XMVECTOR xma = XMVector3Transform( XMLoadFloat3( &a.Position ), tmpPicture );
	XMVECTOR xmb = XMVector3Transform( XMLoadFloat3( &b.Position ), tmpPicture );
	XMVECTOR xmc = XMVector3Transform( XMLoadFloat3( &c.Position ), tmpPicture );

	XMStoreFloat3( &newA.Position, xma );
	XMStoreFloat3( &newB.Position, xmb );
	XMStoreFloat3( &newC.Position, xmc );

	AddTriangle( newA, newB, newC );
}
