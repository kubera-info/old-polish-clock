#include "DXUT.h"
#include "Skin.h"
#include "Singleton.h"

CManagedTexture::CManagedTexture( const std::wstring & pFilename )
{
	iTexture = Singleton->MonadFactory.GetTexture( pFilename );
}

CManagedTexture::operator LPDIRECT3DBASETEXTURE9 ()
{
	return reinterpret_cast< LPDIRECT3DBASETEXTURE9 > ( iTexture );
}
CManagedTexture::operator ID3D11ShaderResourceView** ()
{
	return reinterpret_cast< ID3D11ShaderResourceView** > ( &iTexture );
}


void CVirtualTexture::AddTexture( CManagedTexture & pTexture )
{
	mTextures.push_back( &pTexture );
}

CManagedTexture & CVirtualTexture::GetTexture()
{
	return *mTextures[ GetSelectedTextureIndex() ];
}

CVirtualTexture::operator LPDIRECT3DBASETEXTURE9 ()
{
	return GetTexture();
}
CVirtualTexture::operator ID3D11ShaderResourceView** ()
{
	return GetTexture();
}
