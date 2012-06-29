#include "DXUT.h"
#include "Metallic.h"
#include "Singleton.h"

extern struct StructMetallic const g_Ornament;
extern struct StructMetallic const g_String;
extern struct StructMetallic const g_Join;
extern struct StructMetallic const g_Nail;
extern struct StructMetallic const g_Weight;

static const XMFLOAT4 sBrick( 40.0f / fMaxByte, 35.0f / fMaxByte, 9.0f / fMaxByte, 1.0f );
/*const struct StructMetallic g_Ornament =
{
	XMFLOAT4( 254.0f / fMaxByte, 252.0f / fMaxByte, 254.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 30.0f / fMaxByte, 16.0f / fMaxByte, 2.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 80.0f / fMaxByte, 65.0f / fMaxByte, 15.0f / fMaxByte, 1.0f )
};*/
const struct StructMetallic g_Ornament =
{
	XMFLOAT4( 255.0f / fMaxByte, 254.0f / fMaxByte, 222.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 42.0f / fMaxByte, 26.0f / fMaxByte, 18.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 120.0f / fMaxByte, 95.0f / fMaxByte, 22.0f / fMaxByte, 1.0f )
};
const struct StructMetallic g_Oval =
{
	XMFLOAT4( 255.0f / fMaxByte, 254.0f / fMaxByte, 222.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 42.0f / fMaxByte, 26.0f / fMaxByte, 18.0f / fMaxByte, 1.0f ),
	XMFLOAT4( sBrick )
};
const struct StructMetallic g_String =
{
	XMFLOAT4( 255.0f / fMaxByte, 252.0f / fMaxByte, 253.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 55.0f / fMaxByte, 40.0f / fMaxByte, 38.0f / fMaxByte, 1.0f ),
	XMFLOAT4( sBrick )
};
const struct StructMetallic g_Join =
{
	XMFLOAT4( 254.0f / fMaxByte, 252.0f / fMaxByte, 254.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 46.0f / fMaxByte, 21.0f / fMaxByte, 4.0f / fMaxByte, 1.0f ),
	XMFLOAT4( sBrick )
};
const struct StructMetallic g_NailFace =
{
	XMFLOAT4( 254.0f / fMaxByte, 254.0f / fMaxByte, 255.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 80.0f / fMaxByte, 80.0f / fMaxByte, 83.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 200.0f / fMaxByte, 200.0f / fMaxByte, 200.0f / fMaxByte, 1.0f )
};
const struct StructMetallic g_NailBorder =
{
	XMFLOAT4( 254.0f / fMaxByte, 254.0f / fMaxByte, 255.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 50.0f / fMaxByte, 50.0f / fMaxByte, 51.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 0.1f, 0.1f, 0.1f, 1.0f )
};
const struct StructMetallic g_NailWood =
{
	XMFLOAT4( 254.0f / fMaxByte, 254.0f / fMaxByte, 255.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 50.0f / fMaxByte, 50.0f / fMaxByte, 51.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 111.0f / fMaxByte, 54.0f / fMaxByte, 27.0f / fMaxByte, 1.0f )
};
const struct StructMetallic g_Weight =
{
	XMFLOAT4( 252.0f / fMaxByte, 252.0f / fMaxByte, 255.0f / fMaxByte, 1.0f ),
	XMFLOAT4( 0.0f / fMaxByte, 0.0f / fMaxByte, 0.0f / fMaxByte, 1.0f ),
	XMFLOAT4( sBrick )
};

CGenericMetallic::CGenericMetallic(
								   const char * pTechnique,
								   const std::wstring & pMeshFilename,
								   const structDeclarations::CDecl & pFVF,
								   const char * pShadowTechnique )
								   : CPointAndAmbientModelGeneric(
								   L"light", 
								   pTechnique, 
								   pMeshFilename,
								   pFVF )
{
}

///////////////////////////////////////////////////////////////////////////////

CMetallic::CMetallic(
					 const std::wstring & pMeshFilename,
					 const StructMetallic & pMetallic,
					 const char * pShadowTechnique )
					 :
CGenericMetallic( 
				 "TGoldenMetallic", 
				 pMeshFilename, 
				 Singleton->Declarators.m_pPlainDecl,
				 pShadowTechnique )
{
	m_CBufferMetallic.GetVal() = pMetallic;
}
void CMetallic::OnUpdateDX9PerFrame()
{
	CGenericMetallic::OnUpdateDX9PerFrame();

	SetVector( gD3D9Handles.hBrightColor, m_CBufferMetallic.GetVal().BrightColor );
	SetVector( gD3D9Handles.hDarkColor, m_CBufferMetallic.GetVal().DarkColor );
	SetVector( gD3D9Handles.hFresnelColor, m_CBufferMetallic.GetVal().FresnelColor );
}
void CMetallic::OnUpdateDX11PerFrame()
{
	CGenericMetallic::OnUpdateDX11PerFrame();

	m_CBufferMetallic.Export();
}

CMetallicTex::CMetallicTex(
						   const std::wstring & pTextureFilename,
						   const std::wstring & pMeshFilename )
						   :
CGenericMetallic( 
				 "TMetallicTex", 
				 pMeshFilename, 
				 Singleton->Declarators.m_pTexDecl ),
				 MetalTexture( pTextureFilename )
{
}

void CMetallicTex::OnUpdateDX9PerFrame()
{
	CGenericMetallic::OnUpdateDX9PerFrame();

	SetTexture( gD3D9Handles.hTex0, MetalTexture );
}

void CMetallicTex::OnUpdateDX11PerFrame()
{
	CGenericMetallic::OnUpdateDX11PerFrame();

	SetShaderResource( 6, MetalTexture );
}