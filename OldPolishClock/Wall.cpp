#include "DXUT.h"
#include "OPC-Globals.h"
#include "Wall.h"

CBrickWall::CBrickFX::CBrickFX()
: CTexturedShader( "TDiffuseBrickTex", L"brick", L"brick" )
{
}

CBrickWall::CPlaster::CPlaster()
: CMaterialShader( "TDiffuse", L"white", L"square", "ghost" )
{
}

CWoodenMount::CWoodenMount()
: CWoodenShader( L"mount", L"wood", g_Wood3, "ghost" )
{
}