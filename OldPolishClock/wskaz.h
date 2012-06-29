// ######################################
// wskaz.h - pakiet wskazówek zegarowych.
// ######################################

#pragma once
#ifndef _WSKAZ_H_
#define _WSKAZ_H_

#include "MonadMath.h"
#include "ClockTime.h"
#include "OPC-Shader.h"

class CHandCombine : public CHandShader
{
public:
	CHandCombine(
		const std::wstring & pFilename ) : 
	CHandShader( pFilename )
	{
	}
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );
	virtual bool IsDynamic() { return true; } 

protected:
	virtual const float SmoothRotate() const;
	virtual const float WezKat() const = 0; // Obliczenie k¹ta obróconej wskazówki
};

// #########################
// Nazwa: class CWskazMin
// Opis: Wskazówka minutowa.
// #########################
class CWskazMin : public CHandCombine
{
public:
	CWskazMin();

protected:
	virtual const float WezKat() const;
};

// #####################################
// Nazwa: class CWskazGodz
// Opis: Wskazówka godzinowa.
//       Wskazówka rysuje dodatkowy
//       trójk¹t, którego nie ma
//       w innych modelach ("ogonek").
// #####################################
class CWskazGodz : public CHandCombine
{
public:
	CWskazGodz();

protected:
	virtual const float WezKat() const;
};

#endif