#include "DXUT.h"
#include "OldPolishClock.h"
#include "Hours.h"
#include "ClockTime.h"

const float ClockWorldMoveX = -0.4f;
const float SmallWorldScale = 0.374f;
const float SmallWorldMoveX = 0.0f;
const float SmallWorldMoveY = -0.605f;
const float SmallWorldMoveZ = -0.008f;

COldPolishClock::COldPolishClock()
: m_string( -1.0f ), m_string2( 1.0f ), m_stringL( -1.0f ), m_stringL2( 1.0f )
{
	AddRenderedObject( &m_oval );
	AddRenderedObject( &m_back );
	AddRenderedObject( &m_box );	
	AddRenderedObject( &m_top );
	AddRenderedObject( &m_pendulum );
	AddRenderedObject( &m_hinge );
	AddRenderedObject( &m_hooks );
	AddRenderedObject( &m_middle_hooks );
	AddRenderedObject( &m_back_hooks );
	AddRenderedObject( &SmallWorld );
	AddRenderedObject( &m_metallic );
	AddRenderedObject( &m_casing );
	AddRenderedObject( &m_side );
	AddRenderedObject( &m_front );
	AddRenderedObject( &m_front2 );
	AddRenderedObject( &m_picture );
	AddRenderedObject( &m_back_nails );
	AddRenderedObject( &m_side_nails );
	AddRenderedObject( &m_wooden_nails );
	AddRenderedObject( &m_string );
	AddRenderedObject( &m_string2 );
	AddRenderedObject( &m_stringL );
	AddRenderedObject( &m_stringL2 );
	
	// Alpha blending
	AddRenderedObject( &mSetBlend );
	AddRenderedObject( &m_glass2 );
	AddRenderedObject( &m_glass );
	AddRenderedObject( &mResetBlend );
}

void COldPolishClock::OnNewMatrix( XMMATRIX * pMatrixToAlter )
{
	const static XMMATRIX translacja( XMMatrixTranslation(
		0.0f, 
		0.0f,
		-.22f) );
	*pMatrixToAlter = XMMatrixMultiply( translacja, *pMatrixToAlter );
}
COldPolishClock::CSmallWorld::CSmallWorld()
{
	AddRenderedObject( &m_wsk_godz );
	AddRenderedObject( &m_wsk_min );
	AddRenderedObject( &m_montazMosiadz );
	AddRenderedObject( &m_tarcza );
	AddRenderedObject( &m_tarcza2 );
}

void COldPolishClock::CSmallWorld::OnNewMatrix( XMMATRIX * pMatrixToAlter )
{
#define	translacja XMMatrixTranslation( SmallWorldMoveX, SmallWorldMoveY, SmallWorldMoveZ)
#define	skala XMMatrixScaling( SmallWorldScale, SmallWorldScale, SmallWorldScale)
	const static XMMATRIX small_scale = XMMatrixMultiply( translacja, skala );
	*pMatrixToAlter = XMMatrixMultiply( small_scale, *pMatrixToAlter );
}

