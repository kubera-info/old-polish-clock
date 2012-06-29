#pragma once
#ifndef _MESH_CORE_H_
#define _MESH_CORE_H_

//
// This file is a tool for creating meshes procedurally.
// It is not nececcery in the final version of application.
//

#include <boost/foreach.hpp>
#include "Patterns.h"
#include "MonadException.h"
#include <atlfile.h>
#include "Path.h"
#include "MonadSystem.h"
#ifdef DEBUG
#include "tootlelib.h"
#endif
#include "MonadTemplates.h"

extern bool bAddMe;

using namespace std;

template
<
typename VERTEXTYPE
>
class CVertices : CDestructable
{
public:
	CVertices() : m_pMesh( NULL )
	{
		DXUTTRACE( L"Mesh Builder is ready\n" );
	}
	// Triangles
	DWORD FacesCount() const
	{
		return ( DWORD ) mIndices.size() / 3;
	}
	void AddTriangle2( const VERTEXTYPE & a, const VERTEXTYPE & b, const VERTEXTYPE & c ) throw(...)
	{
		if( !bAddMe )
			return;
		VERTEXTYPE aCopy = a, bCopy = b, cCopy = c;

		AddVertex( aCopy );
		AddVertex( bCopy );
		AddVertex( cCopy );
	}
	void AddQuad2( const VERTEXTYPE & a, const VERTEXTYPE & b, const VERTEXTYPE & c, const VERTEXTYPE & d ) throw(...)
	{
		AddTriangle2( a, b, c );
		AddTriangle2( c, d, a );
	}
	void GetTriangle( int pTriangle, const VERTEXTYPE & a, const VERTEXTYPE & b, const VERTEXTYPE & c ) const
	{
		register int Idx = pTriangle * 3;
		a = mVertices[ mIndices[ Idx ++] ];
		b = mVertices[ mIndices[ Idx ++] ];
		c = mVertices[ mIndices[ Idx ] ];
	}
	void RemoveTriangle( int pTriangle )
	{
		register int Idx = pTriangle * 3 + 2;
		mIndices.RemoveAt(Idx --);
		mIndices.RemoveAt(Idx --);
		mIndices.RemoveAt(Idx);
	}
	virtual void OnParseTriangle( const VERTEXTYPE & a, const VERTEXTYPE & b, const VERTEXTYPE & c ) throw(...)
	{
		AddTriangle2( a, b, c );
	}
	// a helper
	void ValidateAndLockSourceMesh( 
		LPD3DXMESH pSourceMesh, 
		VERTEXTYPE ** pVertices, 
		DWORD ** pIndices,
		DWORD & dwNumFaces,
		DWORD & dwNumVertices,
		DWORD & dwMeshEnum
		) const throw(...)
	{
		if( NULL != m_pMesh )
			THROW_EXC( MonadException::ClassAlreadyExists, E_FAIL, L"Validate PreMesh");
		//if( mVertices.size() + mIndices.size() )
		//THROW_EXC( MonadException::ClassAlreadyExists, E_FAIL, L"Validate PreMesh 2");
		THROW_EXC_IFZ( pSourceMesh, MonadException::NullValue, L"Validate Source Mesh");
		//if( pSourceMesh->GetFVF() != VERTEXTYPE::FVF )
		//THROW_EXC( MonadException::InvalidClass, E_FAIL, L"Validate Source Mesh");

		// Lock the geometry buffers
		dwNumFaces    = pSourceMesh->GetNumFaces();
		dwNumVertices = pSourceMesh->GetNumVertices();
		dwMeshEnum    = pSourceMesh->GetOptions();

		// Allocate a temporary edge list
		THROW_EXC_D3D( pSourceMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*) pVertices ), L"LockSource" );
		THROW_EXC_D3D( pSourceMesh->LockIndexBuffer( D3DLOCK_READONLY, (LPVOID*) pIndices ), L"LockSource2" );
	}
	void ParseMesh( LPD3DXMESH pSourceMesh ) throw(...)
	{
		VERTEXTYPE * pVertices;
		DWORD * pIndices;
		WORD * pIndices2;

		// Lock the geometry buffers
		DWORD dwNumFaces;
		DWORD dwNumVertices;
		DWORD dwMeshEnum;

		ValidateAndLockSourceMesh( pSourceMesh, &pVertices, &pIndices, dwNumFaces, dwNumVertices, dwMeshEnum );
		if( dwMeshEnum & D3DXMESH_32BIT )
			for( ULONG_PTR mInd = 0; mInd < dwNumFaces * 3; )
			{
				VERTEXTYPE a, b, c;

				a = pVertices[ pIndices[ mInd ++ ] ];
				b = pVertices[ pIndices[ mInd ++ ] ];
				c = pVertices[ pIndices[ mInd ++ ] ];

				OnParseTriangle( a, b, c );
			}
		else
		{
			pIndices2 = reinterpret_cast< WORD * > ( pIndices );
			for( ULONG_PTR mInd = 0; mInd < dwNumFaces * 3; )
			{
				VERTEXTYPE a, b, c;

				a = pVertices[ pIndices2[ mInd ++ ] ];
				b = pVertices[ pIndices2[ mInd ++ ] ];
				c = pVertices[ pIndices2[ mInd ++ ] ];

				OnParseTriangle( a, b, c );
			}
		}

		pSourceMesh->UnlockVertexBuffer();
		pSourceMesh->UnlockIndexBuffer();
	}
	void ParseMesh( const std::wstring & pSourceMesh ) throw(...)
	{
		LPD3DXMESH pMeshToParse( NULL );
		THROW_EXC_D3D( D3DXLoadMeshFromX(
			GetPath( pSourceMesh ).c_str(),
			D3DXMESH_32BIT | D3DXMESH_IB_MANAGED | D3DXMESH_VB_MANAGED,
			DXUTGetD3D9Device(),
			NULL,
			NULL,
			NULL,
			NULL,
			&pMeshToParse
			), L"Parse I");
		ParseMesh( pMeshToParse );
	}	
	void CloneFromMesh( LPD3DXMESH pSourceMesh ) throw(...)
	{
		VERTEXTYPE * pVertices;
		DWORD * pIndices;

		// Lock the geometry buffers
		DWORD dwNumFaces;
		DWORD dwNumVertices;
		DWORD dwMeshEnum;

		ValidateAndLockSourceMesh( pSourceMesh, &pVertices, &pIndices, dwNumFaces, dwNumVertices, dwMeshEnum );

		mIndices.reserve( dwNumFaces * 3 );
		mVertices.reserve( dwNumVertices );
		if( dwMeshEnum & D3DXMESH_32BIT )
			for( DWORD mInd = 0; mInd < dwNumFaces * 3; mInd ++ )
				mIndices.push_back( pIndices[ mInd ] );
		else
		{
			WORD * pIndices2 = reinterpret_cast< WORD * > ( pIndices );
			for( DWORD mInd = 0; mInd < dwNumFaces * 3; mInd ++ )
				mIndices.push_back( pIndices2[ mInd ] );
		}
		for( DWORD mVert = 0; mVert < dwNumVertices; mVert ++ )
			mVertices.push_back( pVertices[ mVert ] );

		pSourceMesh->UnlockVertexBuffer();
		pSourceMesh->UnlockIndexBuffer();
	}

	void CommitMesh( bool bOpt = true, bool bFoolOpt = true ) throw(...)
	{
		if( NULL != m_pMesh )
			THROW_EXC( MonadException::ClassAlreadyExists, E_FAIL, L"CommitMesh");

		VOID * m_Faces = NULL;
		VOID * m_Vertices = NULL;

		THROW_EXC_D3D( D3DXCreateMesh(
			FacesCount(),
			( DWORD ) mVertices.size(),
			D3DXMESH_32BIT | D3DXMESH_IB_MANAGED | D3DXMESH_VB_MANAGED,
			VERTEXTYPE::Decl,
			DXUTGetD3D9Device(),
			&m_pMesh), L"Mesh #1");
		THROW_EXC_D3D( m_pMesh->LockVertexBuffer(NULL, &m_Vertices), L"Mesh #2");
		THROW_EXC_D3D( m_pMesh->LockIndexBuffer(NULL, &m_Faces), L"Mesh #3");
		CopyMemory(m_Vertices, &mVertices[0], sizeof(VERTEXTYPE) * mVertices.size());
		CopyMemory(m_Faces, &mIndices[0], sizeof(DWORD) * mIndices.size() );
		THROW_EXC_D3D( m_pMesh->UnlockIndexBuffer(), L"Mesh #4");
		THROW_EXC_D3D( m_pMesh->UnlockVertexBuffer(), L"Mesh #5");
		if( bOpt )
			OptimizeMesh( bFoolOpt );
	}
	void TestIfInitializedMesh() const throw(...)
	{
		if( NULL == m_pMesh )
			THROW_EXC( MonadException::NullValue, CO_E_NOTINITIALIZED, L"TestMesh");
	}
	void NormalizeMesh() throw(...)
	{
		TestIfInitializedMesh();

		THROW_EXC_D3D( D3DXComputeNormals( m_pMesh, NULL ), L"Nrm");
	}
	void WeldMesh(const float EpsilonOfPosition, const float EpsilonOfNormal ) throw(...)
	{
		TestIfInitializedMesh();

		CComPtr<ID3DXBuffer>     AdjacencyBuffer; // adjacency data buffer
		//LPD3DXBUFFER     pD3DXMtrlBuffer  = NULL; // material buffer
		//LPD3DXMESH       pMesh            = NULL; // mesh object
		//DWORD            m_dwNumMaterials;        // number of materials
		D3DXWELDEPSILONS Epsilons;                // structure with epsilon values
		//DWORD            *pFaceRemap[65536];      // face remapping array
		//DWORD            i;                       // internal variable

		// Load the mesh from the specified file
		DWORD nF3 = m_pMesh->GetNumFaces() * 3;
		D3DXCreateBuffer( nF3 * (DWORD) sizeof(DWORD), &AdjacencyBuffer );

		// Set epsilon values
		Epsilons.Position = EpsilonOfPosition;
		Epsilons.Normal = EpsilonOfNormal;
		Epsilons.BlendWeights = 0;
		Epsilons.PSize = 0;
		Epsilons.Tangent = 0;
		Epsilons.TessFactor = 0;

		// Weld the vertices
		/*for( i=0; i < 65536; i++ )
		{ 
		pFaceRemap[i] = 0; 
		}*/

		THROW_EXC_D3D( 
			D3DXWeldVertices ( m_pMesh,
			D3DXWELDEPSILONS_WELDALL,
			&Epsilons,
			NULL,
			(DWORD*)AdjacencyBuffer->GetBufferPointer(),
			//(DWORD*)pFaceRemap,
			NULL,
			NULL ),
			L"Weld");

		THROW_EXC_D3D( 
			m_pMesh->OptimizeInplace(
			D3DXMESHOPT_DEVICEINDEPENDENT,
			(DWORD*) AdjacencyBuffer->GetBufferPointer(), 0, 0, 0), L"Optimize" );

	}
	void OptimizeMesh( bool bFoolOpt = true )
	{
#ifdef DEBUG
		VERTEXTYPE * pVertices;
		DWORD * pIndices;

		// Lock the geometry buffers
		DWORD dwNumFaces( 0 );
		DWORD dwNumVertices( 0 );

		// Lock the geometry buffers
		dwNumFaces    = m_pMesh->GetNumFaces();
		dwNumVertices = m_pMesh->GetNumVertices();

		// Allocate a temporary edge list
		THROW_EXC_D3D( m_pMesh->LockVertexBuffer( 0L, (LPVOID*) &pVertices ), L"LockSource Opt" );
		THROW_EXC_D3D( m_pMesh->LockIndexBuffer( 0L, (LPVOID*) &pIndices ), L"LockSource2 Opt" );

		DWORD * pnIBOut = new DWORD[ dwNumFaces * 3 + 1 ];

		if( TOOTLE_OK != TootleFastOptimize(
			pVertices,
			( const unsigned int * ) pIndices,
			dwNumVertices,
			dwNumFaces,
			sizeof( VERTEXTYPE ),
			TOOTLE_DEFAULT_VCACHE_SIZE,
			TOOTLE_CW,
			( unsigned int * ) pnIBOut,
			NULL,
			TOOTLE_DEFAULT_ALPHA ) )
			THROW_EXC( MonadException::InvalidClass, E_FAIL, L"TootleFastOptimize" );

		if( bFoolOpt )
		{
			unsigned int* faceClusters = new unsigned int[ dwNumFaces + 1 ];
			if( TOOTLE_OK != TootleClusterMesh(
				pVertices, 
				( const unsigned int * ) pnIBOut, 
				dwNumVertices, 
				dwNumFaces, 
				sizeof( VERTEXTYPE ),
				0, // let the algorithm determines the target number of clusters
				( unsigned int * ) pnIBOut, // overwrite the input index buffer
				faceClusters, NULL ) )
				THROW_EXC( MonadException::InvalidClass, E_FAIL, L"TootleClusterMesh" );
			if( TOOTLE_OK != TootleVCacheClusters(
				( const unsigned int * ) pnIBOut, 
				dwNumFaces, 
				dwNumVertices, 
				TOOTLE_DEFAULT_VCACHE_SIZE, 
				faceClusters,
				( unsigned int * ) pnIBOut, // overwrite the input index buffer
				NULL, 
				TOOTLE_VCACHE_AUTO ) )
				THROW_EXC( MonadException::InvalidClass, E_FAIL, L"TootleVCacheClusters" );

			SAFE_DELETE_ARRAY( faceClusters );
		}

		memcpy( pIndices, pnIBOut, dwNumFaces * 3 * sizeof( DWORD ) );

		m_pMesh->UnlockVertexBuffer();
		m_pMesh->UnlockIndexBuffer();
#endif
	}
	void CleanMesh()
	{
		TestIfInitializedMesh();

		LPD3DXMESH pNewMesh = NULL;
		DWORD nF3 = m_pMesh->GetNumFaces() * 3;

		//CComPtr<ID3DXBuffer> pBuffErr, pBuffAdjIn, pBuffAdjOut;
		LPD3DXBUFFER pBuffErr, pBuffAdjIn, pBuffAdjOut;

		D3DXCreateBuffer(1024, &pBuffErr );
		D3DXCreateBuffer(nF3 * sizeof(DWORD), &pBuffAdjIn );
		D3DXCreateBuffer(nF3 * sizeof(DWORD), &pBuffAdjOut );

		THROW_EXC_D3D(
			m_pMesh->GenerateAdjacency(0.0f,(DWORD*)( pBuffAdjIn->GetBufferPointer() ) ), L"Adj");

		THROW_EXC_D3D(
			D3DXCleanMesh( D3DXCLEAN_SIMPLIFICATION,
			m_pMesh,
			(DWORD*)( pBuffAdjIn->GetBufferPointer() ),
			&pNewMesh,
			(DWORD*)( pBuffAdjOut->GetBufferPointer() ),
			&pBuffErr), L"Clean" );

		if( m_pMesh != pNewMesh )
		{
			m_pMesh.Release();
			m_pMesh = pNewMesh;
		}
	}
	static std::wstring GetPath( const std::wstring & pFilename )
	{
		const static std::wstring m_szFolder = GetRootPath() + std::wstring( L"\\Meshes\\" );

		return m_szFolder + pFilename;
	}
	void SaveMesh( const std::wstring & pPath ) const throw(...)
	{
		TestIfInitializedMesh();

		THROW_EXC_D3D( D3DXSaveMeshToX(
			GetPath( pPath ).c_str(),
			m_pMesh,
			NULL,
			NULL,
			NULL,
			0,
			D3DXF_FILEFORMAT_TEXT | D3DXF_FILEFORMAT_COMPRESSED), L"SaveMesh");
	}
	void SimplifyMesh( const DWORD pMinValue )
	{
		TestIfInitializedMesh();

		CComPtr<ID3DXBuffer> AdjacencyBuffer;
		DWORD nF3 = m_pMesh->GetNumFaces() * 3;
		D3DXCreateBuffer(nF3 * sizeof(DWORD), &AdjacencyBuffer );
		LPD3DXMESH pNewMesh = NULL;

		THROW_EXC_D3D(
			m_pMesh->GenerateAdjacency(0.0f, (DWORD *) AdjacencyBuffer->GetBufferPointer() ), L"Adj");

		THROW_EXC_D3D(
			D3DXSimplifyMesh( m_pMesh, (DWORD *) AdjacencyBuffer->GetBufferPointer(), NULL, NULL, pMinValue, D3DXMESHSIMP_VERTEX, &pNewMesh ), L"Simplify" );

		m_pMesh.Release();
		m_pMesh = pNewMesh;
	}
	void ValidMesh( const std::wstring & pPath )
	{
		TestIfInitializedMesh();

		CComPtr<ID3DXBuffer> ErrBuf, AdjacencyBuffer;
		DWORD nF3 = m_pMesh->GetNumFaces() * 3;
		D3DXCreateBuffer( nF3 * sizeof(DWORD), &AdjacencyBuffer );
		THROW_EXC_IFFAILED(
			Direct3D, 
			m_pMesh->GenerateAdjacency( 0.0f,( DWORD* )( AdjacencyBuffer->GetBufferPointer() ) ), L"Adj");

		D3DXValidMesh( m_pMesh, (DWORD *) AdjacencyBuffer->GetBufferPointer(), &ErrBuf );

		CAtlFile FileToWrite;
		FileToWrite.Create( pPath, GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS );
		if( ErrBuf->GetBufferPointer() != NULL )
			FileToWrite.Write( ErrBuf->GetBufferPointer(), ErrBuf->GetBufferSize() );
		FileToWrite.Close();
	}
	void GetMinMax( XMVECTOR & pMax, XMVECTOR & pMin, XMVECTOR & pDelta )
	{
		THROW_EXC_IFZ( mVertices.size(), MonadException::NoDataFound, L"MinMaxMesh" );
		pMax = XMLoadFloat3( &mVertices[ 0 ].Position );
		pMin = pMax;
		XMVECTOR Source;

		for( size_t iVert = 0; mVertices.size() > iVert; iVert ++ )
		{
			Source = XMLoadFloat3( &mVertices[ iVert ].Position );
			for( int ArrId = 0; ArrId < 3; ArrId ++ )
			{
				( ( float * ) &pMax ) [ ArrId ] = max( ( ( float * ) &pMax ) [ ArrId ], ( ( float * ) &Source ) [ ArrId ] );
				( ( float * ) &pMin ) [ ArrId ] = min( ( ( float * ) &pMin ) [ ArrId ], ( ( float * ) &Source ) [ ArrId ] );
			}
		}

		pDelta = XMVectorSubtract( pMax, pMin );
	}
	void CalculateCanonicalMesh() throw(...)
	{
		XMVECTOR vMax;
		XMVECTOR vMin;
		XMVECTOR Delta;

		GetMinMax( vMax, vMin, Delta );

		// Scale
		float MaxDelta = max( max( XMVectorGetX( Delta ), XMVectorGetY( Delta ) ), XMVectorGetZ( Delta ) );

		for( size_t iVert = 0; mVertices.size() > iVert; iVert ++ )
		{
			XMVECTOR tmp = XMLoadFloat3( &mVertices[ iVert ].Position );
			tmp = XMVectorScale( tmp, 2 / MaxDelta );
			XMStoreFloat3( &mVertices[ iVert ].Position, tmp );
		}

		// Center
		GetMinMax( vMax, vMin, Delta );

		for( size_t iVert = 0; mVertices.size() > iVert; iVert ++ )
		{
			XMVECTOR tmp = XMLoadFloat3( &mVertices[ iVert ].Position );
			XMVECTOR tmpDelta = XMVectorScale( Delta, .5f );
			XMVECTOR tmpSum = XMVectorAdd( vMin, tmpDelta );
			tmp = XMVectorSubtract( tmp, tmpSum );
			XMStoreFloat3( &mVertices[ iVert ].Position, tmp );
		}
	}
	void CalculateCanonicalWallMesh() throw(...)
	{
		XMVECTOR vMax;
		XMVECTOR vMin;
		XMVECTOR Delta;

		GetMinMax( vMax, vMin, Delta );

		// Scale
		float MaxDelta = max( max( XMVectorGetX( Delta ), XMVectorGetY( Delta ) ), XMVectorGetZ( Delta ) );

		struct CPostion {
			CPostion( float & pMaxDelta )
				: mMaxDelta( pMaxDelta )
			{
			}
			void operator() ( VERTEXTYPE & pVer )
			{
				pVer.Position.x *= ( 2 / mMaxDelta );
				pVer.Position.y *= ( 2 / mMaxDelta );
				pVer.Position.z *= ( 2 / mMaxDelta );
				/*XMVECTOR tmp = XMLoadFloat3( & );
				tmp *= 2;
				tmp /= mMaxDelta;
				XMStoreFloat3( &pVer.Position, tmp );*/
			}

			const float & mMaxDelta;
		} mIterate( MaxDelta );
		BOOST_FOREACH( VERTEXTYPE & v, mVertices )
			mIterate( v );
		/*for( CVer iVert = 0; mVertices.GetSize() > iVert; iVert ++ )
		{
		mVertices[ iVert ].Position *= 2;
		mVertices[ iVert ].Position /= MaxDelta;
		}/*/

		// Center
		GetMinMax( vMax, vMin, Delta );

		struct CPosition3 {
			CPosition3( XMVECTOR & pDelta, XMVECTOR & pvMin )
				: mDelta( pDelta ), mvMin( pvMin )
			{
			}
			void operator() ( VERTEXTYPE & pVer )
			{ 
				const XMVECTOR tmp = XMLoadFloat3( &pVer.Position );
				XMStoreFloat3( &pVer.Position, XMVectorSubtract( tmp, ( XMVectorAdd( mvMin, XMVectorScale( mDelta, .5f ) ) ) ) );
			} 
			XMVECTOR & mDelta, & mvMin;
		} mDelta( Delta, vMin );
		BOOST_FOREACH( VERTEXTYPE & v, mVertices )
			mDelta( v );

		struct CPosition4 {
			CPosition4( XMVECTOR & pDelta )
				: mDelta( pDelta )
			{
			}
			void operator() ( VERTEXTYPE & pVer )
			{ 
				pVer.Position.z += abs( XMVectorGetZ( mDelta ) / 2 );
			} 
			XMVECTOR & mDelta;
		} mFinish( Delta );
		BOOST_FOREACH( VERTEXTYPE & v, mVertices )
			mFinish( v );
	}
	LPD3DXMESH GetMesh() const throw()
	{
		TestIfInitializedMesh();
		return m_pMesh;
	}

	// Variable members
	typedef CMonadCoherentVector< VERTEXTYPE > CVerticesArray;
	typedef CMonadCoherentVector< DWORD > CInidicesArray;
	CVerticesArray mVertices;
	CInidicesArray mIndices;
protected:
	struct structTriangle
	{
		structTriangle( VERTEXTYPE a, VERTEXTYPE b, VERTEXTYPE c)
		{
			VertA = a.Position;
			VertB = b.Position;
			VertC = c.Position;
		}
		XMFLOAT3 VertA, VertB, VertC;
	};
	std::map<structTriangle, bool> mTriangles;

	std::map<VERTEXTYPE, DWORD> mVerticeMap;
	void AddVertex( VERTEXTYPE & vertex ) throw(...)
	{
		DWORD idx;

		if( mVerticeMap.end() == mVerticeMap.find( vertex ) )
		{
			mVertices.push_back( vertex );
			idx = ( DWORD ) mVertices.size() - 1;
			mVerticeMap[ vertex ] = idx;
		}
		else
			idx = mVerticeMap[ vertex ];

		mIndices.push_back( idx );
	}

	CComPtr<ID3DXMesh> m_pMesh;
};

template
<
	class VERTEXTYPE
>
class CVerticesNormal : public CVertices<VERTEXTYPE>
{
public:
	XMFLOAT3 GetAvgNormal( const VERTEXTYPE & a, const VERTEXTYPE & b, const VERTEXTYPE & c ) const throw(...)
	{
		XMFLOAT3 res = a.Normal + b.Normal + c.Normal;
		res /= 3.0f;

		return res;
	}
	XMFLOAT3 GetAvgNormal( const int pTriangle ) const
	{
		VERTEXTYPE a, b, c;
		GetTriangle( pTriangle, a, b, c);

		return GetAvgNormal( a, b, c );
	}
	XMFLOAT3 CalculateTriangleNormal( const VERTEXTYPE & a, const VERTEXTYPE & b, const VERTEXTYPE & c ) const throw(...)
	{
		const XMVECTOR P = XMVectorSubtract( XMLoadFloat3( &b.Position ), XMLoadFloat3( &a.Position ) );
		const XMVECTOR Q = XMVectorSubtract( XMLoadFloat3( &c.Position ), XMLoadFloat3( &a.Position ) );
		XMVECTOR res = XMVector3Cross( P, Q );
		res = XMVector3Normalize( res );
		XMFLOAT3 ret;
		XMStoreFloat3( &ret, res );

		return ret;
	}
	XMFLOAT3 CalculateTriangleNormal( const int pTriangle ) const throw(...)
	{
		VERTEXTYPE a, b, c;
		GetTriangle( pTriangle, a, b, c);

		return CalculateTriangleNormal( a, b, c );
	}
	void AddTriangle( const VERTEXTYPE & a, const VERTEXTYPE & b, const VERTEXTYPE & c, const bool CalcTriangleNornal = false ) throw(...)
	{
		if( !bAddMe )
			return;
		VERTEXTYPE aCopy = a, bCopy = b, cCopy = c;

		if( CalcTriangleNornal )
			aCopy.Normal = bCopy.Normal = cCopy.Normal = CalculateTriangleNormal( a, b, c );

		AddVertex( aCopy );
		AddVertex( bCopy );
		AddVertex( cCopy );
	}
	void AddAndParseTriangle( const VERTEXTYPE & a, const VERTEXTYPE & b, const VERTEXTYPE & c, const bool CalcTriangleNornal = false ) throw(...)
	{
		if( !bAddMe )
			return;
		VERTEXTYPE aCopy = a, bCopy = b, cCopy = c;

		if( CalcTriangleNornal )
			aCopy.Normal = bCopy.Normal = cCopy.Normal = CalculateTriangleNormal( a, b, c );

		OnParseTriangle( aCopy, bCopy, cCopy );
	}
	void AddQuad( const VERTEXTYPE & a, const VERTEXTYPE & b, const VERTEXTYPE & c, const VERTEXTYPE & d, const bool CalcTriangleNornal = false ) throw(...)
	{
		AddTriangle( a, b, c, CalcTriangleNornal );
		AddTriangle( c, d, a, CalcTriangleNornal );
	}
};


typedef CVerticesNormal<VERTEXTEX> CMeshBuilderTex;
typedef CVerticesNormal<VERTEXPLAIN> CMeshBuilderPlain;
// typedef CVertices<VERTEXVOL> CMeshBuilderVol;

#endif