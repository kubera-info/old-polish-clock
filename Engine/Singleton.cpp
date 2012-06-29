#include "DXUT.h"
#include "Singleton.h"
#include "MonadThread.h"
#include "MonadShader.h"
#include "Light.h"
#include "D3DSaver.h"

CSampler * m_pSamplerLinear, * m_pSamplerAnizotropic;

StructSingleton::StructSingleton()
: mSamplerLinear( D3D11_FILTER_MIN_MAG_MIP_LINEAR ), mSamplerAnizotropic( D3D11_FILTER_ANISOTROPIC, 4 ),
pRTV( NULL ), pDSV( NULL )
{
	if( DXUT_D3D11_DEVICE == DeviceLevel )
	{
		DXUTGetD3D11DeviceContext()->PSSetSamplers( 0, 1, mSamplerLinear );
		DXUTGetD3D11DeviceContext()->PSSetSamplers( 1, 1, mSamplerAnizotropic );		
	}
	if( GetSaverParams().IsFullScreen() )
		m_pSatellites.Attach( new CSatelliteManager() );
}
StructSingleton::~StructSingleton()
{
	MonadEvent::COnEvent::FireEvent( MonadEvent::EventDestroyDevice );	
	SAFE_DELETE( TicToc );
	SAFE_DELETE( Bell );
}
void StructSingleton::CallResetDevice()
{
	MonadEvent::COnEvent::FireEvent( MonadEvent::EventResetDevice );
}
void StructSingleton::CallResetLogic()
{
	MonadEvent::COnEvent::FireEvent( MonadEvent::EventResetLogic );
}
void StructSingleton::CallResetLogicDX9()
{
	MonadEvent::COnEvent::FireEvent( MonadEvent::EventResetLogicDX9 );
}
void StructSingleton::CallLostDevice()
{
	MonadEvent::COnEvent::FireEvent( MonadEvent::EventLostDevice );
}
void StructSingleton::OnCreateDeviceAndShadow()
{	
	LightBuffer.Attach( new CBufferV() );	
	mWorld.Attach( new CWorld() );
	AddRenderedObject( Singleton->mWorld );
}
CAutoPtr<StructSingleton> Singleton;