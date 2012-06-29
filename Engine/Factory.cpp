#include "DXUT.h"
#include "SDKMesh.h"
#include "MonadException.h"
#include "MonadAudio.h"
#include "Singleton.h"
#include "Factory.h"
#include "MonadAudio.h"
#include "DDSTextureLoader.h"

volatile bool bFactoryIsReady = false;

CIFX::CIFX()
{
	RegisterResetEvent();
	RegisterLostEvent();
}
void CIFX::OnResetDevice()
{
	DXUTTRACE( L"Reset Shader\n" );
	if( get() )
		( static_cast< ID3DXEffect * > ( get() ) ) -> OnResetDevice();
}
void CIFX::OnLostDevice()
{
	DXUTTRACE( L"Lost Shader\n" );
	if( get() )
		( static_cast< ID3DXEffect * > ( get() ) ) -> OnLostDevice();
}
ID3DXEffect * CIFX::GetFX9()
{
	return static_cast< ID3DXEffect * > ( get() );
}
ID3DBlob * CIFX::GetFX11()
{
	return static_cast< ID3DBlob * > ( get() );
}
void CRoboTexture::OnCreateDevice( CFileHolder & pFileHolder )
{
	if( DXUT_D3D9_DEVICE == DeviceLevel )
	{
		LPDIRECT3DTEXTURE9 pNewTex( NULL );
		assert( NULL != DXUTGetD3D9Device() );
		assert( 0 < pFileHolder.GetSize() );
		THROW_EXC_D3D(
			D3DXCreateTextureFromFileInMemoryEx(
			DXUTGetD3D9Device(), 
			pFileHolder.GetBuffer(),
			(UINT) pFileHolder.GetSize(),
			D3DX_DEFAULT, D3DX_DEFAULT,
			0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
			D3DX_DEFAULT, D3DX_DEFAULT, 0,
			NULL, NULL, &pNewTex ), 
			L"D3DXCreateTextureFromFileEx" );
		mMember.resetRelease( pNewTex );
	}
	else
	{
		// Create texture from file
		ID3D11ShaderResourceView * pNewTex( NULL );
		THROW_EXC_D3D( 
			CreateDDSTextureFromFile11( 
			DXUTGetD3D11Device(), 
			pFileHolder.GetBuffer(), 
			pFileHolder.GetSize(), 
			&pNewTex
			), L"CreTex11" );
		mMember.resetRelease( pNewTex );
		pFileHolder.DetachFileHolder();
	}
}

void CRoboMesh::OnCreateDevice( CFileHolder & pFileHolder )
{
	CDXUTSDKMesh * mesh = new CDXUTSDKMesh();

	if( DXUT_D3D9_DEVICE == DeviceLevel )
	{
		THROW_EXC_D3D( mesh->Create( DXUTGetD3D9Device(), pFileHolder.GetBuffer(), pFileHolder.GetSize() ), L"Mesh" );
	}
	else
	{
		THROW_EXC_D3D( mesh->Create( DXUTGetD3D11Device(), pFileHolder.GetBuffer(), pFileHolder.GetSize() ), L"Mesh11" );
	}
	pFileHolder.DetachFileHolder();

	mMember.Assign( mesh );
}

void CRoboShader::OnCreateDevice( CFileHolder & pFileHolder )
{
	if( DXUT_D3D9_DEVICE == DeviceLevel )
	{
		// HRESULT hr;
		DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXFX_LARGEADDRESSAWARE;

#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
		dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION|D3DXSHADER_DEBUG;
#endif
#ifdef DEBUG_PS
		dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION|D3DXSHADER_DEBUG;
#endif
		//CComPtr<ID3DXBuffer> mErrBuffer;

		LPD3DXEFFECT tmpEffect( NULL );
		THROW_EXC_D3D( D3DXCreateEffect(
			DXUTGetD3D9Device(),
			(const char *) pFileHolder.GetBuffer(),
			pFileHolder.GetSize(),
			NULL,
			NULL,
			dwShaderFlags,
			NULL,
			&tmpEffect,
			NULL
			//&mErrBuffer
			), "FX" );
		mMember.resetRelease( tmpEffect );

		/*#ifdef DEBUG
		if( FAILED( hr ) )
		{
		MessageBoxA( 0, (char * ) mErrBuffer->GetBufferPointer(), "Shader Error", 0 );
		THROW_EXC_D3D( hr, L"Create FX" );
		}
		#endif;*/
	}
	else
	{
		ID3DBlob * tmpBlob( NULL );
		THROW_EXC_D3D( D3DCreateBlob( pFileHolder.GetSize(), &tmpBlob ), "FXBlob" );
		mMember.resetRelease( tmpBlob );
		memcpy( mMember.GetFX11()->GetBufferPointer(), pFileHolder.GetBuffer(), pFileHolder.GetSize() );
	}
}

CMonadFactory::CMonadFactory() // Because of the rights
{
}

CMonadFactory::~CMonadFactory()
{
	CRoboMesh::GetMembers().clear();
	CRoboTexture::GetMembers().clear();
	CRoboShader::GetMembers().clear();
}

void CMonadFactory::OnCreateResource( CFileHolder & pFileHolder )
{
	if( pFileHolder.GetExtension() == szExtMesh )
	{
		CRoboMesh RoboMesh;
		RoboMesh.OnCreateResource( pFileHolder );
	}
	else if( pFileHolder.GetExtension() == szExtTex /* || pFileHolder.GetExtension() == szExtVolTex */ )
	{
		CRoboTexture RoboTexture;
		RoboTexture.OnCreateResource( pFileHolder );
	}
	else if( pFileHolder.GetExtension() == szExtFx )
	{
		CRoboShader RoboShader;
		RoboShader.OnCreateResource( pFileHolder );
	}
}

const CCue & CMonadFactory::GetCue( const std::wstring & pFilename ) const
{
	CMonadFactory::CCueMap::const_iterator it = GetMusic().find( pFilename );
	if( GetMusic().end() == it )
		THROW_EXC( MonadException::FileNotFound, E_FAIL, L"Error while getting Robo Member");
	return it->second;
}
IUnknown * CMonadFactory::GetTexture( const std::wstring & pFilename ) const
{
	return CRoboTexture::GetMember( pFilename ).GetInterface();
}
CDXUTSDKMesh * CMonadFactory::GetMesh( const std::wstring & pFilename ) const
{
	return CRoboMesh::GetMember( pFilename ).get();
}
const LPD3DXEFFECT CMonadFactory::GetShader( const std::wstring & pFilename ) const
{
	return CRoboShader::GetMember( pFilename ).GetFX9();
}
ID3DBlob* CMonadFactory::GetShader11( const std::wstring & pFilename ) const
{
	return CRoboShader::GetMember( pFilename ).GetFX11();
}
const CMonadShaderConfig & CMonadFactory::GetFXConfig( const std::wstring & pFilename ) const
{
	CMonadFactory::CFXMap::const_iterator it = mConfigs.find( pFilename );
	if( mConfigs.end() == it )
		THROW_EXC( MonadException::FileNotFound, E_FAIL, L"Error while getting Robo Member");
	return it->second;
}
CMonadFactory::CCueMap & CMonadFactory::GetMusic()
{
	static CCueMap mVectorMap;
	return mVectorMap;
}
void CMonadFactory::SetCue( const std::wstring & pFilename, const CCue & pCue )
{
	GetMusic().insert( std::make_pair( pFilename, pCue ) );
}
void CMonadFactory::SetFXConfig( const std::wstring & pFilename, const CMonadShaderConfig & pConfig )
{
	mConfigs.insert( std::make_pair( pFilename, pConfig ) );
}