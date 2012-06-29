#include "DXUT.h"
#include "Hinge.h"

CHinge::CHinge() : CMetallic( L"hinge", g_Join )
{
}

CHooks::CHooks() : CMetallic( L"hook", g_Join )
{
}

CBackHooks::CBackHooks() : CMetallic( L"back_hook", g_Join )
{
}

CMiddleHooks::CMiddleHooks() : CMetallic( L"middle_hook", g_Join )
{
}

COval::COval() : CMetallic( L"oval", g_Oval )
{
}