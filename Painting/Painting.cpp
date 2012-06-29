#include "DXUT.h"
#include "Painting.h"

static const float fZ = -1.0f;
CArtGeneric::CArtGeneric(
								   const std::wstring & pPictureFilename,
								   const XMFLOAT4X4 & pTransform
								   )
								   : m_border( pPictureFilename + L"_border" ),
								   m_pipe( pPictureFilename + L"_metallic" ),
								   mPos( pTransform )
{
	AddRenderedObject( &m_border );
	AddRenderedObject( &m_pipe );
}
void CArtGeneric::OnNewMatrix( XMMATRIX * pMatrixToAlter )
{
	*pMatrixToAlter = XMMatrixMultiply( XMLoadFloat4x4( &mPos ), *pMatrixToAlter );
}

CPaintingGeneric::CPaintingGeneric( 
								   const std::wstring & pPictureFilename,
								   const XMFLOAT4X4 & pTransform
								   )
								   : CArtGeneric( pPictureFilename, pTransform ), m_canvas( pPictureFilename, pPictureFilename )
{
	AddRenderedObject( &m_canvas );
}

CDuplexGeneric::CDuplexGeneric( 
		const std::wstring & pPictureFilename,
		const std::wstring & pCanvasFilename,
		const XMFLOAT4X4 & pTransform
								   )
								   : CArtGeneric( pPictureFilename, pTransform ), m_canvas( pPictureFilename, pCanvasFilename )
{
	AddRenderedObject( &m_canvas );
}

CPainting::CPainting( const PAINTING_POS pPos )
: CPaintingGeneric( L"ostap", Init( pPos ) )
{
}
XMFLOAT4X4 CPainting::Init( const PAINTING_POS pPos )
{
	const XMMATRIX tmpTransl( XMMatrixTranslation( 2.42f * pPos, 0.0f, fZ ) );
	const XMMATRIX tmpRot( XMMatrixRotationZ( -.05f ) );
	const XMMATRIX Transform( XMMatrixMultiply( tmpRot, tmpTransl ) );

	XMFLOAT4X4 fTransform;
	XMStoreFloat4x4( &fTransform, Transform );

	return fTransform;
}

CJesusPainting::CJesusPainting( const PAINTING_POS pPos )
: CPaintingGeneric( L"jesus", Init( pPos ) )
{
}
XMFLOAT4X4 CJesusPainting::Init( const PAINTING_POS pPos )
{
	const XMMATRIX tmpTransl( XMMatrixTranslation( 2.32f * pPos, 0.0f, fZ ) );
	const XMMATRIX tmpRot( XMMatrixRotationZ( .03f ) );
	const XMMATRIX Transform( XMMatrixMultiply( tmpRot, tmpTransl ) );

	XMFLOAT4X4 fTransform;
	XMStoreFloat4x4( &fTransform, Transform );

	return fTransform;
}

CLandscapePainting::CLandscapePainting( const PAINTING_POS pPos )
: CPaintingGeneric( L"landscape", Init( pPos ) )
{
}
XMFLOAT4X4 CLandscapePainting::Init( const PAINTING_POS pPos )
{
	const XMMATRIX Transform( XMMatrixTranslation( 2.32f * pPos, 0.0f, fZ ) );

	XMFLOAT4X4 fTransform;
	XMStoreFloat4x4( &fTransform, Transform );

	return fTransform;
}

CNapoleonPainting::CNapoleonPainting( const PAINTING_POS pPos )
: CPaintingGeneric( L"napoleon", Init( pPos ) )
{
}
XMFLOAT4X4 CNapoleonPainting::Init( const PAINTING_POS pPos )
{
	const XMMATRIX Transform( XMMatrixTranslation( 2.08f * pPos, 0.0f, fZ ) );

	XMFLOAT4X4 fTransform;
	XMStoreFloat4x4( &fTransform, Transform );

	return fTransform;
}

CGirl::CGirl( const PAINTING_POS pPos ) : CTexturedShader( "TDiffuseTex", L"girl", L"folder" ),
mPos( Init( pPos ) )
{
}
XMFLOAT4X4 CGirl::Init( const PAINTING_POS pPos )
{
	const XMMATRIX Transform( XMMatrixMultiply(
		XMMatrixRotationZ( .03f ),
		XMMatrixTranslation( 1.92f * pPos, 0.0f, -1.0f ) ) );

	XMFLOAT4X4 fTransform;
	XMStoreFloat4x4( &fTransform, Transform );

	return fTransform;
}
void CGirl::OnNewMatrix( XMMATRIX * pMatrixToAlter )
{
	*pMatrixToAlter = XMMatrixMultiply( XMLoadFloat4x4( &mPos ), *pMatrixToAlter );
}

CAMPainting::CAMPainting( const PAINTING_POS pPos )
: m_adam( pPos ), m_mickiewicz( pPos )
{
	AddRenderedObject( &m_adam );
	AddRenderedObject( &m_mickiewicz );
}
CAMPainting::CAdamPainting::CAdamPainting( const PAINTING_POS pPos )
: CDuplexGeneric( L"adam", L"adam", Init( pPos ) )
{
}
XMFLOAT4X4 CAMPainting::CAdamPainting::Init( const PAINTING_POS pPos )
{
	const XMMATRIX Transform( XMMatrixTranslation( 1.52f * pPos, 0.0f, fZ ) );

	XMFLOAT4X4 fTransform;
	XMStoreFloat4x4( &fTransform, Transform );

	return fTransform;
}

CAMPainting::CMickiewiczPainting::CMickiewiczPainting( const PAINTING_POS pPos )
: CDuplexGeneric( L"adam", L"adam2", Init( pPos ) )
{
}
XMFLOAT4X4 CAMPainting::CMickiewiczPainting::Init( const PAINTING_POS pPos )
{
	const XMMATRIX Transform( XMMatrixTranslation( 3.08f * pPos, 0.0f, fZ ) );

	XMFLOAT4X4 fTransform;
	XMStoreFloat4x4( &fTransform, Transform );

	return fTransform;
}