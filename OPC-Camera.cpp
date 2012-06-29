#include "DXUT.h"
#include "MetaDefine.h"
#include "OPC-Camera.h"
#include "Matrices.h"
#include "MonadShader.h"
#include "MonadMath.h"
#include "Light.h"
// #include "MonadTime.h"

// using namespace MonadTime;

// const static XMFLOAT3 vClockFaceEyePtInternal( 0.0, -0.2233, .94 );
// const static XMFLOAT3 vClockFaceLookatPtInternal( .0, -0.2233f, 0.0f );
//const static XMFLOAT3 vClockFaceEyePtInternal( 3.7f, 1.0f, .0f );
//const static XMFLOAT3 vClockFaceLookatPtInternal( 0.0f, 0.0f, 0.0f );
//const static XMFLOAT3 vClockFaceEyePtInternal( 1.1f, -.0f, 2.4f );
//const static XMFLOAT3 vClockFaceLookatPtInternal( 0.0f, -1.4f, 0.0f );
//const static XMFLOAT3 vClockFaceEyePtInternal( 2.0f, 1.0f, -0.2f );
//const static XMFLOAT3 vClockFaceLookatPtInternal( 0.0f, -1.0f, 0.0f );
const static XMFLOAT3 vClockFaceEyePtInternal( 2.3f, -0.0f, -.3f );
const static XMFLOAT3 vClockFaceLookatPtInternal( -.0f, 0.f, -.7f );

COPCCamera::COPCCamera()
{
	RegisterEvent( MonadEvent::EventFrameMoveStub );
}
void COPCCamera::OnFrameMoveStub()
{
	// Etap czwarty: macierze charakteryzuj¹ce przestrzeñ.
#ifdef _MONAD_ANIME_
	{
		const double fTime = DXUTGetTime() *.75;

		const double mHorizontal = ( sin( fTime * .1 ) * 1.2 + cos( fTime * .12 ) );
		const double mHorizontal2 = mHorizontal * 1.6;
		const double mHorizontal3 = mHorizontal * .7;
		const double mVertical = sin( fTime * .8f ) * cos( fTime * .15 ) / 1.75 + .35;
		const double mDepth = 2.3 + sin( fTime * .44 );

		const XMFLOAT3A vEyePtInternal( ( float ) mHorizontal2, ( float ) mVertical, ( float ) mDepth );
		const XMFLOAT3A vLookatPtInternal( ( float ) mHorizontal3, 0.0f, 0.4f );
		const XMVECTOR vEyePt = XMLoadFloat3( &vEyePtInternal );
		const XMVECTOR vLookatPt = XMLoadFloat3( &vLookatPtInternal );
		VIEW_MATRIX = XMMatrixLookAtLH( vEyePt, vLookatPt, vUpVec );
	}
#else
	{
		const XMFLOAT3 vEyePtInternal( vClockFaceEyePtInternal );
		const XMFLOAT3 vLookatPtInternal( vClockFaceLookatPtInternal );
		const XMVECTOR vEyePt = XMLoadFloat3( &vEyePtInternal );
		const XMVECTOR vLookatPt = XMLoadFloat3( &vLookatPtInternal );
		PROJECTION_MATRIX = XMMatrixPerspectiveFovLH( XM_PIDIV4, dx / dy, 1.0f, ZFAR() );
		VIEW_MATRIX = XMMatrixLookAtLH( vEyePt, vLookatPt, vUpVec );
	}
#endif
};