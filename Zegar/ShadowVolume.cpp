#include "stdafx.h"
#include "ShadowVolume.h"

//-----------------------------------------------------------------------------
// Name: AddEdge()
// Desc: Adds an edge to a list of silohuette edges of a shadow volume.
//-----------------------------------------------------------------------------
VOID AddEdge( WORD* pEdges, WORD& dwNumEdges, WORD v0, WORD v1 )
{
    // Remove interior edges (which appear in the list twice)
    for( WORD i=0; i < dwNumEdges; i++ )
    {
        if( ( pEdges[2*i+0] == v0 && pEdges[2*i+1] == v1 ) ||
            ( pEdges[2*i+0] == v1 && pEdges[2*i+1] == v0 ) )
        {
            if( dwNumEdges > 1 )
            {
                pEdges[2*i+0] = pEdges[2*(dwNumEdges-1)+0];
                pEdges[2*i+1] = pEdges[2*(dwNumEdges-1)+1];
            }
            dwNumEdges--;
            return;
        }
    }

    pEdges[2*dwNumEdges+0] = v0;
    pEdges[2*dwNumEdges+1] = v1;
    dwNumEdges++;
}
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT CShadowVolume::Render( LPDIRECT3DDEVICE8 pd3dDevice )
{
    //pd3dDevice->SetVertexShader( NULL );
	pd3dDevice->SetVertexShader( D3DFVF_XYZ );

    return pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_dwNumVertices/3,
                                        m_pVertices, sizeof(D3DXVECTOR3) );
}

//-----------------------------------------------------------------------------
// Name: BuildFromMesh()
// Desc: Takes a mesh as input, and uses it to build a shadowvolume. The
//       technique used considers each triangle of the mesh, and adds its
//       edges to a temporary list. The edge list is maintained, such that
//       only silohuette edges are kept. Finally, the silohuette edges are
//       extruded to make the shadow volume vertex list.
//-----------------------------------------------------------------------------
HRESULT CShadowVolume::AddFromPrimitive(
		class CPrymityw & pPrymityw)
{
    const WORD dwNumVertices = pPrymityw.m_il_punkt; //pMesh->GetNumVertices();
    const WORD dwNumFaces    = pPrymityw.m_il_trojk; //pMesh->GetNumFaces();

    // Allocate a temporary edge list
    WORD* pEdges = new WORD [dwNumFaces * 6];
    WORD dwNumEdges = 0;

    // For each face
    for( WORD i=0; i<dwNumFaces; i++ )
    {

        WORD wFace0; // = pIndices[3*i+0];
        WORD wFace1; // = pIndices[3*i+1];
        WORD wFace2; // = pIndices[3*i+2];
		pPrymityw.DajIndeksy(i, wFace2, wFace1, wFace0);

        D3DXVECTOR3 v0 = * pPrymityw.WezPunkt(wFace0);
        D3DXVECTOR3 v1 = * pPrymityw.WezPunkt(wFace1);
        D3DXVECTOR3 v2 = * pPrymityw.WezPunkt(wFace2);

        // Transform vertices or transform light?
        D3DXVECTOR3 vNormal;
        D3DXVec3Cross( &vNormal, &(v2-v1), &(v1-v0) );

        if( D3DXVec3Dot( &vNormal, &vLight ) <= 0.0f )
        {
            AddEdge( pEdges, dwNumEdges, wFace0, wFace1 );
            AddEdge( pEdges, dwNumEdges, wFace1, wFace2 );
            AddEdge( pEdges, dwNumEdges, wFace2, wFace0 );
        }
    }

	static DWORD dwPrevEdges = dwNumEdges;
	if(m_pVertices == NULL || dwPrevEdges < dwNumEdges)
	{
		SAFE_DELETE_ARRAY(m_pVertices);
		m_pVertices = new D3DXVECTOR3 [ dwNumEdges * 6 ];
	}

	for( i=0; i<dwNumEdges; i++ )
    {
		D3DXVECTOR3 v1, v2;
		v1 =  * pPrymityw.WezPunkt(pEdges[2*i+0]);
		v2 =  * pPrymityw.WezPunkt(pEdges[2*i+1]);

        D3DXVECTOR3 v3 = v1 - vLight*10;
        D3DXVECTOR3 v4 = v2 - vLight*10;

        // Add a quad (two triangles) to the vertex list
        m_pVertices[m_dwNumVertices++] = v1;
        m_pVertices[m_dwNumVertices++] = v2;
        m_pVertices[m_dwNumVertices++] = v3;

        m_pVertices[m_dwNumVertices++] = v2;
        m_pVertices[m_dwNumVertices++] = v4;
        m_pVertices[m_dwNumVertices++] = v3;
    }

    // Delete the temporary edge list
//    delete[] pEdges;

    // Unlock the geometry buffers
    //pMesh->UnlockVertexBuffer();
    //pMesh->UnlockIndexBuffer();

    return S_OK;
}
