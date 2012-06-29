//--------------------------------------------------------------------------------------
// File: SDKMesh.cpp
//
// The SDK Mesh format (.sdkmesh) is not a recommended file format for games.  
// It was designed to meet the specific needs of the SDK samples.  Any real-world 
// applications should avoid this file format in favor of a destination format that 
// meets the specific needs of the application.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "SDKMesh.h"

//--------------------------------------------------------------------------------------
HRESULT CDXUTSDKMesh::CreateVertexBuffer( ID3D11Device* pd3dDevice, SDKMESH_VERTEX_BUFFER_HEADER* pHeader,
                                          void* pVertices, SDKMESH_CALLBACKS11* pLoaderCallbacks )
{
    HRESULT hr = S_OK;
    pHeader->DataOffset = 0;
    //Vertex Buffer
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = ( UINT )( pHeader->SizeBytes );
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    if( pLoaderCallbacks && pLoaderCallbacks->pCreateVertexBuffer )
    {
        pLoaderCallbacks->pCreateVertexBuffer( pd3dDevice, &pHeader->pVB11, bufferDesc, pVertices,
                                               pLoaderCallbacks->pContext );
    }
    else
    {
        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = pVertices;
        hr = pd3dDevice->CreateBuffer( &bufferDesc, &InitData, &pHeader->pVB11 );
        DXUT_SetDebugName( pHeader->pVB11, "CDXUTSDKMesh" );
    }

    return hr;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTSDKMesh::CreateIndexBuffer( ID3D11Device* pd3dDevice, SDKMESH_INDEX_BUFFER_HEADER* pHeader,
                                         void* pIndices, SDKMESH_CALLBACKS11* pLoaderCallbacks )
{
    HRESULT hr = S_OK;
    pHeader->DataOffset = 0;
    //Index Buffer
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = ( UINT )( pHeader->SizeBytes );
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    if( pLoaderCallbacks && pLoaderCallbacks->pCreateIndexBuffer )
    {
        pLoaderCallbacks->pCreateIndexBuffer( pd3dDevice, &pHeader->pIB11, bufferDesc, pIndices,
                                              pLoaderCallbacks->pContext );
    }
    else
    {
        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = pIndices;
        hr = pd3dDevice->CreateBuffer( &bufferDesc, &InitData, &pHeader->pIB11 );
        DXUT_SetDebugName( pHeader->pIB11, "MonadMesh" );
    }

    return hr;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTSDKMesh::CreateVertexBuffer( IDirect3DDevice9* pd3dDevice, SDKMESH_VERTEX_BUFFER_HEADER* pHeader,
                                          void* pVertices, SDKMESH_CALLBACKS9* pLoaderCallbacks )
{
    HRESULT hr = S_OK;

    pHeader->DataOffset = 0;
    if( pLoaderCallbacks && pLoaderCallbacks->pCreateVertexBuffer )
    {
        pLoaderCallbacks->pCreateVertexBuffer( pd3dDevice, &pHeader->pVB9, ( UINT )pHeader->SizeBytes,
                                               D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, pVertices,
                                               pLoaderCallbacks->pContext );
    }
    else
    {
        hr = pd3dDevice->CreateVertexBuffer( ( UINT )pHeader->SizeBytes,
                                             D3DUSAGE_WRITEONLY,
                                             0,
                                             D3DPOOL_DEFAULT,
                                             &pHeader->pVB9,
                                             NULL );

        //lock
        if( SUCCEEDED( hr ) )
        {
            void* pLockedVerts = NULL;
            V_RETURN( pHeader->pVB9->Lock( 0, 0, &pLockedVerts, 0 ) );
            CopyMemory( pLockedVerts, pVertices, ( size_t )pHeader->SizeBytes );
            pHeader->pVB9->Unlock();
        }
    }

    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT CDXUTSDKMesh::CreateIndexBuffer( IDirect3DDevice9* pd3dDevice, SDKMESH_INDEX_BUFFER_HEADER* pHeader,
                                         void* pIndices, SDKMESH_CALLBACKS9* pLoaderCallbacks )
{
    HRESULT hr = S_OK;

    pHeader->DataOffset = 0;

    D3DFORMAT ibFormat = D3DFMT_INDEX16;
    switch( pHeader->IndexType )
    {
        case IT_16BIT:
            ibFormat = D3DFMT_INDEX16;
            break;
        case IT_32BIT:
            ibFormat = D3DFMT_INDEX32;
            break;
    };

    if( pLoaderCallbacks && pLoaderCallbacks->pCreateIndexBuffer )
    {
        pLoaderCallbacks->pCreateIndexBuffer( pd3dDevice, &pHeader->pIB9, ( UINT )pHeader->SizeBytes,
                                              D3DUSAGE_WRITEONLY, ibFormat, D3DPOOL_DEFAULT, pIndices,
                                              pLoaderCallbacks->pContext );
    }
    else
    {
        hr = pd3dDevice->CreateIndexBuffer( ( UINT )( pHeader->SizeBytes ),
                                            D3DUSAGE_WRITEONLY,
                                            ibFormat,
                                            D3DPOOL_DEFAULT,
                                            &pHeader->pIB9,
                                            NULL );

        if( SUCCEEDED( hr ) )
        {
            void* pLockedIndices = NULL;
            V_RETURN( pHeader->pIB9->Lock( 0, 0, &pLockedIndices, 0 ) );
            CopyMemory( pLockedIndices, pIndices, ( size_t )( pHeader->SizeBytes ) );
            pHeader->pIB9->Unlock();
        }
    }

    return hr;
}

HRESULT CDXUTSDKMesh::CreateFromMemory( ID3D11Device* pDev11,
                                        IDirect3DDevice9* pDev9,
                                        BYTE* pData,
                                        UINT DataBytes,
                                        bool bCreateAdjacencyIndices,
                                        bool bCopyStatic,
                                        SDKMESH_CALLBACKS11* pLoaderCallbacks11,
                                        SDKMESH_CALLBACKS9* pLoaderCallbacks9 )
{
    HRESULT hr = E_FAIL;
    D3DXVECTOR3 lower; 
    D3DXVECTOR3 upper; 
    
    m_pDev9 = pDev9;
	m_pDev11 = pDev11;

    // Set outstanding resources to zero
    m_NumOutstandingResources = 0;

    if( bCopyStatic )
    {
        SDKMESH_HEADER* pHeader = ( SDKMESH_HEADER* )pData;

        SIZE_T StaticSize = ( SIZE_T )( pHeader->HeaderSize + pHeader->NonBufferDataSize );
        m_pHeapData = new BYTE[ StaticSize ];
        if( !m_pHeapData )
            return hr;

        m_pStaticMeshData = m_pHeapData;

        CopyMemory( m_pStaticMeshData, pData, StaticSize );
    }
    else
    {
        m_pHeapData = pData;
        m_pStaticMeshData = pData;
    }

    // Pointer fixup
    m_pMeshHeader = ( SDKMESH_HEADER* )m_pStaticMeshData;
    m_pVertexBufferArray = ( SDKMESH_VERTEX_BUFFER_HEADER* )( m_pStaticMeshData +
                                                              m_pMeshHeader->VertexStreamHeadersOffset );
    m_pIndexBufferArray = ( SDKMESH_INDEX_BUFFER_HEADER* )( m_pStaticMeshData +
                                                            m_pMeshHeader->IndexStreamHeadersOffset );
    m_pMeshArray = ( SDKMESH_MESH* )( m_pStaticMeshData + m_pMeshHeader->MeshDataOffset );
    m_pSubsetArray = ( SDKMESH_SUBSET* )( m_pStaticMeshData + m_pMeshHeader->SubsetDataOffset );
    m_pFrameArray = ( SDKMESH_FRAME* )( m_pStaticMeshData + m_pMeshHeader->FrameDataOffset );
    m_pMaterialArray = ( SDKMESH_MATERIAL* )( m_pStaticMeshData + m_pMeshHeader->MaterialDataOffset );

    // Setup subsets
    for( UINT i = 0; i < m_pMeshHeader->NumMeshes; i++ )
    {
        m_pMeshArray[i].pSubsets = ( UINT* )( m_pStaticMeshData + m_pMeshArray[i].SubsetOffset );
        m_pMeshArray[i].pFrameInfluences = ( UINT* )( m_pStaticMeshData + m_pMeshArray[i].FrameInfluenceOffset );
    }

    // error condition
    if( m_pMeshHeader->Version != SDKMESH_FILE_VERSION )
    {
        hr = E_NOINTERFACE;
        goto Error;
    }

    // Setup buffer data pointer
    BYTE* pBufferData = pData + m_pMeshHeader->HeaderSize + m_pMeshHeader->NonBufferDataSize;

    // Get the start of the buffer data
    UINT64 BufferDataStart = m_pMeshHeader->HeaderSize + m_pMeshHeader->NonBufferDataSize;

    // Create VBs
    m_ppVertices = new BYTE*[m_pMeshHeader->NumVertexBuffers];
    for( UINT i = 0; i < m_pMeshHeader->NumVertexBuffers; i++ )
    {
        BYTE* pVertices = NULL;
        pVertices = ( BYTE* )( pBufferData + ( m_pVertexBufferArray[i].DataOffset - BufferDataStart ) );

        if( pDev11 )
            CreateVertexBuffer( pDev11, &m_pVertexBufferArray[i], pVertices, pLoaderCallbacks11 );
        else if( pDev9 )
            CreateVertexBuffer( pDev9, &m_pVertexBufferArray[i], pVertices, pLoaderCallbacks9 );

        m_ppVertices[i] = pVertices;
    }

    // Create IBs
    m_ppIndices = new BYTE*[m_pMeshHeader->NumIndexBuffers];
    for( UINT i = 0; i < m_pMeshHeader->NumIndexBuffers; i++ )
    {
        BYTE* pIndices = NULL;
        pIndices = ( BYTE* )( pBufferData + ( m_pIndexBufferArray[i].DataOffset - BufferDataStart ) );

        if( pDev11 )
            CreateIndexBuffer( pDev11, &m_pIndexBufferArray[i], pIndices, pLoaderCallbacks11 );
        else if( pDev9 )
            CreateIndexBuffer( pDev9, &m_pIndexBufferArray[i], pIndices, pLoaderCallbacks9 );

        m_ppIndices[i] = pIndices;
    }

    // Create a place to store our bind pose frame matrices
    //m_pBindPoseFrameMatrices = new D3DXMATRIX[ m_pMeshHeader->NumFrames ];
    //if( !m_pBindPoseFrameMatrices )
        //goto Error;

    // Create a place to store our transformed frame matrices
    //m_pTransformedFrameMatrices = new D3DXMATRIX[ m_pMeshHeader->NumFrames ];
    //if( !m_pTransformedFrameMatrices )
        //goto Error;
    //m_pWorldPoseFrameMatrices = new D3DXMATRIX[ m_pMeshHeader->NumFrames ];
    //if( !m_pWorldPoseFrameMatrices )
        //goto Error;

    SDKMESH_SUBSET* pSubset = NULL;
    D3D11_PRIMITIVE_TOPOLOGY PrimType;

    // update bounding volume 
    SDKMESH_MESH* currentMesh = &m_pMeshArray[0];
    int tris = 0;
    for (UINT meshi=0; meshi < m_pMeshHeader->NumMeshes; ++meshi) {
        lower.x = FLT_MAX; lower.y = FLT_MAX; lower.z = FLT_MAX;
        upper.x = -FLT_MAX; upper.y = -FLT_MAX; upper.z = -FLT_MAX;
        currentMesh = GetMesh( meshi );
        INT indsize;
        if (m_pIndexBufferArray[currentMesh->IndexBuffer].IndexType == IT_16BIT ) {
            indsize = 2;
        }else {
            indsize = 4;        
        }

        for( UINT subset = 0; subset < currentMesh->NumSubsets; subset++ )
        {
            pSubset = GetSubset( meshi, subset ); //&m_pSubsetArray[ currentMesh->pSubsets[subset] ];

            PrimType = GetPrimitiveType11( ( SDKMESH_PRIMITIVE_TYPE )pSubset->PrimitiveType );
            assert( PrimType == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );// only triangle lists are handled.

            UINT IndexCount = ( UINT )pSubset->IndexCount;
            UINT IndexStart = ( UINT )pSubset->IndexStart;

            /*if( bAdjacent )
            {
                IndexCount *= 2;
                IndexStart *= 2;
            }*/
     
        //BYTE* pIndices = NULL;
            //m_ppIndices[i]
            UINT *ind = ( UINT * )m_ppIndices[currentMesh->IndexBuffer];
            FLOAT *verts =  ( FLOAT* )m_ppVertices[currentMesh->VertexBuffers[0]];
            UINT stride = (UINT)m_pVertexBufferArray[currentMesh->VertexBuffers[0]].StrideBytes;
            assert (stride % 4 == 0);
            stride /=4;
            for (UINT vertind = IndexStart; vertind < IndexStart + IndexCount; ++vertind) {
                UINT current_ind=0;
                if (indsize == 2) {
                    UINT ind_div2 = vertind / 2;
                    current_ind = ind[ind_div2];
                    if (vertind %2 ==0) {
                        current_ind = current_ind << 16;
                        current_ind = current_ind >> 16;
                    }else {
                        current_ind = current_ind >> 16;
                    }
                }else {
                    current_ind = ind[vertind];
                }
                tris++;
                D3DXVECTOR3 *pt = (D3DXVECTOR3*)&(verts[stride * current_ind]);
                if (pt->x < lower.x) {
                    lower.x = pt->x;
                }
                if (pt->y < lower.y) {
                    lower.y = pt->y;
                }
                if (pt->z < lower.z) {
                    lower.z = pt->z;
                }
                if (pt->x > upper.x) {
                    upper.x = pt->x;
                }
                if (pt->y > upper.y) {
                    upper.y = pt->y;
                }
                if (pt->z > upper.z) {
                    upper.z = pt->z;
                }
                //BYTE** m_ppVertices;
                //BYTE** m_ppIndices;
            }
            //pd3dDeviceContext->DrawIndexed( IndexCount, IndexStart, VertexStart );
        }

        D3DXVECTOR3 half = upper - lower;
        half *=0.5f;

        currentMesh->BoundingBoxCenter = lower + half;
        currentMesh->BoundingBoxExtents = half;

    }
    // Update 
        


    hr = S_OK;
Error:

    if( !pLoaderCallbacks9 )
    {
        CheckLoadDone();
    }

    return hr;
}


#define MAX_D3D11_VERTEX_STREAMS D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
//--------------------------------------------------------------------------------------
void CDXUTSDKMesh::RenderMesh( UINT iMesh,
                               bool bAdjacent,
                               ID3D11DeviceContext* pd3dDeviceContext )
{
    if( 0 < GetOutstandingBufferResources() )
        return;

    SDKMESH_MESH* pMesh = &m_pMeshArray[iMesh];

    UINT Strides[MAX_D3D11_VERTEX_STREAMS];
    UINT Offsets[MAX_D3D11_VERTEX_STREAMS];
    ID3D11Buffer* pVB[MAX_D3D11_VERTEX_STREAMS];

    if( pMesh->NumVertexBuffers > MAX_D3D11_VERTEX_STREAMS )
        return;

    for( UINT64 i = 0; i < pMesh->NumVertexBuffers; i++ )
    {
        pVB[i] = m_pVertexBufferArray[ pMesh->VertexBuffers[i] ].pVB11;
        Strides[i] = ( UINT )m_pVertexBufferArray[ pMesh->VertexBuffers[i] ].StrideBytes;
        Offsets[i] = 0;
    }

    SDKMESH_INDEX_BUFFER_HEADER* pIndexBufferArray;
    if( bAdjacent )
        pIndexBufferArray = m_pAdjacencyIndexBufferArray;
    else
        pIndexBufferArray = m_pIndexBufferArray;

    ID3D11Buffer* pIB = pIndexBufferArray[ pMesh->IndexBuffer ].pIB11;
    DXGI_FORMAT ibFormat = DXGI_FORMAT_R16_UINT;
    switch( pIndexBufferArray[ pMesh->IndexBuffer ].IndexType )
    {
    case IT_16BIT:
        ibFormat = DXGI_FORMAT_R16_UINT;
        break;
    case IT_32BIT:
        ibFormat = DXGI_FORMAT_R32_UINT;
        break;
    };

    pd3dDeviceContext->IASetVertexBuffers( 0, pMesh->NumVertexBuffers, pVB, Strides, Offsets );
    pd3dDeviceContext->IASetIndexBuffer( pIB, ibFormat, 0 );

    SDKMESH_SUBSET* pSubset = NULL;
    D3D11_PRIMITIVE_TOPOLOGY PrimType;

    for( UINT subset = 0; subset < pMesh->NumSubsets; subset++ )
    {
        pSubset = &m_pSubsetArray[ pMesh->pSubsets[subset] ];

        PrimType = GetPrimitiveType11( ( SDKMESH_PRIMITIVE_TYPE )pSubset->PrimitiveType );
        if( bAdjacent )
        {
            switch( PrimType )
            {
            case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
                PrimType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
                break;
            case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
                PrimType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
                break;
            case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
                PrimType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
                break;
            case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
                PrimType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
                break;
            }
        }

        pd3dDeviceContext->IASetPrimitiveTopology( PrimType );

        UINT IndexCount = ( UINT )pSubset->IndexCount;
        UINT IndexStart = ( UINT )pSubset->IndexStart;
        UINT VertexStart = ( UINT )pSubset->VertexStart;
        if( bAdjacent )
        {
            IndexCount *= 2;
            IndexStart *= 2;
        }

        pd3dDeviceContext->DrawIndexed( IndexCount, IndexStart, VertexStart );
    }
}

//--------------------------------------------------------------------------------------
void CDXUTSDKMesh::RenderFrame( UINT iFrame,
                                bool bAdjacent,
                                ID3D11DeviceContext* pd3dDeviceContext )
{
    if( !m_pStaticMeshData || !m_pFrameArray )
        return;

    if( m_pFrameArray[iFrame].Mesh != INVALID_MESH )
    {
        RenderMesh( m_pFrameArray[iFrame].Mesh,
                    bAdjacent,
                    pd3dDeviceContext );
    }

    // Render our children
    if( m_pFrameArray[iFrame].ChildFrame != INVALID_FRAME )
        RenderFrame( m_pFrameArray[iFrame].ChildFrame, bAdjacent, pd3dDeviceContext );

    // Render our siblings
    if( m_pFrameArray[iFrame].SiblingFrame != INVALID_FRAME )
        RenderFrame( m_pFrameArray[iFrame].SiblingFrame, bAdjacent, pd3dDeviceContext );
}

//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
void CDXUTSDKMesh::RenderMesh( UINT iMesh,
                               LPDIRECT3DDEVICE9 pd3dDevice,
                               LPD3DXEFFECT pEffect,
                               D3DXHANDLE hTechnique )
{
    if( 0 < GetOutstandingBufferResources() )
        return;

    SDKMESH_MESH* pMesh = &m_pMeshArray[iMesh];

    // set vb streams
    for( UINT i = 0; i < ( UINT )pMesh->NumVertexBuffers; i++ )
    {
        pd3dDevice->SetStreamSource( i,
                                     m_pVertexBufferArray[ pMesh->VertexBuffers[i] ].pVB9,
                                     0,
                                     ( UINT )m_pVertexBufferArray[ pMesh->VertexBuffers[i] ].StrideBytes );
    }

    // Set our index buffer as well
    pd3dDevice->SetIndices( m_pIndexBufferArray[ pMesh->IndexBuffer ].pIB9 );

    // Render the scene with this technique 
    pEffect->SetTechnique( hTechnique );

    SDKMESH_SUBSET* pSubset = NULL;
    // SDKMESH_MATERIAL* pMat = NULL;
    D3DPRIMITIVETYPE PrimType;
    UINT cPasses = 0;
    pEffect->Begin( &cPasses, 0 );

    for( UINT p = 0; p < cPasses; ++p )
    {
        pEffect->BeginPass( p );

        for( UINT subset = 0; subset < pMesh->NumSubsets; subset++ )
        {
            pSubset = &m_pSubsetArray[ pMesh->pSubsets[subset] ];

            PrimType = GetPrimitiveType9( ( SDKMESH_PRIMITIVE_TYPE )pSubset->PrimitiveType );

            pEffect->CommitChanges();

            UINT PrimCount = ( UINT )pSubset->IndexCount;
            UINT IndexStart = ( UINT )pSubset->IndexStart;
            UINT VertexStart = ( UINT )pSubset->VertexStart;
            UINT VertexCount = ( UINT )pSubset->VertexCount;
            if( D3DPT_TRIANGLELIST == PrimType )
                PrimCount /= 3;
            if( D3DPT_LINELIST == PrimType )
                PrimCount /= 2;
            if( D3DPT_TRIANGLESTRIP == PrimType )
                PrimCount = ( PrimCount - 3 ) + 1;
            if( D3DPT_LINESTRIP == PrimType )
                PrimCount -= 1;

            pd3dDevice->DrawIndexedPrimitive( PrimType, VertexStart, 0, VertexCount, IndexStart, PrimCount );
        }

        pEffect->EndPass();
    }

    pEffect->End();
}

//--------------------------------------------------------------------------------------
void CDXUTSDKMesh::RenderFrame( UINT iFrame,
                                LPDIRECT3DDEVICE9 pd3dDevice,
                                LPD3DXEFFECT pEffect,
                                D3DXHANDLE hTechnique )
{
    if( !m_pStaticMeshData || !m_pFrameArray )
        return;

    if( m_pFrameArray[iFrame].Mesh != INVALID_MESH )
    {
        RenderMesh( m_pFrameArray[iFrame].Mesh,
                    pd3dDevice,
                    pEffect,
                    hTechnique );
    }

    // Render our children
    if( m_pFrameArray[iFrame].ChildFrame != INVALID_FRAME )
        RenderFrame( m_pFrameArray[iFrame].ChildFrame, pd3dDevice, pEffect, hTechnique );

    // Render our siblings
    if( m_pFrameArray[iFrame].SiblingFrame != INVALID_FRAME )
        RenderFrame( m_pFrameArray[iFrame].SiblingFrame, pd3dDevice, pEffect, hTechnique );
}


//--------------------------------------------------------------------------------------
CDXUTSDKMesh::CDXUTSDKMesh() : m_NumOutstandingResources( 0 ),
                               m_bLoading( false ),
                               m_hFile( 0 ),
                               m_hFileMappingObject( 0 ),
                               m_pMeshHeader( NULL ),
                               m_pStaticMeshData( NULL ),
                               m_pHeapData( NULL ),
                               m_pAdjacencyIndexBufferArray( NULL ),
                               m_ppVertices( NULL ),
                               m_ppIndices( NULL ),
                               m_pDev9( NULL ),
							   m_pDev11( NULL )
{
}


//--------------------------------------------------------------------------------------
CDXUTSDKMesh::~CDXUTSDKMesh()
{
    Destroy();
}

//--------------------------------------------------------------------------------------
HRESULT CDXUTSDKMesh::Create( ID3D11Device* pDev11, BYTE* pData, UINT DataBytes, bool bCreateAdjacencyIndices,
                              bool bCopyStatic, SDKMESH_CALLBACKS11* pLoaderCallbacks )
{
    return CreateFromMemory( pDev11, NULL, pData, DataBytes, bCreateAdjacencyIndices, bCopyStatic,
                             pLoaderCallbacks,  NULL );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTSDKMesh::Create( IDirect3DDevice9* pDev9, BYTE* pData, UINT DataBytes, bool bCreateAdjacencyIndices,
                              bool bCopyStatic, SDKMESH_CALLBACKS9* pLoaderCallbacks )
{
    return CreateFromMemory( NULL, pDev9, pData, DataBytes, bCreateAdjacencyIndices, bCopyStatic, NULL, 
                             pLoaderCallbacks );
}

//--------------------------------------------------------------------------------------
void CDXUTSDKMesh::Destroy()
{
    if( !CheckLoadDone() )
        return;

    if( m_pStaticMeshData )
    {
        for( UINT64 i = 0; i < m_pMeshHeader->NumVertexBuffers; i++ )
        {
            if( !IsErrorResource( m_pVertexBufferArray[i].pVB9 ) )
                SAFE_RELEASE( m_pVertexBufferArray[i].pVB9 );
        }

        for( UINT64 i = 0; i < m_pMeshHeader->NumIndexBuffers; i++ )
        {
            if( !IsErrorResource( m_pIndexBufferArray[i].pIB9 ) )
                SAFE_RELEASE( m_pIndexBufferArray[i].pIB9 );
        }
    }

    if( m_pAdjacencyIndexBufferArray )
    {
        for( UINT64 i = 0; i < m_pMeshHeader->NumIndexBuffers; i++ )
        {
            SAFE_RELEASE( m_pAdjacencyIndexBufferArray[i].pIB11 );
        }
    }
    SAFE_DELETE_ARRAY( m_pAdjacencyIndexBufferArray );

    SAFE_DELETE_ARRAY( m_pHeapData );
    m_pStaticMeshData = NULL;
    
    SAFE_DELETE_ARRAY( m_ppVertices );
    SAFE_DELETE_ARRAY( m_ppIndices );

    m_pMeshHeader = NULL;
    m_pVertexBufferArray = NULL;
    m_pIndexBufferArray = NULL;
    m_pMeshArray = NULL;
    m_pSubsetArray = NULL;
    m_pFrameArray = NULL;
    m_pMaterialArray = NULL;
}


//--------------------------------------------------------------------------------------
void CDXUTSDKMesh::Render( ID3D11DeviceContext* pd3dDeviceContext )
{
    RenderFrame( 0, false, pd3dDeviceContext );
}


//--------------------------------------------------------------------------------------
void CDXUTSDKMesh::Render( LPDIRECT3DDEVICE9 pd3dDevice,
                           LPD3DXEFFECT pEffect,
                           D3DXHANDLE hTechnique )
{
    RenderFrame( 0, pd3dDevice, pEffect, hTechnique );
}

//--------------------------------------------------------------------------------------
D3D11_PRIMITIVE_TOPOLOGY CDXUTSDKMesh::GetPrimitiveType11( SDKMESH_PRIMITIVE_TYPE PrimType )
{
    D3D11_PRIMITIVE_TOPOLOGY retType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    switch( PrimType )
    {
        case PT_TRIANGLE_LIST:
            retType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        case PT_TRIANGLE_STRIP:
            retType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
            break;
        case PT_LINE_LIST:
            retType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        case PT_LINE_STRIP:
            retType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
            break;
        case PT_POINT_LIST:
            retType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
            break;
        case PT_TRIANGLE_LIST_ADJ:
            retType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
            break;
        case PT_TRIANGLE_STRIP_ADJ:
            retType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
            break;
        case PT_LINE_LIST_ADJ:
            retType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
            break;
        case PT_LINE_STRIP_ADJ:
            retType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
            break;
    };

    return retType;
}

//--------------------------------------------------------------------------------------
DXGI_FORMAT CDXUTSDKMesh::GetIBFormat11( UINT iMesh )
{
    switch( m_pIndexBufferArray[ m_pMeshArray[ iMesh ].IndexBuffer ].IndexType )
    {
        case IT_16BIT:
            return DXGI_FORMAT_R16_UINT;
        case IT_32BIT:
            return DXGI_FORMAT_R32_UINT;
    };
    return DXGI_FORMAT_R16_UINT;
}

//--------------------------------------------------------------------------------------
ID3D11Buffer* CDXUTSDKMesh::GetVB11( UINT iMesh, UINT iVB )
{
    return m_pVertexBufferArray[ m_pMeshArray[ iMesh ].VertexBuffers[iVB] ].pVB11;
}

//--------------------------------------------------------------------------------------
ID3D11Buffer* CDXUTSDKMesh::GetIB11( UINT iMesh )
{
    return m_pIndexBufferArray[ m_pMeshArray[ iMesh ].IndexBuffer ].pIB11;
}
SDKMESH_INDEX_TYPE CDXUTSDKMesh::GetIndexType( UINT iMesh ) 
{
    return ( SDKMESH_INDEX_TYPE ) m_pIndexBufferArray[m_pMeshArray[ iMesh ].IndexBuffer].IndexType;
}
//--------------------------------------------------------------------------------------
ID3D11Buffer* CDXUTSDKMesh::GetAdjIB11( UINT iMesh )
{
    return m_pAdjacencyIndexBufferArray[ m_pMeshArray[ iMesh ].IndexBuffer ].pIB11;
}

//--------------------------------------------------------------------------------------
D3DPRIMITIVETYPE CDXUTSDKMesh::GetPrimitiveType9( SDKMESH_PRIMITIVE_TYPE PrimType )
{
    D3DPRIMITIVETYPE retType = D3DPT_TRIANGLELIST;

    switch( PrimType )
    {
        case PT_TRIANGLE_LIST:
            retType = D3DPT_TRIANGLELIST;
            break;
        case PT_TRIANGLE_STRIP:
            retType = D3DPT_TRIANGLESTRIP;
            break;
        case PT_LINE_LIST:
            retType = D3DPT_LINELIST;
            break;
        case PT_LINE_STRIP:
            retType = D3DPT_LINESTRIP;
            break;
        case PT_POINT_LIST:
            retType = D3DPT_POINTLIST;
            break;
    };

    return retType;
}

//--------------------------------------------------------------------------------------
D3DFORMAT CDXUTSDKMesh::GetIBFormat9( UINT iMesh )
{
    switch( m_pIndexBufferArray[ m_pMeshArray[ iMesh ].IndexBuffer ].IndexType )
    {
        case IT_16BIT:
            return D3DFMT_INDEX16;
        case IT_32BIT:
            return D3DFMT_INDEX32;
    };
    return D3DFMT_INDEX16;
}

//--------------------------------------------------------------------------------------
IDirect3DVertexBuffer9* CDXUTSDKMesh::GetVB9( UINT iMesh, UINT iVB )
{
    return m_pVertexBufferArray[ m_pMeshArray[ iMesh ].VertexBuffers[iVB] ].pVB9;
}

//--------------------------------------------------------------------------------------
IDirect3DIndexBuffer9* CDXUTSDKMesh::GetIB9( UINT iMesh )
{
    return m_pIndexBufferArray[ m_pMeshArray[ iMesh ].IndexBuffer ].pIB9;
}

//--------------------------------------------------------------------------------------
char* CDXUTSDKMesh::GetMeshPathA()
{
    return m_strPath;
}

//--------------------------------------------------------------------------------------
WCHAR* CDXUTSDKMesh::GetMeshPathW()
{
    return m_strPathW;
}

//--------------------------------------------------------------------------------------
UINT CDXUTSDKMesh::GetNumMeshes()
{
    if( !m_pMeshHeader )
        return 0;
    return m_pMeshHeader->NumMeshes;
}

//--------------------------------------------------------------------------------------
UINT CDXUTSDKMesh::GetNumMaterials()
{
    if( !m_pMeshHeader )
        return 0;
    return m_pMeshHeader->NumMaterials;
}

//--------------------------------------------------------------------------------------
UINT CDXUTSDKMesh::GetNumVBs()
{
    if( !m_pMeshHeader )
        return 0;
    return m_pMeshHeader->NumVertexBuffers;
}

//--------------------------------------------------------------------------------------
UINT CDXUTSDKMesh::GetNumIBs()
{
    if( !m_pMeshHeader )
        return 0;
    return m_pMeshHeader->NumIndexBuffers;
}

//--------------------------------------------------------------------------------------
ID3D11Buffer* CDXUTSDKMesh::GetVB11At( UINT iVB )
{
    return m_pVertexBufferArray[ iVB ].pVB11;
}

//--------------------------------------------------------------------------------------
ID3D11Buffer* CDXUTSDKMesh::GetIB11At( UINT iIB )
{
    return m_pIndexBufferArray[ iIB ].pIB11;
}

//--------------------------------------------------------------------------------------
IDirect3DVertexBuffer9* CDXUTSDKMesh::GetVB9At( UINT iVB )
{
    return m_pVertexBufferArray[ iVB ].pVB9;
}

//--------------------------------------------------------------------------------------
IDirect3DIndexBuffer9* CDXUTSDKMesh::GetIB9At( UINT iIB )
{
    return m_pIndexBufferArray[ iIB ].pIB9;
}

//--------------------------------------------------------------------------------------
BYTE* CDXUTSDKMesh::GetRawVerticesAt( UINT iVB )
{
    return m_ppVertices[iVB];
}

//--------------------------------------------------------------------------------------
BYTE* CDXUTSDKMesh::GetRawIndicesAt( UINT iIB )
{
    return m_ppIndices[iIB];
}

//--------------------------------------------------------------------------------------
SDKMESH_MATERIAL* CDXUTSDKMesh::GetMaterial( UINT iMaterial )
{
    return &m_pMaterialArray[ iMaterial ];
}

//--------------------------------------------------------------------------------------
SDKMESH_MESH* CDXUTSDKMesh::GetMesh( UINT iMesh )
{
    return &m_pMeshArray[ iMesh ];
}

//--------------------------------------------------------------------------------------
UINT CDXUTSDKMesh::GetNumSubsets( UINT iMesh )
{
    return m_pMeshArray[ iMesh ].NumSubsets;
}

//--------------------------------------------------------------------------------------
SDKMESH_SUBSET* CDXUTSDKMesh::GetSubset( UINT iMesh, UINT iSubset )
{
    return &m_pSubsetArray[ m_pMeshArray[ iMesh ].pSubsets[iSubset] ];
}

//--------------------------------------------------------------------------------------
UINT CDXUTSDKMesh::GetVertexStride( UINT iMesh, UINT iVB )
{
    return ( UINT )m_pVertexBufferArray[ m_pMeshArray[ iMesh ].VertexBuffers[iVB] ].StrideBytes;
}

//--------------------------------------------------------------------------------------
UINT64 CDXUTSDKMesh::GetNumVertices( UINT iMesh, UINT iVB )
{
    return m_pVertexBufferArray[ m_pMeshArray[ iMesh ].VertexBuffers[iVB] ].NumVertices;
}

//--------------------------------------------------------------------------------------
UINT64 CDXUTSDKMesh::GetNumIndices( UINT iMesh )
{
    return m_pIndexBufferArray[ m_pMeshArray[ iMesh ].IndexBuffer ].NumIndices;
}

//--------------------------------------------------------------------------------------
D3DXVECTOR3 CDXUTSDKMesh::GetMeshBBoxCenter( UINT iMesh )
{
    return m_pMeshArray[iMesh].BoundingBoxCenter;
}

//--------------------------------------------------------------------------------------
D3DXVECTOR3 CDXUTSDKMesh::GetMeshBBoxExtents( UINT iMesh )
{
    return m_pMeshArray[iMesh].BoundingBoxExtents;
}

//--------------------------------------------------------------------------------------
UINT CDXUTSDKMesh::GetOutstandingResources()
{
	return 0;
}

//--------------------------------------------------------------------------------------
UINT CDXUTSDKMesh::GetOutstandingBufferResources()
{
    UINT outstandingResources = 0;
    if( !m_pMeshHeader )
        return 1;

    for( UINT i = 0; i < m_pMeshHeader->NumVertexBuffers; i++ )
    {
        if( !m_pVertexBufferArray[i].pVB9 && !IsErrorResource( m_pVertexBufferArray[i].pVB9 ) )
            outstandingResources ++;
    }

    for( UINT i = 0; i < m_pMeshHeader->NumIndexBuffers; i++ )
    {
        if( !m_pIndexBufferArray[i].pIB9 && !IsErrorResource( m_pIndexBufferArray[i].pIB9 ) )
            outstandingResources ++;
    }

    return outstandingResources;
}

//--------------------------------------------------------------------------------------
bool CDXUTSDKMesh::CheckLoadDone()
{
    if( 0 == GetOutstandingResources() )
    {
        m_bLoading = false;
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------
bool CDXUTSDKMesh::IsLoaded()
{
    if( m_pStaticMeshData && !m_bLoading )
    {
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------
bool CDXUTSDKMesh::IsLoading()
{
    return m_bLoading;
}

//--------------------------------------------------------------------------------------
void CDXUTSDKMesh::SetLoading( bool bLoading )
{
    m_bLoading = bLoading;
}

//--------------------------------------------------------------------------------------
BOOL CDXUTSDKMesh::HadLoadingError()
{
    if( m_pMeshHeader )
    {
        for( UINT i = 0; i < m_pMeshHeader->NumVertexBuffers; i++ )
        {
            if( IsErrorResource( m_pVertexBufferArray[i].pVB9 ) )
                return TRUE;
        }

        for( UINT i = 0; i < m_pMeshHeader->NumIndexBuffers; i++ )
        {
            if( IsErrorResource( m_pIndexBufferArray[i].pIB9 ) )
                return TRUE;
        }
    }

    return FALSE;
}
