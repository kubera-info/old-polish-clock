#pragma once
#ifndef _CLOCKS_WORLD_H_
#define _CLOCKS_WORLD_H_

#include "Manager.h"
#include "Wall.h"
#include "OldPolishClock.h"
#include "Painting.h"

struct CPictureFactory
{
	CPictureFactory();
	CMonad * CreateMonad( const std::wstring & p_szSide, const PAINTING_POS pPos );
	const std::wstring & GetLeft() const throw();
	const std::wstring & GetRight() const throw();

protected:
	const std::wstring m_szGirl;
	const std::wstring m_szJesus;
	const std::wstring m_szBender;
	const std::wstring m_szLandscape;	
	const std::wstring m_szNapoleon;
	const std::wstring m_szAdam;
	std::wstring m_szLeft;
	std::wstring m_szRight;
};

extern CPictureFactory gPicturesFactory;

class CWorld : public CWorldGrCombine
{
public:
	CWorld();

protected:
	class COldPolishClock m_clock;
	class CBrickWall m_wall;
	struct CWoodenMount m_mount;
	CAutoPtr<class CMonad> m_first;
	CAutoPtr<class CMonad> m_second;
};

#endif