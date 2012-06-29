#include "DXUT.h"
#include "Manager.h"
#include "MonadException.h"
#include "MonadTime.h"
#include "Singleton.h"

//
// Class CWorldGrCombine
//

void CWorldGrCombine::AddRenderedObject( CMonad * pObject ) throw(...)
{
	m_rendered_objects.push_back( pObject );
}

// Count members of the collection
const CWorldGrCombine::CMonadArray::size_type CWorldGrCombine::GetSize() const
{
	return m_rendered_objects.size();
}

const CWorldGrCombine::CMonadArray::size_type CWorldGrCombine::GetMonadSeq() const
{
	return m_IMonadSeq;
}

void CWorldGrCombine::InvalidateSeq()
{
	m_IMonadSeq = m_rendered_objects.max_size();
}

float CWorldGrCombine::GetMonadFactor() const
{
	if( 0 == GetSize() )
		return 1;
	return float( m_IMonadSeq ) / float( GetSize() );
}

void CWorldGrCombine::OnFrameRender()
{
	XMMATRIX m_NewMatrix( XMLoadFloat4x4( &Singleton->mMatrixStack.top() ) );
	OnNewMatrix( &m_NewMatrix );
	XMFLOAT4X4A sMatrix;
	XMStoreFloat4x4A(
         &sMatrix,
         m_NewMatrix
	);
	Singleton->mMatrixStack.push( sMatrix );

	for( m_IMonadSeq = 0; m_IMonadSeq < m_rendered_objects.size(); m_IMonadSeq++ )
	{
		CMonad * pThis = m_rendered_objects[ m_IMonadSeq ];
		if( pThis->IsEnabled() )
			pThis->OnFrameRender();
	}
	InvalidateSeq();

	Singleton->mMatrixStack.pop();
}

CWorldGrCombine::CStack::CStack()
{
	XMFLOAT4X4A mMatrix;
	XMStoreFloat4x4A(
         &mMatrix,
         XMMatrixIdentity()
	);
	push( mMatrix );
}