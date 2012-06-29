// #####################################################
// FVF.h - Direct3D structs
// #####################################################

#pragma once
#ifndef _MONAD_FVF_H_
#define _MONAD_FVF_H_

#include <D3D11.h>
#include <XNAMath.h>
#include "MonadTemplates.h"
#include "MonadPtr.h"

struct VERTEXPLAIN;

struct VERTEXTEX sealed
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	FLOAT    tu, tv;

	VERTEXTEX();
	VERTEXTEX( const VERTEXPLAIN & Vert );
	VERTEXTEX( const VERTEXTEX & Vert );
	VERTEXTEX & VERTEXTEX::operator=( const VERTEXPLAIN & Vert );

	const static D3DVERTEXELEMENT9 Decl[4];
	static D3D11_INPUT_ELEMENT_DESC Decl11[3];
};

/*struct VERTEXVOL sealed
{
	XMFLOAT3 Position;
	FLOAT    tu, tv;

	const static D3DVERTEXELEMENT9 Decl[4];
	static D3D11_INPUT_ELEMENT_DESC Decl11[3];
};*/

struct VERTEXPLAIN sealed
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;

	VERTEXPLAIN();
	VERTEXPLAIN( const VERTEXPLAIN & Vert );
	VERTEXPLAIN( const VERTEXTEX & Vert );
	VERTEXPLAIN & VERTEXPLAIN::operator=( const VERTEXTEX & Vert );

	const static D3DVERTEXELEMENT9 Decl[3];
	static D3D11_INPUT_ELEMENT_DESC Decl11[2];
};

struct structDeclarations
{
	typedef CMonadIPtrBasic<IDirect3DVertexDeclaration9> tDecl;
	struct CDecl
	{
		CDecl( D3D11_INPUT_ELEMENT_DESC * pNewDecl, const UINT pDecl11Size, char * szShadowTechnique );
		tDecl DeclD3D9;
		D3D11_INPUT_ELEMENT_DESC * DeclD3D11;
		UINT Decl11Size;
		char * m_szShadowTechnique;
	};

	structDeclarations();
	void RegisterDecl( const D3DVERTEXELEMENT9 pElem [], CDecl & pDecl );

	CDecl	m_pPlainDecl;
	// CDecl   m_pVolDecl;
	CDecl	m_pTexDecl;
};

#endif