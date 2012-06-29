// ###################################
// ClockFace.h - pakiet tarczy zegarowej.
// ###################################

#pragma once
#ifndef _CLOCK_FACE_H_
#define _CLOCK_FACE_H_

#include "OPC-Shader.h"
#include "Metallic.h"
#include "ClockTime.h"
#include "Manager.h"

struct CAnimateChange : public CTimeLerp
{
	CAnimateChange();

	bool IsNormal() const;
	bool IsAnimated() const;

	void Alter( const float pBPM );

	size_t GetTextureID() const;
	size_t GetPreviousTextureID() const;

	static CAnimateChange & GetAnimateChange()
	{
		static struct CAnimateChange mAnimate;

		return mAnimate;
	}

protected:
	size_t iTextureID;
	size_t iPrevTextureID;
};

class CTarcza : public CPointAndAmbientModelGeneric
{
public:
	CTarcza(
		const char * pTechnique = "TSpecularTex" );
	virtual void OnUpdateDX9PerFrame();
	virtual void OnUpdateDX11PerFrame();
	virtual bool IsEnabled();

protected:
	struct CVirtualFaceTexture : public CVirtualTexture
	{
		CVirtualFaceTexture() :
	textureFace1( L"clockface01" ),
		textureFace2( L"clockface02" ),
		textureFace3( L"clockface03" ),
		textureFace4( L"clockface04" )
	{
		AddTexture( textureFace1 );
		AddTexture( textureFace2 );
		AddTexture( textureFace3 );
		AddTexture( textureFace4 );
	}
	virtual size_t GetSelectedTextureIndex() const;

	protected:
		class CManagedTexture textureFace1, textureFace2, textureFace3, textureFace4;
	} textureFacesPack;
};

class CTarcza2 : public CTarcza
{
public:
	CTarcza2();

	virtual void OnComputePerFrame( const XMMATRIX * pWorld );
	virtual void OnUpdateDX9PerFrame();
	virtual void OnUpdateDX11PerFrame();
	virtual bool IsEnabled();

protected:
	struct StructFactor
	{
		XMFLOAT4 fLerp;
	};
	typedef CCBuffer<StructFactor, CB_LERP, MONAD_USAGE_PS> CBufferFactor;
	CBufferFactor m_CBufferFactor;
	struct CVirtualSecondaryFaceTexture : public CVirtualFaceTexture
	{
		virtual size_t GetSelectedTextureIndex() const;
	} textureSecondaryPack;
};

class CMetallicOrnament : public CMetallic
{
public:
	CMetallicOrnament();
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );
};

class CWeight : public CMetallic
{
public:
	CWeight();
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );

protected:
	XMFLOAT4X4 m_MyPos;
};

class CClockString : public CMetallic
{
public:
	CClockString();
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );

protected:
	XMFLOAT4X4 m_MyPos;
};

class CClockStringL : public CMetallic
{
public:
	CClockStringL( float fSide );
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );

protected:
	XMFLOAT4X4 m_MyPos;
};

class CTorus : public CMetallic
{
public:
	CTorus();
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );
};

class CPicture : public CTexturedShader
{
public:
	CPicture() : 
	  CTexturedShader( "TDiffuseTex", L"miss", L"miss" )
	  {
	  }
};

class CWeightAndString : public CWorldGrCombine
{
public:
	CWeightAndString( float fSide );
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );

protected:
	CWeight mWeight;
	CClockString mString;
	CTorus mTorus;
	XMFLOAT4X4 m_MyPos;
};
#endif