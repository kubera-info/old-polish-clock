// ########################################################
// ogolne.h - pakiet rysuj¹cy mechanizm mocuj¹cy wskazówki.
// ########################################################

#ifndef _NAKRETKA_H_
#define _NAKRETKA_H_

#if defined(CPP_RAW_PRIMITIVE)

#include "Kreatory\$nakretka$.h"

#else

class CMontazMosiadz : public CShadowMesh
{
public:
	CMontazMosiadz() : CShadowMesh(IDX_BRASS)
	{
	}
};

class CMontazStal : public CShadowMesh
{
public:
	CMontazStal() : CShadowMesh(IDX_STEEL)
	{
	}
};

#endif

#endif