// ###########################################
// Nazwa: class CMonad
// Opis: Bazowa klasa grafiki Direct3D zegara.
// ###########################################

#pragma once
#ifndef _MINI_INFO_MONAD_H_
#define _MINI_INFO_MONAD_H_

#include "DXUT.h"
#include <boost/utility.hpp>
#include "Patterns.h"
#include "MonadException.h"

class CMonad : public CDestructable, boost::noncopyable
{
public:
	CMonad() {}

	// Klasa dziedzicz¹ca musi implementowaæ rysowanie.
	virtual void OnFrameRender() = 0; 
	virtual bool IsEnabled( )																	{ return true; }
};


#endif