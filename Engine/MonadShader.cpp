#include "DXUT.h"
#include "MonadShader.h"
#include "Singleton.h"
#include "MonadException.h"
#include "Light.h"
#include "D3DSaver.h"

const int CShadowHelper::TEXDEPTH_HEIGHT = 2048;
const int CShadowHelper::TEXDEPTH_WIDTH = 4096;

void ClearTextures()
{
	ID3D11ShaderResourceView * const MonadResourceViews [] = { NULL, NULL, NULL, NULL, NULL, NULL };
	DXUTGetD3D11DeviceContext()->PSSetShaderResources( 0, sizeof( MonadResourceViews ) / sizeof( ID3D11ShaderResourceView * ), MonadResourceViews );
}
CShadowHelper::CShadowHelper() :
m_fDepthBias( 0.0004f ),
m_fBiasSlope( 5.0f )
{
	if( DXUT_D3D9_DEVICE == DeviceLevel )
		RegisterEvent( MonadEvent::EventResetLogicDX9 );
	else
	{
		mLevels.reserve( s_ulShadowCnt );
		for( ULONG_PTR iLevel = 0; iLevel < s_ulShadowCnt; ++iLevel )
		{
			CComPtr<ID3D11ShaderResourceView> DepthMapSRV;
			CComPtr<ID3D11DepthStencilView> DepthMapDSV;

			CComPtr<ID3D11Texture2D> depthMap;
			D3D11_TEXTURE2D_DESC texDesc;
			texDesc.Width = TEXDEPTH_WIDTH >> 1;
			texDesc.Height = TEXDEPTH_HEIGHT;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
			texDesc.SampleDesc.Count = 1;
			texDesc.SampleDesc.Quality = 0;
			texDesc.Usage = D3D11_USAGE_DEFAULT;
			texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			texDesc.CPUAccessFlags = 0;
			texDesc.MiscFlags = 0;
			THROW_EXC_D3D( DXUTGetD3D11Device()->CreateTexture2D( &texDesc, 0, &depthMap ), L"Create Depth" );

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = 0;
			dsvDesc.Flags = 0;
			THROW_EXC_D3D( DXUTGetD3D11Device()->CreateDepthStencilView( depthMap, &dsvDesc, &DepthMapDSV ), L"Create Stencil" );
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			THROW_EXC_D3D( DXUTGetD3D11Device()->CreateShaderResourceView( depthMap, &srvDesc, &DepthMapSRV ), L"Depth Resource" );
			XMFLOAT3 Light;
			switch( iLevel )
			{
			case 0 :
				XMStoreFloat3( &Light, g_lightPosition );
				mLevels.push_back( COneShadow( Light, &SHADOW_TEX_MATRIX0, DepthMapSRV.Detach(), DepthMapDSV.Detach() ) );
				break;
			case 1 :
				XMStoreFloat3( &Light, XMVectorScale( g_lightPosition, .9925f ) );
				mLevels.push_back( COneShadow( Light, &SHADOW_TEX_MATRIX1, DepthMapSRV.Detach(), DepthMapDSV.Detach() ) );
				break;
			case 2 :
				XMStoreFloat3( &Light, XMVectorScale( g_lightPosition, .978f ) );
				mLevels.push_back( COneShadow( Light, &SHADOW_TEX_MATRIX2, DepthMapSRV.Detach(), DepthMapDSV.Detach() ) );
				break;
			}
		}
	}
}
void CShadowHelper::OnResetLogicDX9()
{
	CComPtr<IDirect3DTexture9> tmpTex;
	CComPtr<IDirect3DSurface9> tmpSurf;

	if( NULL == GetColorTexture().GetInterface() )
	{
		D3DFORMAT colorFormat = D3DFMT_A8R8G8B8;

		THROW_EXC_D3D( 
			DXUTGetD3D9Device()->CreateTexture(
			TEXDEPTH_WIDTH,
			TEXDEPTH_HEIGHT, 
			1, 
			D3DUSAGE_RENDERTARGET, 
			colorFormat,     
			D3DPOOL_DEFAULT, 
			&tmpTex, 
			NULL), L"CreColorTexture" );
		GetColorTexture().resetRelease( tmpTex.Detach() );

		THROW_EXC_D3D(
			GetColorTexture()->GetSurfaceLevel( 0, &tmpSurf ), L"GetColorSurface" );
		GetColorSurface().resetRelease( tmpSurf.Detach() );
	}
	if( NULL == m_pSMZTexture.GetInterface() )
	{
		D3DFORMAT zFormat = D3DFMT_D24S8;	

		THROW_EXC_D3D( 
			DXUTGetD3D9Device()->CreateTexture(
			TEXDEPTH_WIDTH, 
			TEXDEPTH_HEIGHT, 
			1, 
			D3DUSAGE_DEPTHSTENCIL, 
			zFormat, 
			D3DPOOL_DEFAULT, 
			&tmpTex, 
			NULL), L"Z-Tex" );
		m_pSMZTexture.resetRelease( tmpTex.Detach() );

		THROW_EXC_D3D( 
			m_pSMZTexture->GetSurfaceLevel(0, &tmpSurf ), L"Z-Surface" );
		m_pSMZSurface.resetRelease( tmpSurf.Detach() );
	}
	if( NULL == GetBackBuffer().GetInterface() )
	{
		THROW_EXC_D3D( DXUTGetD3D9Device()->GetRenderTarget( 0, &tmpSurf ), L"BackBuffer" );
		GetBackBuffer().resetRelease( tmpSurf.Detach() );
		THROW_EXC_D3D( DXUTGetD3D9Device()->GetDepthStencilSurface( &tmpSurf ), L"Z-Buffer" );
		GetZBuffer().resetRelease( tmpSurf.Detach() );
	}
}
void CShadowHelper::SaveD3D9World()
{
	//save old viewport
	THROW_EXC_D3D( DXUTGetD3D9Device()->GetViewport(&GetOldViewport()), L"GetViewport" );
}
void CShadowHelper::BeforeD3D9Rendering()
{
	THROW_EXC_D3D( DXUTGetD3D9Device()->SetRenderTarget(0, GetColorSurface() ), L"ColorSurface" );
	THROW_EXC_D3D( DXUTGetD3D9Device()->SetDepthStencilSurface( m_pSMZSurface ), L"Surface" );

	//set new, funky viewport
	THROW_EXC_D3D( DXUTGetD3D9Device()->SetViewport(&GetNewViewport()), L"Viewport" );

	THROW_EXC_D3D( DXUTGetD3D9Device()->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&m_fDepthBias), L"DBIAS" );
	THROW_EXC_D3D( DXUTGetD3D9Device()->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD*)&m_fBiasSlope), L"SBIAS" );
	THROW_EXC_D3D( DXUTGetD3D9Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FFFFFF, 1.0f, 0L), L"Clear" );
}

const D3DVIEWPORT9 CShadowHelper::GetDefaultNewViewport()
{
	D3DVIEWPORT9 newViewport;
	newViewport.X = 0;
	newViewport.Y = 0;
	newViewport.Width  = TEXDEPTH_WIDTH;
	newViewport.Height = TEXDEPTH_HEIGHT;
	newViewport.MinZ = 0.0f;
	newViewport.MaxZ = 1.0f;

	return newViewport;
}
void CShadowHelper::AfterD3D9Rendering()
{
	//depth bias
	float fTemp = 0.0f;
	THROW_EXC_D3D( DXUTGetD3D9Device()->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&fTemp), L"Restore DEPTHBIAS" );
	THROW_EXC_D3D( DXUTGetD3D9Device()->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD*)&fTemp), L"Restore SLOPESCALEDEPTHBIAS" );
	THROW_EXC_D3D( DXUTGetD3D9Device()->SetViewport(&GetOldViewport()), L"Restore Viewport" );

	THROW_EXC_D3D( DXUTGetD3D9Device()->SetRenderTarget(0, GetBackBuffer() ), L"Restore BackBuffer" );
	THROW_EXC_D3D( DXUTGetD3D9Device()->SetDepthStencilSurface( GetZBuffer() ), L"Restore Z-Buffer" );
	THROW_EXC_D3D( DXUTGetD3D9Device()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR( 0.0, 0.0, 0.0, 1.0), 1.0f, 0L ), L"Clear" );
}
void CShadowHelper::SaveD3D11World()
{
	UINT Cnt( 1 );
	DXUTGetD3D11DeviceContext()->RSGetViewports( &Cnt, &GetOldViewport11() );
	Singleton->pRTV = DXUTGetD3D11RenderTargetView();
	Singleton->pDSV = DXUTGetD3D11DepthStencilView();
}
const D3D11_VIEWPORT CShadowHelper::GetDefaultNewViewport11()
{
	D3D11_VIEWPORT newViewport;
	newViewport.TopLeftX = 0;
	newViewport.TopLeftY = 0;
	newViewport.Width  = float( TEXDEPTH_WIDTH >> 1 );
	newViewport.Height = float( TEXDEPTH_HEIGHT );
	newViewport.MinDepth = 0.0f;
	newViewport.MaxDepth = 1.0f;

	return newViewport;
}
void CShadowHelper::BeforeD3D11Rendering( ULONG_PTR pShadowLevelId )
{
	mLevels[ pShadowLevelId ].BeforeD3D11Rendering();
}
void CShadowHelper::AfterD3D11Rendering()
{
	ID3D11RenderTargetView * renderTargets[ 1 ] = { Singleton->pRTV };
	DXUTGetD3D11DeviceContext()->OMSetRenderTargets( 1, renderTargets, Singleton->pDSV );
	DXUTGetD3D11DeviceContext()->RSSetViewports( 1, &GetOldViewport11() );
	static const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	DXUTGetD3D11DeviceContext()->ClearRenderTargetView( Singleton->pRTV, ClearColor );
	DXUTGetD3D11DeviceContext()->ClearDepthStencilView( Singleton->pDSV, D3D11_CLEAR_DEPTH, 1.0, 0 );
}
CShadowHelper::COneShadow::COneShadow(
									  const XMFLOAT3 & pEyePtInternal,
									  XMMATRIX * pShadowMatrix,
									  ID3D11ShaderResourceView * pDepthMapSRV,
									  ID3D11DepthStencilView * pDepthMapDSV )
									  : mDepthMapSRV( pDepthMapSRV ), mDepthMapDSV( pDepthMapDSV ), mMyShadowMatrix( pShadowMatrix )
{
	const XMFLOAT3A vLookatPtInternal( pEyePtInternal.x / 2.0f, 0.0f, 0.3f );
	const XMVECTOR vEyePt = XMLoadFloat3( &pEyePtInternal );
	const XMVECTOR vLookatPt = XMLoadFloat3( &vLookatPtInternal );
	XMMATRIX SHADOW_PROJECTION_MATRIX = XMMatrixPerspectiveFovLH( D3DXToRadian( 75.0f ), 1.0f, 1.0f, 12.0f );
	XMMATRIX SHADOW_VIEW_MATRIX = XMMatrixLookAtLH( vEyePt, vLookatPt, vUpVec );
	*pShadowMatrix = XMMatrixMultiply( SHADOW_VIEW_MATRIX, SHADOW_PROJECTION_MATRIX );
}
void CShadowHelper::COneShadow::BeforeD3D11Rendering()
{
	ID3D11RenderTargetView * renderTargets[ 1 ] = { NULL };
	DXUTGetD3D11DeviceContext()->OMSetRenderTargets( 1, renderTargets, mDepthMapDSV );
	DXUTGetD3D11DeviceContext()->RSSetViewports( 1, &GetNewViewport11() );
	DXUTGetD3D11DeviceContext()->ClearDepthStencilView( mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	SHADOW_TEX_MATRIX = *mMyShadowMatrix;
}

CMonadShaderGeneric::CMonadShaderGeneric(
	const std::wstring & pFilename,
	const char * pTechnique,
	const std::wstring & pMeshFilename,
	const structDeclarations::CDecl & pFVF,
	const char * pShadowTechnique )
	: mMesh( NULL )
{
	RegisterEvent( MonadEvent::EventResetLogic );
	mMesh = Singleton->MonadFactory.GetMesh( pMeshFilename );
	if( DXUT_D3D9_DEVICE == DeviceLevel )
	{		
		m_pInterfaces.D3D9.m_pEffect = Singleton->MonadFactory.GetShader( pFilename );
		m_pInterfaces.D3D9.m_hTechnique = GetTechniqueByName( m_pInterfaces.D3D9.m_pEffect, pTechnique );
		m_pInterfaces.D3D9.mFVF = pFVF.DeclD3D9;

		if( NULL == pShadowTechnique )
		{
			m_pInterfaces.D3D9.m_pShadowEffect = Singleton->MonadFactory.GetShader( L"shadow" );
			m_pInterfaces.D3D9.m_hShadowTechnique = 
				GetTechniqueByName(
				m_pInterfaces.D3D9.m_pShadowEffect, 
				pFVF.m_szShadowTechnique );
		}
		RegisterEvent( MonadEvent::EventResetLogicDX9 );
	}
	else
	{
		const CMonadShaderConfig WorldFX( Singleton->MonadFactory.GetFXConfig( MonadI18N::AnsiToUnicode( pTechnique ) ) );		
		Singleton->mShaders11.CompileVertexShader(
			WorldFX.GetVS(),
			&m_pInterfaces.D3D11.pVertexShader11,
			pFVF,
			&m_pInterfaces.D3D11.pInputLayout );
		Singleton->mShaders11.CompilePixelShader(
			WorldFX.GetPS(),
			&m_pInterfaces.D3D11.pPixelShader11 );
		if( NULL == pShadowTechnique )
		{
			const CMonadShaderConfig ShadowFX( Singleton->MonadFactory.GetFXConfig( MonadI18N::AnsiToUnicode( pFVF.m_szShadowTechnique ) ) );
			Singleton->mShaders11.CompileVertexShader(  
				ShadowFX.GetVS(),
				&m_pInterfaces.D3D11.pVertexShader11Shadow,
				pFVF,
				&m_pInterfaces.D3D11.pInputLayoutShadow );
			Singleton->mShaders11.CompilePixelShader(
				ShadowFX.GetPS(),
				&m_pInterfaces.D3D11.pPixelShader11Shadow );
		}
		RegisterEvent( MonadEvent::EventResetLogicDX11 );
	}
}

D3DXHANDLE CMonadShaderGeneric::GetTechniqueByName( LPD3DXEFFECT pEffect, const char * pTechnique )
{
	D3DXHANDLE tmp( pEffect->GetTechniqueByName( pTechnique ) );
	THROW_EXC_IFZ( tmp, MonadException::NoDataFound, L"Technique" );
	return tmp;
}

void CMonadShaderGeneric::OnFrameRender()
{
	XMMATRIX World( XMLoadFloat4x4( &Singleton->mMatrixStack.top() ) );
	OnNewMatrix( &World );
	OnComputePerFrame( &World );

	if( DXUT_D3D9_DEVICE == DeviceLevel )
	{
		THROW_EXC_D3D( DXUTGetD3D9Device()->SetVertexDeclaration( m_pInterfaces.D3D9.mFVF ), L"FVF" );

		if( MONAD_WORLD != IsShadowRendered() )
		{
			if( MONAD_DYNAMIC_SHADOW == IsShadowRendered() && !IsDynamic() )
				return;
			if( MONAD_STATIC_SHADOW == IsShadowRendered() && IsDynamic() )
				return;
			if( NULL != m_pInterfaces.D3D9.m_pShadowEffect )
			{
				SetShadowMatrix(
					gD3D9Handles.hg_mShadowModelViewProj, 
					XMMatrixMultiply( World, SHADOW_TEX_MATRIX ) );
				mMesh->Render(
					DXUTGetD3D9Device(),
					m_pInterfaces.D3D9.m_pShadowEffect,
					m_pInterfaces.D3D9.m_hShadowTechnique );
			}
		}
		else
		{
			// Shadow Mapping - begins
			SetTexture( gD3D9Handles.hShadowMap, Singleton->mShadowHelper.m_pSMZTexture );
			SetTexture( gD3D9Handles.hShadowMapDyn, Singleton->mShadowHelperDyn.m_pSMZTexture );			
			OnUpdateDX9PerFrame();
			mMesh->Render(
				DXUTGetD3D9Device(),
				m_pInterfaces.D3D9.m_pEffect,
				m_pInterfaces.D3D9.m_hTechnique );
		}
	}
	else
	{
		if( MONAD_WORLD != IsShadowRendered() )
		{
			if( MONAD_DYNAMIC_SHADOW == IsShadowRendered() && !IsDynamic() )
				return;
			if( MONAD_STATIC_SHADOW == IsShadowRendered() && IsDynamic() )
				return;
			if( NULL != m_pInterfaces.D3D11.pVertexShader11Shadow )
			{
				DXUTGetD3D11DeviceContext()->IASetInputLayout( m_pInterfaces.D3D11.pInputLayoutShadow );
				DXUTGetD3D11DeviceContext()->VSSetShader( m_pInterfaces.D3D11.pVertexShader11Shadow, NULL, 0 );
				DXUTGetD3D11DeviceContext()->PSSetShader( m_pInterfaces.D3D11.pPixelShader11Shadow, NULL, 0 );

				m_CBufferShadow.GetVal().g_mShadowModelViewProj = XMMatrixTranspose( XMMatrixMultiply( World, SHADOW_TEX_MATRIX ) );
				m_CBufferShadow.Export();
				mMesh->Render( DXUTGetD3D11DeviceContext() );
			}
		}
		else
		{
			DXUTGetD3D11DeviceContext()->IASetInputLayout( m_pInterfaces.D3D11.pInputLayout );
			DXUTGetD3D11DeviceContext()->VSSetShader( m_pInterfaces.D3D11.pVertexShader11, NULL, 0 );
			DXUTGetD3D11DeviceContext()->PSSetShader( m_pInterfaces.D3D11.pPixelShader11, NULL, 0 );
			ID3D11ShaderResourceView * const MonadResourceViews [] = 
			{ Singleton->mShadowHelper.mLevels[ 0 ].mDepthMapSRV.GetInterface(), 
			Singleton->mShadowHelper.mLevels[ 1 ].mDepthMapSRV.GetInterface(), 
			Singleton->mShadowHelper.mLevels[ 2 ].mDepthMapSRV.GetInterface(), 
			Singleton->mShadowHelperDyn.mLevels[ 0 ].mDepthMapSRV.GetInterface(),
			Singleton->mShadowHelperDyn.mLevels[ 1 ].mDepthMapSRV.GetInterface(),
			Singleton->mShadowHelperDyn.mLevels[ 2 ].mDepthMapSRV.GetInterface() };
			DXUTGetD3D11DeviceContext()->PSSetShaderResources( 0, sizeof( MonadResourceViews ) / sizeof( ULONG_PTR ), MonadResourceViews );
			OnUpdateDX11PerFrame();
			mMesh->Render( DXUTGetD3D11DeviceContext() );
		}				
	}
}
void CMonadShaderGeneric::SetFloat( D3DXHANDLE hConstant, const float pFloat )
{
	assert( m_pInterfaces.D3D9.m_pEffect );
	THROW_EXC_D3D( m_pInterfaces.D3D9.m_pEffect->SetFloat( hConstant, pFloat), L"SetFloat" );
}
void CMonadShaderGeneric::SetVector( D3DXHANDLE hConstant, const D3DXVECTOR4 * pVector )
{
	assert( m_pInterfaces.D3D9.m_pEffect );
	THROW_EXC_D3D( m_pInterfaces.D3D9.m_pEffect->SetVector( hConstant, pVector ), L"SetVector" );
}
void CMonadShaderGeneric::SetVector( D3DXHANDLE hConstant, const XMFLOAT4 & pVector )
{
	SetVector( hConstant, reinterpret_cast<const D3DXVECTOR4 *> ( &pVector ) );
}
void CMonadShaderGeneric::SetVector( D3DXHANDLE hConstant, const XMVECTOR & pVector )
{
	SetVector( hConstant, reinterpret_cast<const D3DXVECTOR4 *> ( &pVector ) );
}
void CMonadShaderGeneric::SetShadowVector( D3DXHANDLE hConstant, const D3DXVECTOR4 * pVector )
{
	assert( m_pInterfaces.D3D9.m_pEffect );
	THROW_EXC_D3D( m_pInterfaces.D3D9.m_pShadowEffect->SetVector( hConstant, pVector ), L"SetShadowVector" );
}
void CMonadShaderGeneric::SetMatrix( D3DXHANDLE hConstant, const XMMATRIX & pMatrix )
{
	THROW_EXC_D3D( m_pInterfaces.D3D9.m_pEffect->SetMatrix( hConstant, reinterpret_cast<const D3DXMATRIX *> ( &pMatrix ) ), L"SetMatrix" );
}
void CMonadShaderGeneric::SetShadowMatrix( D3DXHANDLE hConstant, const XMMATRIX & pMatrix )
{
	THROW_EXC_D3D( m_pInterfaces.D3D9.m_pShadowEffect->SetMatrix( hConstant, reinterpret_cast<const D3DXMATRIX *> ( &pMatrix ) ), L"SetShadowMatrix" );
}
void CMonadShaderGeneric::SetTexture( D3DXHANDLE hConstant, CAbstractTexture & pTexture )
{
	SetTexture( hConstant, ( LPDIRECT3DBASETEXTURE9 ) pTexture );
}
void CMonadShaderGeneric::SetTexture( D3DXHANDLE hConstant, LPDIRECT3DBASETEXTURE9 pTexture )
{
	THROW_EXC_D3D( m_pInterfaces.D3D9.m_pEffect->SetTexture( hConstant, pTexture ), L"SetTexture" );
}
void CMonadShaderGeneric::SetShaderResource( UINT StartSlot, CAbstractTexture & pTexture )
{
	DXUTGetD3D11DeviceContext()->PSSetShaderResources( StartSlot, 1, pTexture );
}

// Repostiory of unique pFilename X pFunctionName shader interfaces
const CMonadShaderGeneric::MonadShaderDX11 & CMonadShaderGeneric::CShaders::GetShader( const std::wstring & pFunctionName )
{
	return AllShaders.find( pFunctionName )->second;
}
void CMonadShaderGeneric::CShaders::CompileVertexShader(
	const std::wstring & pFunctionName,
	ID3D11VertexShader ** ppVertexShader11,
	const structDeclarations::CDecl & pDeclD3D11,
	ID3D11InputLayout ** ppInputLayout ) 
{
	CAllShaders::const_iterator it( AllShaders.find( pFunctionName ) );
	if( AllShaders.end() != it )
		it->second.GetInterfaces( ppVertexShader11, ppInputLayout );
	else
	{
		ID3DBlob * pSrcBlob = Singleton->MonadFactory.GetShader11( pFunctionName );
		THROW_EXC_D3D( DXUTGetD3D11Device()->CreateVertexShader( pSrcBlob->GetBufferPointer(),
			pSrcBlob->GetBufferSize(), NULL, ppVertexShader11 ), L"VS Create" );
		DXUT_SetDebugName( *ppVertexShader11, "Monad VS" );

		THROW_EXC_D3D( DXUTGetD3D11Device()->CreateInputLayout( 
			pDeclD3D11.DeclD3D11, pDeclD3D11.Decl11Size, 
			pSrcBlob->GetBufferPointer(),                                   
			pSrcBlob->GetBufferSize(), 
			ppInputLayout ), L"CreLayout" );
		DXUT_SetDebugName( *ppInputLayout, "Monad Input Layout" );

		AllShaders.insert( std::make_pair( pFunctionName, MonadShaderDX11( *ppVertexShader11, *ppInputLayout ) ) );
	}
};
void CMonadShaderGeneric::CShaders::CompilePixelShader(
	const std::wstring & pFunctionName,
	ID3D11PixelShader ** ppPixelShader11 )
{
	CAllShaders::const_iterator it( AllShaders.find( pFunctionName ) );
	if( AllShaders.end() != it )
		*ppPixelShader11 = it->second.GetPS();
	else
	{
		ID3DBlob * pSrcBlob = Singleton->MonadFactory.GetShader11( pFunctionName );
		THROW_EXC_D3D( DXUTGetD3D11Device()->CreatePixelShader( pSrcBlob->GetBufferPointer(),
			pSrcBlob->GetBufferSize(), NULL, ppPixelShader11 ), L"PS Create" );
		DXUT_SetDebugName( *ppPixelShader11, "Monad PS" );
		AllShaders.insert( std::make_pair( pFunctionName, *ppPixelShader11 ) );
	}
};

CCBufferSlot::CCBufferSlot( UINT pStartSlot, UINT pBufferSize, ULONG_PTR pUsage )
: StartSlot( pStartSlot ), BufferSize( pBufferSize ), pIBuffer( NULL ), Usage( pUsage )
{
	assert( 0 == pBufferSize % 16 );
	if( DXUT_D3D11_DEVICE == DeviceLevel )
	{
		CCBufferRepository::CAllSlots::const_iterator it = Singleton->Slots.mAllSlots.find( pStartSlot );
		if( Singleton->Slots.mAllSlots.end() != it )
		{
#ifdef DEBUG
			pIBuffer = it->second;
			D3D11_BUFFER_DESC Desc;
			pIBuffer->GetDesc( &Desc );
			assert( Desc.ByteWidth == BufferSize );
#endif
			pIBuffer = it->second;
		}
		else
		{
			THROW_EXC_D3D( DXUTGetD3D11Device()->CreateBuffer( GetDesc(), NULL, &pIBuffer ), L"CreIBuf" );
			DXUT_SetDebugName( pIBuffer, "Buf" );
			Singleton->Slots.mAllSlots.insert( std::make_pair( pStartSlot, CAutoBuffer( pIBuffer ) ) );
		}
	}
}
D3D11_BUFFER_DESC * CCBufferSlot::GetDesc()
{
	// Create constant buffers
	static D3D11_BUFFER_DESC cbDesc;
	ZeroMemory( &cbDesc, sizeof( cbDesc ) );
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	cbDesc.ByteWidth = BufferSize;
	return &cbDesc;
};

void CCBufferSlot::Export( const void * pCBuffer ) const
{
#ifdef DEBUG
	assert( NULL != pCBuffer );
	assert( NULL != pIBuffer );
	D3D11_BUFFER_DESC Desc;
	pIBuffer->GetDesc( &Desc );
	assert( Desc.ByteWidth == BufferSize );
#endif
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	THROW_EXC_D3D( 
		DXUTGetD3D11DeviceContext()->
		Map( pIBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ), L"Map" );
	memcpy( MappedResource.pData, pCBuffer, BufferSize );
	DXUTGetD3D11DeviceContext()->Unmap( pIBuffer, 0 );
	if( ( Usage & MONAD_USAGE_VS ) > 0 )
		DXUTGetD3D11DeviceContext()->VSSetConstantBuffers( StartSlot, 1, &pIBuffer );
	if( ( Usage & MONAD_USAGE_PS ) > 0 )
		DXUTGetD3D11DeviceContext()->PSSetConstantBuffers( StartSlot, 1, &pIBuffer );
}

CCBufferGeneric::CCBufferGeneric( const UINT pSize, const UINT pStartSlot, ULONG_PTR pUsage )
: CAligned16TypeGeneric( pSize ), CCBufferSlot( pStartSlot, pSize, pUsage )
{
}