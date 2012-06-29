#pragma once
#ifndef _METALLIC_H_
#define _METALLIC_H_

#include "MonadMath.h"
#include "OPC-Shader.h"

struct StructMetallic
{
	XMFLOAT4 BrightColor;
	XMFLOAT4 DarkColor;
	XMFLOAT4 FresnelColor;
};

extern struct StructMetallic const g_Ornament;
extern struct StructMetallic const g_Oval;
extern struct StructMetallic const g_String;
extern struct StructMetallic const g_Join;
extern struct StructMetallic const g_NailFace;
extern struct StructMetallic const g_NailBorder;
extern struct StructMetallic const g_NailWood;
extern struct StructMetallic const g_Weight;

class CGenericMetallic abstract : public CPointAndAmbientModelGeneric
{
public:
	CGenericMetallic(
		const char * pTechnique,
		const std::wstring & pMeshFilename, 
		const structDeclarations::CDecl & pFVF,
		const char * pShadowTechnique = NULL );
};

class CMetallic : public CGenericMetallic
{
public:
	CMetallic(
		const std::wstring & pMeshFilename,
		const StructMetallic & pMetallic = g_Ornament,
		const char * pShadowTechnique = NULL );
	virtual void OnUpdateDX9PerFrame();
	virtual void OnUpdateDX11PerFrame();

protected:
	typedef CCBuffer<StructMetallic, CB_METAL, MONAD_USAGE_PS> CBufferMetallic;
	CBufferMetallic m_CBufferMetallic;
};

class CMetallicTex : public CGenericMetallic
{
public:
	CMetallicTex(
		const std::wstring & pTextureFilename,
		const std::wstring & pMeshFilename );
	virtual void OnUpdateDX9PerFrame();
	virtual void OnUpdateDX11PerFrame();

protected:
	class CManagedTexture MetalTexture;
};

#endif // _METALLIC_H_