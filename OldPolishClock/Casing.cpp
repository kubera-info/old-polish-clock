#include "DXUT.h"
#include "Casing.h"
#include "Light.h"
#include "Singleton.h"
#include <math.h>

CWoodenCasing::CWoodenCasing()
: CWoodenShader( L"casing" )
{
}

CWoodenFront::CWoodenFront()
: CTexturedShader( "TBlackWood", L"wood", L"front" )
{
}

CWoodenFront2::CWoodenFront2()
: CWoodenShader( L"front2" )
{
}

CWoodenSide::CWoodenSide()
: CTexturedShader( "TBlackWood", L"wood", L"side" )
{
}

CGlassCasing::CGlassCasing() : CGlassShader( L"glass" )
{
}

CGlassBorderCasing::CGlassBorderCasing() : CAmbientGlassShader( L"glass2" )
{
}

CBackCasing::CBackCasing() : CWoodenShader( L"back", L"wood", g_DarkWood, "ghost" )
{
}

CBoxCasing::CBoxCasing()
: CTexturedShader( "TBlackWood", L"wood", L"box", "ghost" )
{
}
CWoodenTop::CWoodenTop() : CWoodenShader( L"top", L"wood", g_DarkWood, "ghost" )
{
}

CBackNails::CBackNails() : CMetallic( L"nails", g_NailFace )
{
}

CSideNails::CSideNails() : CMetallic( L"side_nails", g_NailBorder )
{
}

CWoodenNails::CWoodenNails() : CMetallic( L"wooden_nails", g_NailBorder )
{
}

/*CHR::CHR() : CMetallic( L"li" )
{
}*/

CPendulum::CPendulum() : CMetallic( L"pendulum", g_Join, "ghost" )
{
}
void CPendulum::OnNewMatrix( XMMATRIX * pMatrixToAlter )
{	
	const float Multi = XM_PI / 1000.0f;
	const float Milli = float ( g_aktual_czas.wMilliseconds ) * Multi;
	const float Radius = g_aktual_czas.wSecond % 2 ? -Milli : Milli;
	const XMMATRIX Rotate = XMMatrixRotationZ( sinf( Radius ) * .2f );
	*pMatrixToAlter = XMMatrixMultiply( Rotate, *pMatrixToAlter );
}