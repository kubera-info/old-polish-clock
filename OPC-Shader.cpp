#include "DXUT.h"
#include "Singleton.h"
#include "OPC-Shader.h"
#include "Light.h"

const float fMaxByte( 255.0f );

const struct StructWood g_Wood =
{
	XMFLOAT4( 189.0f / fMaxByte, 131.0f / fMaxByte, 79.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 111.0f / fMaxByte, 54.0f / fMaxByte, 27.0f / fMaxByte, 1.0f )
};
const struct StructWood g_GlossyWood =
{
	XMFLOAT4( 60.0f / fMaxByte, 60.0f / fMaxByte, 60.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 10.0f / fMaxByte, 10.0f / fMaxByte, 10.0f / fMaxByte, 1.0f )
};
const struct StructWood g_DarkWood =
{
	XMFLOAT4( 84.0f / fMaxByte, 84.0f / fMaxByte, 84.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 22.0f / fMaxByte, 22.0f / fMaxByte, 22.0f / fMaxByte, 1.0f )
};
const struct StructWood g_Wood3 =
{
	XMFLOAT4( 180.0f / fMaxByte, 136.0f / fMaxByte, 83.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 81.0f / fMaxByte, 55.0f / fMaxByte, 50.0f / fMaxByte, 1.0f )
};

const struct StructWood g_Duplex =
{
	XMFLOAT4( 255.0f / fMaxByte, 255.0f / fMaxByte, 232.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 28.0f / fMaxByte, 53.0f / fMaxByte, 62.0f / fMaxByte, 1.0f )
};

CPointLightShader::CPointLightShader( 
									 const std::wstring & pFilename, 
									 const char * pTechnique, 
									 const std::wstring & pMeshFilename, 
									 const structDeclarations::CDecl & pFVF,
									 const char * pShadowTechnique )
									 : CMonadShaderGeneric( 
									 pFilename, 
									 pTechnique, 
									 pMeshFilename, 
									 pFVF,
									 pShadowTechnique )
{
}
void CPointLightShader::OnComputePerFrame( const XMMATRIX * pWorld )
{
	CMonadShaderGeneric::OnComputePerFrame( pWorld );

	XMStoreFloat4( &Singleton->LightBuffer->GetVal().lightPosition, XMVector4Transform( g_lightPosition, VIEW_MATRIX ) );
}
void CPointLightShader::OnUpdateDX9PerFrame()
{
	if( !ViewOnce() )
		SetVector( gD3D9Handles.hLight1_Position, Singleton->LightBuffer->GetVal().lightPosition );
}

void CPointLightShader::OnUpdateDX11PerFrame()
{
	if( !ViewOnce() )
		Singleton->LightBuffer->Export();
}

CPointAndAmbientModelGeneric::CPointAndAmbientModelGeneric(
	const std::wstring & pFilename,
	const char * pTechnique,
	const std::wstring & pMeshFilename,
	const structDeclarations::CDecl & pFVF,
	const char * pShadowTechnique )
	: CPointLightShader( 
	pFilename, pTechnique, 
	pMeshFilename,
	pFVF,
	pShadowTechnique )
{
}

void CPointAndAmbientModelGeneric::OnComputePerFrame( const XMMATRIX * pWorld )
{
	CPointLightShader::OnComputePerFrame( pWorld );

	WORLDVIEW_MATRIX = XMMatrixMultiply( *pWorld, VIEW_MATRIX );
	WORLDVIEWPROJECTION_MATRIX = XMMatrixMultiply( WORLDVIEW_MATRIX, PROJECTION_MATRIX );
	XMStoreFloat4x4( &TexTransform0, XMMatrixMultiply( *pWorld, SHADOW_TEX_MATRIX ) );
	if( DXUT_D3D11_DEVICE == DeviceLevel )
	{
		XMStoreFloat4x4( &TexTransform1, XMMatrixMultiply( *pWorld, SHADOW_TEX_MATRIX1 ) );
		XMStoreFloat4x4( &TexTransform2, XMMatrixMultiply( *pWorld, SHADOW_TEX_MATRIX2 ) );
	}
}

void CPointAndAmbientModelGeneric::OnUpdateDX9PerFrame()
{
	CPointLightShader::OnUpdateDX9PerFrame();

	SetMatrix( gD3D9Handles.hg_mModel, WORLDVIEW_MATRIX );
	SetMatrix( gD3D9Handles.hg_mModelViewProj, WORLDVIEWPROJECTION_MATRIX );
	SetMatrix( gD3D9Handles.hTexTransform, XMMatrixMultiply( XMLoadFloat4x4( &TexTransform0 ), texScaleBiasMat ) );
}

void CPointAndAmbientModelGeneric::OnUpdateDX11PerFrame()
{
	CPointLightShader::OnUpdateDX11PerFrame();

	XMStoreFloat4x4( &m_CBufferModel.GetVal().g_mModel, XMMatrixTranspose( WORLDVIEW_MATRIX ) );
	XMStoreFloat4x4( &m_CBufferModel.GetVal().g_mModelViewProj, XMMatrixTranspose( WORLDVIEWPROJECTION_MATRIX ) );
	XMStoreFloat4x4( &m_CBufferModel.GetVal().TexTransform0, XMMatrixTranspose( XMLoadFloat4x4( &TexTransform0 ) ) );
	XMStoreFloat4x4( &m_CBufferModel.GetVal().TexTransform1, XMMatrixTranspose( XMLoadFloat4x4( &TexTransform1 ) ) );
	XMStoreFloat4x4( &m_CBufferModel.GetVal().TexTransform2, XMMatrixTranspose( XMLoadFloat4x4( &TexTransform2 ) ) );
	m_CBufferModel.Export();
}

CMaterialShader::CMaterialShader(
								 const char * pTechnique,
								 const std::wstring & pMaterialFilename,
								 const std::wstring & pMeshFilename,
								 const char * pShadowTechnique
								 )
								 : CPointAndAmbientModelGeneric(
								 L"light",
								 pTechnique, 
								 pMeshFilename,
								 Singleton->Declarators.m_pPlainDecl,
								 pShadowTechnique )
{
}

CGlassShader::CGlassShader(
						   const std::wstring & pMeshFilename
						   )
						   : CPointAndAmbientModelGeneric(
						   L"light",
						   "TGlass", 
						   pMeshFilename,
						   Singleton->Declarators.m_pPlainDecl,
						   "ghost" )
{
}

CAmbientGlassShader::CAmbientGlassShader(
	const std::wstring & pMeshFilename
	)
	: CPointAndAmbientModelGeneric(
	L"light",
	"TAmbientGlass", 
	pMeshFilename,
	Singleton->Declarators.m_pPlainDecl,
	"ghost" )
{
}

CHandShader::CHandShader(
						 const std::wstring & pMeshFilename )
						 : CMaterialShader( "THand", L"hand", pMeshFilename )
{
}

CTexturedShader::CTexturedShader(
	const char * pTechnique,
	const std::wstring & pTextureFilename,
	const std::wstring & pMeshFilename,
	const char * pShadowTechnique )
	: CPointAndAmbientModelGeneric(
	L"light",
	pTechnique,
	pMeshFilename,
	Singleton->Declarators.m_pTexDecl,
	pShadowTechnique ),
	mSingleTexture( pTextureFilename )
{
}

void CTexturedShader::OnUpdateDX9PerFrame()
{
	CPointAndAmbientModelGeneric::OnUpdateDX9PerFrame();

	SetTexture( gD3D9Handles.hTex0, mSingleTexture );
}

void CTexturedShader::OnUpdateDX11PerFrame()
{
	CPointAndAmbientModelGeneric::OnUpdateDX11PerFrame();

	SetShaderResource( 6, mSingleTexture );
}

CWoodenShader::CWoodenShader(
							 const std::wstring & pMeshFilename,
							 const std::wstring & pTextureFilename,
							 const struct StructWood & pWood,
							 const char * pShadowTechnique ) :
CTexturedShader( "TWood", pTextureFilename, pMeshFilename, pShadowTechnique )
{
	m_CBufferWood.GetVal() = pWood;
}

void CWoodenShader::OnUpdateDX9PerFrame()
{
	CTexturedShader::OnUpdateDX9PerFrame();

	SetVector( gD3D9Handles.hg_WoodBright, m_CBufferWood.GetVal().g_WoodBright );
	SetVector( gD3D9Handles.hg_WoodDark, m_CBufferWood.GetVal().g_WoodDark );
}

void CWoodenShader::OnUpdateDX11PerFrame()
{
	CTexturedShader::OnUpdateDX11PerFrame();

	m_CBufferWood.Export();
}