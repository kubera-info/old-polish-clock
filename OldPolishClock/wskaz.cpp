#include "DXUT.h"
#include "wskaz.h"

void CHandCombine::OnNewMatrix( XMMATRIX * pMatrixToAlter )
{
	*pMatrixToAlter = XMMatrixMultiply( XMMatrixRotationZ( 1.5f * XM_PI + WezKat() ), *pMatrixToAlter );
}
const float CHandCombine::SmoothRotate() const
{
	return 59 == g_aktual_czas.wSecond ? (float) g_aktual_czas.wMilliseconds / 1000.0f : 0.0f;
}

CWskazMin::CWskazMin() : CHandCombine( L"min" )
{
}
const float CWskazMin::WezKat() const
{
	return
		( ( XM_2PI * ( ( (float) g_aktual_czas.wMinute ) + SmoothRotate() ) ) / 60.0f );
}

CWskazGodz::CWskazGodz() : CHandCombine( L"hour" )
{
}
const float CWskazGodz::WezKat() const
{
	return
		( ( XM_2PI * ( ( (float) ( g_aktual_czas.wHour % 12 ) * 60 + g_aktual_czas.wMinute ) + SmoothRotate() ) ) / ( 12.0f * 60.0f ) );
}