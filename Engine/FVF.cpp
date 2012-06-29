#include "DXUT.h"
#include "FVF.h"
#include "DeviceLevel.h"
#include "MonadException.h"

VERTEXTEX::VERTEXTEX()
{
}
VERTEXTEX::VERTEXTEX( const VERTEXPLAIN & Vert )
{
	Position = Vert.Position;
	Normal = Vert.Normal;
	tu = 0.0f;
	tv = 0.0f;
}
VERTEXTEX::VERTEXTEX( const VERTEXTEX & Vert )
{
	Position = Vert.Position;
	Normal = Vert.Normal;
	tu = Vert.tu;
	tv = Vert.tv;
}
VERTEXTEX & VERTEXTEX::operator=( const VERTEXPLAIN & Vert )
{
	Position = Vert.Position;
	Normal = Vert.Normal;
	tu = 0.0f;
	tv = 0.0f;

	return *this;
}

VERTEXPLAIN::VERTEXPLAIN()
{
}
VERTEXPLAIN::VERTEXPLAIN( const VERTEXPLAIN & Vert )
{
	Position = Vert.Position;
	Normal = Vert.Normal;
}
VERTEXPLAIN::VERTEXPLAIN( const VERTEXTEX & Vert )
{
	Position = Vert.Position;
	Normal = Vert.Normal;
}
VERTEXPLAIN & VERTEXPLAIN::operator=( const VERTEXTEX & Vert )
{
	Position = Vert.Position;
	Normal = Vert.Normal;

	return *this;
}

char * TechPlain = "GenHardwareShadowMap";
// char * TechVol = "GenHardwareShadowMapVol";
char * TechTex = "GenHardwareShadowMapTex";

const D3DVERTEXELEMENT9 VERTEXTEX::Decl[4] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END()
};

/*const D3DVERTEXELEMENT9 VERTEXVOL::Decl[4] =
{
{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
D3DDECL_END()
};*/

const D3DVERTEXELEMENT9 VERTEXPLAIN::Decl[3] =
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
	D3DDECL_END()
};

D3D11_INPUT_ELEMENT_DESC VERTEXPLAIN::Decl11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/*D3D11_INPUT_ELEMENT_DESC VERTEXVOL::Decl11[] =
{
{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};*/

D3D11_INPUT_ELEMENT_DESC VERTEXTEX::Decl11[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

structDeclarations::structDeclarations()
:
m_pPlainDecl( VERTEXPLAIN::Decl11, ARRAYSIZE( VERTEXPLAIN::Decl11 ), TechPlain ),
// m_pVolDecl( VERTEXVOL::Decl11, ARRAYSIZE( VERTEXVOL::Decl11 ), TechVol ),
m_pTexDecl( VERTEXTEX::Decl11, ARRAYSIZE( VERTEXTEX::Decl11 ), TechTex )
{
	if( DXUT_D3D9_DEVICE == DeviceLevel )
	{
		RegisterDecl( VERTEXPLAIN::Decl, m_pPlainDecl );
		// RegisterDecl( VERTEXVOL::Decl, m_pVolDecl );
		RegisterDecl( VERTEXTEX::Decl, m_pTexDecl );
	}
}
void structDeclarations::RegisterDecl( const D3DVERTEXELEMENT9 pElem [], CDecl & pDecl )
{
	IDirect3DVertexDeclaration9 * tmpDecl( NULL );
	THROW_EXC_D3D( DXUTGetD3D9Device()->CreateVertexDeclaration( pElem, &tmpDecl ), L"Declarator" );
	pDecl.DeclD3D9.resetRelease( tmpDecl );
}

structDeclarations::CDecl::CDecl( D3D11_INPUT_ELEMENT_DESC * pNewDecl, const UINT pDecl11Size, char * szShadowTechnique )
: DeclD3D11( pNewDecl ), Decl11Size( pDecl11Size ), m_szShadowTechnique( szShadowTechnique )
{
}

