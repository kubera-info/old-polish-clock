// ###################################
// tarcza.h - pakiet tarczy zegarowej.
// ###################################

#ifndef _TARCZA_H_
#define _TARCZA_H_

#include "stdafx.h"

#if defined(CPP_RAW_PRIMITIVE)

#include "Kreatory\$tarcza$.h"

#else

// ############################
// Nazwa: class CTarcza
// Opis: Bia³a tarcza zegarowa.
// ############################
class CTarcza : public CMesh
{
public:
	CTarcza() : CMesh(IDX_FACE)
	{
	}
};

// #########################################
// Nazwa: class COzdoba
// Opis: Z³ocony i rzeŸbiony ornament.
//       Jest to prostok¹t odrysowuj¹cy
//       teksturê ze zdjêcia.
//       Cieñ, który trafi³by na ten model
//       wykaza³by faktyczn¹ p³askoœæ figury.
//       W celu ukrucia tego efektu,
//       COzdoba nie implenentuje cienia.
// ##########################################
class COzdoba : public CGhostMesh
{
public:
	COzdoba() : CGhostMesh(IDX_ORNAMENT)
	{
	}
};

// #############################
// Nazwa: class CSzklo
// Opis: Szk³o nie rzuca cienia.
// #############################
class CSzklo : public CGhostMesh
{
public:
	CSzklo() : CGhostMesh(IDX_GLASS)
	{
	}
};

// #######################################
// Nazwa: class CCone
// Opis: Niewidoczny obiekt, który
//       wchodzi w interakcjê z cieniem.
//       Pozoruje on trójwymiar ornamentu,
//       gdy¿ sam nie jest p³aski.
// #######################################
class CCone : public CMesh
{
public:
	CCone() : CMesh(IDX_CONE)
	{
	}
};

#endif

#endif