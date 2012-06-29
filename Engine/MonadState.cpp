#include "DXUT.h"
#include "MonadState.h"

CSampler::CSampler( D3D11_FILTER pFilter, UINT pMaxAnizotropy )
: m_pSampler( NULL )
{
	if( DXUT_D3D11_DEVICE == DeviceLevel )
	{
		D3D11_SAMPLER_DESC samDesc;
		ZeroMemory( &samDesc, sizeof( samDesc ) );
		samDesc.Filter = pFilter;
		samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.MaxAnisotropy = pMaxAnizotropy;
		samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samDesc.MaxLOD = D3D11_FLOAT32_MAX;
		THROW_EXC_D3D( DXUTGetD3D11Device()->CreateSamplerState( &samDesc, &m_pSampler ), L"Sampler" );
		DXUT_SetDebugName( m_pSampler, "Sampler" );
	}
}
CSampler::operator ID3D11SamplerState** ()
{
	return &m_pSampler;
}
CBlendState::CBlendState()
: m_pBlendState( NULL )
{
	if( DXUT_D3D11_DEVICE == DeviceLevel )
	{
		D3D11_BLEND_DESC bls;
		ZeroMemory( &bls, sizeof( bls ) );
		bls.RenderTarget[0].BlendEnable = TRUE;  
		bls.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;  
		bls.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;  
		bls.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;  
		bls.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;  
		bls.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;  
		bls.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;  
		bls.RenderTarget[0].RenderTargetWriteMask = 0x0f;  
		THROW_EXC_D3D( DXUTGetD3D11Device()->CreateBlendState(
			&bls,
			&m_pBlendState ), L"Blend State" );
	}
}
CBlendState::operator ID3D11BlendState* ()
{
	return m_pBlendState;
}

void CMonadResetBlendState::OnFrameRender()
{
    FLOAT blendFactor[4] = { 0,0,0,0 };
    DXUTGetD3D11DeviceContext()->OMSetBlendState( NULL, blendFactor, 0xFFFFFFFF );
}

void CMonadSetBlendState::OnFrameRender()
{
    FLOAT blendFactor[4] = { 0,0,0,0 };
    DXUTGetD3D11DeviceContext()->OMSetBlendState( mBlend, blendFactor, 0xFFFFFFFF );
}