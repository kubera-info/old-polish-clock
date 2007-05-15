#include "stdafx.h"
#include "ogolne.h"

HRESULT CFaktura::OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{	
	FUNC_TRACE;
	HRESULT hr;

	if(NULL != m_tekstura)
		return S_OK;

	class ResourceCrypt DecryptedTexture(m_IDB);

	V( D3DXCreateTextureFromFileInMemoryEx(
		pd3dDevice,
		DecryptedTexture.GetBuffer(),
		DecryptedTexture.GetBufferSize(),
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		m_format,
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_DEFAULT,
		0,
		NULL,
		NULL,
		&m_tekstura) );
	return hr;
}

void CFaktura::OnDestroyDevice()
{
	SAFE_RELEASE(m_tekstura);
}

HRESULT CMesh::OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
	HRESULT hr;
	LPD3DXBUFFER Dummy1 = NULL, Dummy2 = NULL, Dummy3 = NULL;
	DWORD NumMaterials;

	if(NULL != m_model)
		return S_OK;

	class ResourceCrypt DecryptedMesh(m_ID);

	V ( D3DXLoadMeshFromXInMemory(
		DecryptedMesh.GetBuffer(),
		DecryptedMesh.GetBufferSize(),
		D3DXMESH_32BIT | D3DXMESH_IB_MANAGED | D3DXMESH_VB_MANAGED /*| m_flags*/,
		pd3dDevice,
		&Dummy1,
		&Dummy2,
		&Dummy3,
		&NumMaterials,
		&m_model) );
	SAFE_RELEASE(Dummy1);
	SAFE_RELEASE(Dummy2);
	SAFE_RELEASE(Dummy3);

	return hr;
}


// ##############################################
// Nazwa: ZaladujZasob
// Opis: Funkcja ³aduje dowolny zasób do pamiêci.
// ##############################################
LPCVOID ZaladujZasob(CONST INT pResID, DWORD & pLen)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	HRSRC hRsrc = FindResource(
		hInst,
		MAKEINTRESOURCE(pResID),
		RT_RCDATA);
	if(NULL == hRsrc)
		return NULL;
	DWORD len = SizeofResource(hInst, hRsrc);
	if(len == 0)
		return NULL;
	LPVOID lpLoad = LoadResource(hInst, hRsrc);
	if(NULL == lpLoad)
		return NULL;
	pLen = len;
	return LockResource(lpLoad);
}