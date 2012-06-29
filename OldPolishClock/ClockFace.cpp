#include "DXUT.h"
#include "Singleton.h"
#include "ClockFace.h"
#include "MetaDefine.h"

CAnimateChange::CAnimateChange() :
iTextureID( 2 ),
iPrevTextureID( 2 )
{
}

void CAnimateChange::Alter( const float pBPM )
{
	SetLerpParams( pBPM, 0.0f, 1.0f );
	Flush();
	iPrevTextureID = iTextureID;
#ifdef _FACE_BLENDING_
	iTextureID = MonadMath::GetRandomULongPtr( 0, g_aktual_czas.wHour > 11 && g_aktual_czas.wHour < 23 ? 3 : 2 );
#else
	iTextureID = 2;
#endif
	Start();
}

bool CAnimateChange::IsAnimated() const
{
	return !IsNormal();
}

bool CAnimateChange::IsNormal() const
{
	return m_startTime <= 0.0f;
}

size_t CAnimateChange::GetTextureID() const
{
	return iTextureID;
}

size_t CAnimateChange::GetPreviousTextureID() const
{
	return iPrevTextureID;
}

CTarcza::CTarcza(
				 const char * pTechnique )
				 : CPointAndAmbientModelGeneric(
				 L"light",
				 pTechnique, 
				 L"clockface", 
				 Singleton->Declarators.m_pTexDecl ),
				 textureFacesPack()
{
}

bool CTarcza::IsEnabled()
{
	return CAnimateChange::GetAnimateChange().IsNormal();
}

void CTarcza::OnUpdateDX9PerFrame()
{
	CPointAndAmbientModelGeneric::OnUpdateDX9PerFrame();

	SetTexture( gD3D9Handles.hTex0, textureFacesPack );
}
void CTarcza::OnUpdateDX11PerFrame()
{
	CPointAndAmbientModelGeneric::OnUpdateDX11PerFrame();

	SetShaderResource( 6, textureFacesPack );
}

size_t CTarcza::CVirtualFaceTexture::GetSelectedTextureIndex() const
{
	return CAnimateChange::GetAnimateChange().GetTextureID();
}

CTarcza2::CTarcza2()
: CTarcza( "TSpecularTex2" )
{
}

bool CTarcza2::IsEnabled()
{
	return CAnimateChange::GetAnimateChange().IsAnimated();
}

void CTarcza2::OnComputePerFrame( const XMMATRIX * pWorld )
{
	CPointAndAmbientModelGeneric::OnComputePerFrame( pWorld );

	m_CBufferFactor.GetVal().fLerp.x = 
		m_CBufferFactor.GetVal().fLerp.y = 
		m_CBufferFactor.GetVal().fLerp.z = 
		m_CBufferFactor.GetVal().fLerp.w = 
		CAnimateChange::GetAnimateChange().GetCurrentValue();
}
void CTarcza2::OnUpdateDX9PerFrame()
{
	CTarcza::OnUpdateDX9PerFrame();

	SetTexture( gD3D9Handles.hTex1, textureSecondaryPack );
	SetVector( gD3D9Handles.hfLerp, m_CBufferFactor.GetVal().fLerp );
}

void CTarcza2::OnUpdateDX11PerFrame()
{
	CTarcza::OnUpdateDX11PerFrame();

	SetShaderResource( 7, textureSecondaryPack );
	m_CBufferFactor.Export();
}

size_t CTarcza2::CVirtualSecondaryFaceTexture::GetSelectedTextureIndex() const
{
	return CAnimateChange::GetAnimateChange().GetPreviousTextureID();
}

CMetallicOrnament::CMetallicOrnament()
: CMetallic( L"ornmnt" )
{
}

void CMetallicOrnament::OnNewMatrix( XMMATRIX * pMatrixToAlter ) 
{ 
#define skala (.88f)
#define tmpScale XMMatrixScaling( skala, skala, skala )
#define tmpTranslate XMMatrixTranslation( 0.0f, 0.0f, 0.036f )
	const static XMMATRIX tmpOrnmnt = XMMatrixMultiply( tmpTranslate, tmpScale );
	*pMatrixToAlter = XMMatrixMultiply( tmpOrnmnt, *pMatrixToAlter );
#undef tmpScale
#undef tmpTranslate
}

CWeight::CWeight()
: CMetallic( L"weight", g_Weight )
{
	const float fSkala (.7f);
	const XMMATRIX tmpScale( XMMatrixScaling( fSkala, fSkala, fSkala ) );
	const XMMATRIX tmpTranslate( XMMatrixTranslation( 0.0f, -2.877f, 0.0f ) );
	XMStoreFloat4x4( &m_MyPos, XMMatrixMultiply( tmpScale, tmpTranslate ) );
}
static const float fDeltaY = .4f;
static const float fStringZ = -.38f;
static const float fStringX = .31f;
static const float fStringY = -.66f;

void CWeight::OnNewMatrix( XMMATRIX * pMatrixToAlter ) 
{
	*pMatrixToAlter = XMMatrixMultiply( XMLoadFloat4x4( &m_MyPos ), *pMatrixToAlter );
}

CClockString::CClockString()
: CMetallic( L"string", g_String )
{
	XMStoreFloat4x4( &m_MyPos, XMMatrixTranslation( 0.0f, fStringY, 0.0f ) );
}

void CClockString::OnNewMatrix( XMMATRIX * pMatrixToAlter ) 
{ 
	*pMatrixToAlter = XMMatrixMultiply( XMLoadFloat4x4( &m_MyPos ), *pMatrixToAlter );
}

CClockStringL::CClockStringL( float fSide )
: CMetallic( L"string_int", g_String )
{
	XMStoreFloat4x4( &m_MyPos, XMMatrixTranslation( fStringX * fSide / 3.0f, fStringY, fStringZ ) );
}

void CClockStringL::OnNewMatrix( XMMATRIX * pMatrixToAlter ) 
{ 
	*pMatrixToAlter = XMMatrixMultiply( XMLoadFloat4x4( &m_MyPos ), *pMatrixToAlter );
}

CTorus::CTorus()
: CMetallic( L"torus", g_Join )
{
}

void CTorus::OnNewMatrix( XMMATRIX * pMatrixToAlter ) 
{ 
#define tmpTranslate XMMatrixTranslation( 0.0f, -2.149f, 0.0f )
	*pMatrixToAlter = XMMatrixMultiply( tmpTranslate, *pMatrixToAlter );
#undef tmpTranslate
}

CWeightAndString::CWeightAndString( float fSide )
{
	const float m_fRelative = ( ( float ) MonadMath::GetRandomLongPtr( -100, 100 ) / 1000.0f );
	const float m_fRotation = ( ( float ) MonadMath::GetRandomLongPtr( -100, 100 ) / 100.0f );
	const XMMATRIX Rotate( XMMatrixRotationY( m_fRotation / 2.0f ) );
	const XMMATRIX Translate( XMMatrixTranslation( fStringX * fSide, fDeltaY + m_fRelative, fStringZ ) );
	XMStoreFloat4x4( &m_MyPos, XMMatrixMultiply( Rotate, Translate ) );
	AddRenderedObject( &mString );
	AddRenderedObject( &mTorus );
	AddRenderedObject( &mWeight );
}
void CWeightAndString::OnNewMatrix( XMMATRIX * pMatrixToAlter )
{
	*pMatrixToAlter = XMMatrixMultiply( XMLoadFloat4x4( &m_MyPos ), *pMatrixToAlter );
}