//--------------------------------------------------------------------------------------
// File: DDSTextureLoader.h
//
// Functions for loading a DDS texture without using D3DX
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
// Altered: W. Kubera

#include <d3d9.h>
#include <d3d11.h>

//HRESULT CreateDDSTextureFromFile9( LPDIRECT3DDEVICE9 pDev, BYTE* pBuffer, const DWORD pSize, LPDIRECT3DTEXTURE9* ppTex, DWORD * dwWidth, DWORD * dwHeight );
HRESULT CreateDDSTextureFromFile11( ID3D11Device* pDev, BYTE* pBuffer, const DWORD pSize, ID3D11ShaderResourceView** ppSRV );