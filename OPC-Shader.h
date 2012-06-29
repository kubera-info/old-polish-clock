#pragma once
#ifndef _EDU_SHADER_H_
#define _EDU_SHADER_H_

#include "MonadShader.h"
#include "OPC-Shader.h"
#include "OPC-Struct.h"
#include "Skin.h"

extern const float fMaxByte;

struct StructWood
{
	XMFLOAT4 g_WoodBright;
	XMFLOAT4 g_WoodDark;
};
extern struct StructWood const g_Wood;
extern struct StructWood const g_GlossyWood;
extern struct StructWood const g_DarkWood;
extern struct StructWood const g_Wood3;
extern struct StructWood const g_Duplex;

struct StructPointView
{
	XMFLOAT4 lightPosition;
};
typedef CCBuffer<StructPointView, CB_LIGHT_POS, MONAD_USAGE_BOTH> CBufferV;

class CPointLightShader : public CMonadShaderGeneric
{
public:
	CPointLightShader(
		const std::wstring & pFilename, 
		const char * pTechnique, 
		const std::wstring & pMeshFilename, 
		const structDeclarations::CDecl & pFVF,
		const char * pShadowTechnique = NULL );
	virtual void OnComputePerFrame( const XMMATRIX * pWorld );
	virtual void OnUpdateDX11PerFrame();
	virtual void OnUpdateDX9PerFrame();
	static CSealedSingleCall & ViewOnce() throw()
	{
		static CSealedSingleCall mLightOnce;

		return mLightOnce;
	}
};

class CPointAndAmbientModelGeneric abstract : public CPointLightShader
{
public:
	CPointAndAmbientModelGeneric(
		const std::wstring & pFilename,
		const char * pTechnique,
		const std::wstring & pMeshFilename,
		const structDeclarations::CDecl & pFVF,
		const char * pShadowTechnique = NULL
		);
	virtual void OnComputePerFrame( const XMMATRIX * pWorld );
	virtual void OnUpdateDX9PerFrame();
	virtual void OnUpdateDX11PerFrame();

protected:
	XMFLOAT4X4 TexTransform0;
	XMFLOAT4X4 TexTransform1;
	XMFLOAT4X4 TexTransform2;
	struct StructModel
	{
		XMFLOAT4X4 g_mModelViewProj;
		XMFLOAT4X4 g_mModel;
		XMFLOAT4X4 TexTransform0;
		XMFLOAT4X4 TexTransform1;
		XMFLOAT4X4 TexTransform2;
	};
	typedef CCBuffer<StructModel, CB_MODEL, MONAD_USAGE_VS> CBufferModel;
	CBufferModel m_CBufferModel;
};

class CMaterialShader abstract : public CPointAndAmbientModelGeneric
{
public:
	CMaterialShader(
		const char * pTechnique,
		const std::wstring & pMaterialFilename,
		const std::wstring & pMeshFilename,
		const char * pShadowTechnique = NULL
		);
};

class CGlassShader : public CPointAndAmbientModelGeneric
{
public:
	CGlassShader(
		const std::wstring & pMeshFilename
		);
};

class CAmbientGlassShader : public CPointAndAmbientModelGeneric
{
public:
	CAmbientGlassShader(
		const std::wstring & pMeshFilename
		);
};

class CHandShader : public CMaterialShader
{
public:
	CHandShader(
		const std::wstring & pMeshFilename
		);
};

class CTexturedShader abstract : public CPointAndAmbientModelGeneric
{
public:
	CTexturedShader(
		const char * pTechnique,
		const std::wstring & pTextureFilename,
		const std::wstring & pMeshFilename,
		const char * pShadowTechnique = NULL
		);
	virtual void OnUpdateDX9PerFrame();
	virtual void OnUpdateDX11PerFrame();

protected:
	class CManagedTexture mSingleTexture;
};

class CWoodenShader : public CTexturedShader
{
public:
	CWoodenShader(
		const std::wstring & pMeshFilename,
		const std::wstring & pTextureFilename = L"wood",
		const struct StructWood & pWood = g_Wood,
		const char * pShadowTechnique = NULL );

	virtual void OnUpdateDX9PerFrame();
	virtual void OnUpdateDX11PerFrame();

protected:
	typedef CCBuffer<StructWood, CB_WOOD, MONAD_USAGE_PS> CBufferWood;
	CBufferWood m_CBufferWood;
};
#endif
