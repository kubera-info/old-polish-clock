// ######################################
// wskaz.h - pakiet wskazówek zegarowych.
// ######################################

#ifndef _WSKAZ_H_
#define _WSKAZ_H_

#include "stdafx.h"
#include "Zegar.h"

#if defined(CPP_RAW_PRIMITIVE)

#include "Kreatory\$wskaz$.h"

#else

// ###########################################
// Nazwa: class CWskazowka
// Opis: Klasa pierwotna dla wskazówek.
//       Dziedziczy po CShadowMesh,
//		 wiêc wskazówki bêd¹ rzucaæ cieñ.
//       Przy obrocie wskazówki jest
//       regenerowany jej model cienia
//       (tylko po modyfikacji zmiennej czasu).
// ############################################
class CWskazowka : public CShadowMesh
{
public:
	CWskazowka(const INT p_ID) : CShadowMesh(p_ID)
	{
		MyAngle = 100.0; // Wartoœæ niespotykana podczas dalszych obliczeñ.
		// S³u¿y inicjalizacji cienia.
		MyInvalidatedShadow = true;
	}
	virtual void OnFrameMove( )
	{
		// Calculation first
		float MyNewAngle = WezKat();
		if(MyAngle != MyNewAngle)
		{
			MyAngle = MyNewAngle;
			MyInvalidatedShadow = true;
			D3DXMatrixRotationZ(&m_rotation, 1.5f * D3DX_PI + MyAngle);
			D3DXMatrixMultiply(&m_rotation, &m_rotation, &g_small_world);
		}

		CShadowMesh::OnFrameMove( );
	}
	virtual void OnFrameRender( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		RotateMyWorld( pd3dDevice );

		CShadowMesh::OnFrameRender( pd3dDevice );
	}
	virtual void OnCastShadow( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		RotateMyWorld( pd3dDevice );

		CShadowMesh::OnCastShadow( pd3dDevice );		
	}
private:
	void RotateMyWorld( LPDIRECT3DDEVICE9 pd3dDevice )
	{
		pd3dDevice->SetTransform(D3DTS_WORLD, &m_rotation);
	}
	virtual float WezKat() = 0; // Obliczenie k¹ta obróconej wskazówki
	float MyAngle; // Ostatnit k¹t wskazówki
	D3DXMATRIXA16 m_rotation; // Raz obliczona macierz rotacji
};

// #########################
// Nazwa: class CWskazMin
// Opis: Wskazówka minutowa.
// #########################
class CWskazMin : public CWskazowka
{
public:
	CWskazMin() : CWskazowka(IDX_MIN_HAND)
	{
	}
private:
	virtual float WezKat()
	{
		return (float)
			// ((M_PI * 2 * (p_czas.wMinute * 60 + p_czas.wSecond)) / 3600);
			((M_PI_MUL_2 * (D3DSaver::g_aktual_czas.wMinute)) / 60);
	}
};

// #####################################
// Nazwa: class CWskazMin
// Opis: Opcjonalna wskazówka sekundowa.
// #####################################
class CWskazSec : public CWskazowka
{
public:
	CWskazSec() : CWskazowka(IDX_SEC_HAND)
	{
	}
private:
	virtual float WezKat()
	{
		return (float)
			((M_PI_MUL_2 * (D3DSaver::g_aktual_czas.wSecond)) / 60);
	}
};

// #####################################
// Nazwa: class CWskazGodz
// Opis: Wskazówka godzinowa.
//       Wskazówka rysuje dodatkowy
//       trójk¹t, którego nie ma
//       w innych modelach ("ogonek").
// #####################################
class CWskazGodz : public CWskazowka
{
public:
	CWskazGodz() : CWskazowka(IDX_HOUR_HAND)
	{
	}
private:
	virtual float WezKat()
	{
		return (float)
			((M_PI_MUL_2 * ((D3DSaver::g_aktual_czas.wHour % 12) * 60 + D3DSaver::g_aktual_czas.wMinute)) / (12 * 60));
	}
};

#endif

#endif