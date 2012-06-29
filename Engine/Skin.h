#pragma once
#ifndef _MINI_INFO_SKIN_H_
#define _MINI_INFO_SKIN_H_

#include "DXUT.h"
#include "CProto.h"
#include "MonadTemplates.h"

// #####################################
// Nazwa: class CMaterial
// Opis: Ustawienie mataria³u dla modeli
// #####################################

class CAbstractTexture : public CDestructable
{
public:
	virtual operator LPDIRECT3DBASETEXTURE9 () = 0;
	virtual operator ID3D11ShaderResourceView** () = 0;
};

class CVirtualTexture;

class CManagedTexture : public CAbstractTexture
{
public:
	friend CVirtualTexture;
	CManagedTexture( const std::wstring & pFilename );
	virtual operator LPDIRECT3DBASETEXTURE9 ();
	virtual operator ID3D11ShaderResourceView** ();

protected:
	IUnknown * iTexture;
};

class CVirtualTexture : public CAbstractTexture
{
public:
	void AddTexture( CManagedTexture & pTexture );
	virtual operator LPDIRECT3DBASETEXTURE9 ();
	virtual operator ID3D11ShaderResourceView** ();

protected:
	virtual size_t GetSelectedTextureIndex() const = 0;
	CManagedTexture & GetTexture();

	typedef struct CMonadCoherentVector<CManagedTexture *> CTextures;
	
	CTextures mTextures;
};

#endif