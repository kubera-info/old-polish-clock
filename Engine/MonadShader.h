#pragma once

#ifndef _MONAD_SHADER_H_
#define _MONAD_SHADER_H_

#include "DXUT.h"
#include <boost/utility.hpp>
#include "SDKMesh.h"
#include "MonadMath.h"
#include "MonadEvents.h"
#include "Manager.h"
#include "Skin.h"
#include "Event.h"

#define CB_SHADOW ( 0 )

enum MONAD_SHADOW_MODE
{
	MONAD_WORLD,
	MONAD_STATIC_SHADOW,
	MONAD_DYNAMIC_SHADOW
};

enum MONAD_SHADER_USAGE
{
	MONAD_USAGE_VS = 1,
	MONAD_USAGE_PS = 2,
	MONAD_USAGE_BOTH = 3
};

struct CMonadShaderConfig sealed
{
	CMonadShaderConfig( const std::wstring & pVS, const std::wstring pPS )
		: mVS( pVS ), mPS( pPS )
	{
	}
	inline const std::wstring & GetVS() const throw() { return mVS; }
	inline const std::wstring & GetPS() const throw() { return mPS; }

private:
	std::wstring mVS;
	std::wstring mPS;
};

void ClearTextures();
struct CShadowHelper : boost::noncopyable, MonadEvent::COnEvent
{
	CShadowHelper();

	static const int TEXDEPTH_HEIGHT;
	static const int TEXDEPTH_WIDTH;
	const float m_fDepthBias;
	const float m_fBiasSlope;

	// D3D9
	virtual void OnResetLogicDX9();
	typedef CMonadLostableIPtr<IDirect3DTexture9> CAutoTexture;
	typedef CMonadLostableIPtr<IDirect3DSurface9> CAutoSurface;
	static void SaveD3D9World();
	void BeforeD3D9Rendering();
	static void AfterD3D9Rendering();
	CAutoTexture m_pSMZTexture;
	CAutoSurface m_pSMZSurface;
	static CAutoSurface & GetZBuffer()
	{
		static CAutoSurface m_pZBuffer;
		return m_pZBuffer;
	}
	static CAutoSurface & GetBackBuffer()
	{
		static CAutoSurface m_pBackBuffer;
		return m_pBackBuffer;
	}
	static CAutoTexture & GetColorTexture()
	{
		static CAutoTexture m_pSMColorTexture;
		return m_pSMColorTexture;
	}
	static CAutoSurface & GetColorSurface()
	{
		static CAutoSurface m_pSMColorSurface;
		return m_pSMColorSurface;
	}
	static D3DVIEWPORT9 & GetOldViewport() { static D3DVIEWPORT9 oldViewport; return oldViewport; }
	static const D3DVIEWPORT9 GetDefaultNewViewport();
	static const D3DVIEWPORT9 & GetNewViewport() { static D3DVIEWPORT9 newViewport( GetDefaultNewViewport() ); return newViewport; }


	// D3D11
	static void SaveD3D11World();
	void BeforeD3D11Rendering( ULONG_PTR pShadowLevelId );
	static void AfterD3D11Rendering();
	static const ULONG_PTR s_ulShadowCnt = 3;
	struct COneShadow sealed
	{
		COneShadow(
			const XMFLOAT3 & pEyePtInternal,
			XMMATRIX * pShadowMatrix,
			ID3D11ShaderResourceView * pDepthMapSRV,
			ID3D11DepthStencilView * pDepthMapDSV );
		void BeforeD3D11Rendering();

		CMonadIPtrBasic<ID3D11ShaderResourceView> mDepthMapSRV;
		CMonadIPtrBasic<ID3D11DepthStencilView> mDepthMapDSV;
		XMMATRIX * mMyShadowMatrix;
	};
	typedef std::vector<COneShadow> CShadowLevels;
	CShadowLevels mLevels;
	static D3D11_VIEWPORT & GetOldViewport11() { static D3D11_VIEWPORT oldViewport; return oldViewport; }
	static const D3D11_VIEWPORT GetDefaultNewViewport11();
	static const D3D11_VIEWPORT & GetNewViewport11() { static D3D11_VIEWPORT newViewport( GetDefaultNewViewport11() ); return newViewport; }
};

struct CCBufferSlot abstract : boost::noncopyable
{
	const UINT StartSlot;
	const UINT BufferSize;
	ID3D11Buffer* pIBuffer;
	ULONG_PTR Usage;

	CCBufferSlot( UINT pStartSlot, UINT pBufferSize, ULONG_PTR pUsage );

	D3D11_BUFFER_DESC * GetDesc();
	typedef CMonadIPtrBasic<ID3D11Buffer> CAutoBuffer;

	struct CCBufferRepository sealed
	{
		typedef std::map<UINT, CAutoBuffer> CAllSlots;
		CAllSlots mAllSlots;
	};

protected:
	void CCBufferSlot::Export( const void * pCBuffer ) const;
};

struct CCBufferGeneric abstract : public MonadMath::CAligned16TypeGeneric, protected CCBufferSlot
{
	CCBufferGeneric( const UINT pSize, const UINT pStartSlot, ULONG_PTR pUsage );

	inline void Export() const
	{
		CCBufferSlot::Export( GetVar() );
	}
};

template
<
UINT pStartSlot,
ULONG_PTR pUsage
>
struct CCBufferWithSlotGeneric abstract : public CCBufferGeneric
{
	CCBufferWithSlotGeneric( const UINT pSize ) : CCBufferGeneric( pSize, pStartSlot, pUsage ) {}
};

template
<
typename SrcType,
UINT pStartSlot,
ULONG_PTR pUsage
>
struct CCBuffer : public MonadMath::CAligned16Type< SrcType, CCBufferWithSlotGeneric < pStartSlot, pUsage > >
{
	CCBuffer() : MonadMath::CAligned16Type< SrcType, CCBufferWithSlotGeneric < pStartSlot, pUsage > >() {}
};

class CMonadShaderGeneric : public CWorldGrCombine, private MonadEvent::COnEvent
{
public:
	CMonadShaderGeneric(
		const std::wstring & pFilename,
		const char * pTechnique,
		const std::wstring & pMeshFilename,
		const structDeclarations::CDecl & pFVF,
		const char * pShadowTechnique = NULL );

	virtual void OnFrameRender();
	virtual bool IsDynamic() { return false; }

	// Settings for constants each frame
	virtual void OnComputePerFrame( const XMMATRIX * pWorld ) {};
	virtual void OnUpdateDX9PerFrame() {};
	virtual void OnUpdateDX11PerFrame() {};

	// Settings for constants per device reset
	virtual void OnComputePerReset() {};
	virtual void OnUpdateDX9PerReset() {};
	virtual void OnUpdateDX11PerReset() {};

	void SetFloat( D3DXHANDLE hConstant, const float pFloat );
	void SetVector( D3DXHANDLE hConstant, const XMFLOAT4 & pVector );
	void SetVector( D3DXHANDLE hConstant, const XMVECTOR & pVector );
	void SetMatrix( D3DXHANDLE hConstant, const XMMATRIX & pMatrix );
	void SetTexture( D3DXHANDLE hConstant, CAbstractTexture & pTexture );
	void SetTexture( D3DXHANDLE hConstant, LPDIRECT3DBASETEXTURE9 pTexture );
	void SetShaderResource( UINT StartSlot, CAbstractTexture & pTexture );
	D3DXHANDLE GetTechniqueByName( LPD3DXEFFECT pEffect, const char * pTechnique );

	static MONAD_SHADOW_MODE & IsShadowRendered() { static MONAD_SHADOW_MODE bIsShadowRendered; return bIsShadowRendered; };

private:
	struct StructShadow
	{
		XMMATRIX g_mShadowModelViewProj;
	};
	typedef CCBuffer<StructShadow, CB_SHADOW, MONAD_USAGE_VS> CBufferShadow;
	CBufferShadow m_CBufferShadow;

	void SetShadowMatrix( D3DXHANDLE hConstant, const XMMATRIX & pMatrix );
	union D3DUnion
	{
		inline D3DUnion()
		{
			ZeroMemory( this, sizeof( *this ) );
		}
		struct
		{
			LPD3DXEFFECT m_pEffect;
			LPD3DXEFFECT m_pShadowEffect;
			D3DXHANDLE m_hTechnique;
			D3DXHANDLE m_hShadowTechnique;
			IDirect3DVertexDeclaration9 * mFVF;
		} D3D9;
		struct
		{
			ID3D11VertexShader*                 pVertexShader11;
			ID3D11PixelShader*                  pPixelShader11;
			ID3D11InputLayout*					pInputLayout;
			ID3D11VertexShader*                 pVertexShader11Shadow;
			ID3D11PixelShader*                  pPixelShader11Shadow;
			ID3D11InputLayout*					pInputLayoutShadow;
		} D3D11;
	} m_pInterfaces;
	CDXUTSDKMesh * mMesh;
	void SetVector( D3DXHANDLE hConstant, const D3DXVECTOR4 * pVector );
	void SetShadowVector( D3DXHANDLE hConstant, const D3DXVECTOR4 * pVector );
	virtual void OnResetLogic() { OnComputePerReset(); }
	virtual void OnResetLogicDX9() { OnUpdateDX9PerReset(); }
	virtual void OnResetLogicDX11() { OnUpdateDX11PerReset(); }

	struct MonadShaderDX11
	{
		typedef CMonadIPtrBasic<IUnknown> CShaderIPtr;
		typedef CMonadIPtrBasic<ID3D11InputLayout> CLayout;
		CShaderIPtr mShader;
		CLayout mLayout;

		MonadShaderDX11( ID3D11VertexShader * pShader, ID3D11InputLayout * pInputLayout )
			: mShader( pShader ), mLayout( pInputLayout )
		{
		}
		MonadShaderDX11( ID3D11PixelShader * pShader )
			: mShader( pShader )
		{
		}
		inline void GetInterfaces( ID3D11VertexShader ** pVertexShader, ID3D11InputLayout ** pInputLayout ) const throw()
		{
			*pVertexShader = GetVS();
			*pInputLayout = GetILayout();
		}
		MonadShaderDX11& operator=( const MonadShaderDX11& p ); // Denied
		ID3D11VertexShader * GetVS() const throw() { return reinterpret_cast<ID3D11VertexShader *> ( mShader.GetInterface() ); }
		ID3D11PixelShader * GetPS() const throw() { return reinterpret_cast<ID3D11PixelShader *> ( mShader.GetInterface() ); }
		ID3D11InputLayout * GetILayout() const throw() { return mLayout; }
	};

public:
	class CShaders
	{
		typedef std::map<std::wstring, MonadShaderDX11> CAllShaders;

	public:
		const MonadShaderDX11 & GetShader( const std::wstring & pFunctionName );
		void CompileVertexShader(
			const std::wstring & pFunctionName,
			ID3D11VertexShader ** ppVertexShader11,
			const structDeclarations::CDecl & pDeclD3D11,
			ID3D11InputLayout ** pInputLayout );
		void CompilePixelShader(
			const std::wstring & pFunctionName,
			ID3D11PixelShader ** ppPixelShader11 );

	private:
		CAllShaders AllShaders;

		// Compile the shaders
#if defined( DEBUG ) || defined( _DEBUG )
		const static DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
#else
		const static DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif
	};
};

#endif