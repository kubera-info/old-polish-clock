#pragma once
#ifndef _OLD_POLISH_CLOCK_H_
#define _OLD_POLISH_CLOCK_H_ 

#include "MonadState.h"
#include "Manager.h"
#include "wskaz.h"
#include "ClockFace.h"
#include "Nut.h"
#include "casing.h"
#include "Hinge.h"

extern const float ClockWorldMoveX;
extern const float SmallWorldScale;
extern const float SmallWorldMoveX;
extern const float SmallWorldMoveY;
extern const float SmallWorldMoveZ;

class CWorld;

class COldPolishClock : public CWorldGrCombine
{
	friend CWorld;
public:
	COldPolishClock();
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );

protected:
	struct CBoxCasing m_box;
	struct CWoodenTop m_top;
	struct CBackCasing m_back;
	struct CHinge m_hinge;
	struct CHooks m_hooks;
	struct CBackHooks m_back_hooks;
	struct CMiddleHooks m_middle_hooks;
	// struct CHR li;
	class CPicture m_picture;
	class CWoodenCasing m_casing;
	class CWoodenSide m_side;
	class CWoodenFront m_front;
	class CWoodenFront2 m_front2;
	class CMetallicOrnament m_metallic;
	class CMonadSetBlendState mSetBlend;
	class CGlassCasing m_glass;
	class CGlassBorderCasing m_glass2;
	class CMonadResetBlendState mResetBlend;
	class CWeightAndString m_string, m_string2;
	class CClockStringL m_stringL, m_stringL2;
	struct CBackNails m_back_nails;
	struct CSideNails m_side_nails;
	struct CWoodenNails m_wooden_nails;
	struct CPendulum m_pendulum;
	struct COval m_oval;
	class CSmallWorld : public CWorldGrCombine
	{
	public:
		CSmallWorld();
		virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );
	protected:
		class CTarcza m_tarcza;
		class CTarcza2 m_tarcza2;
		class CWskazGodz m_wsk_godz;
		class CWskazMin m_wsk_min;
		class CMetal m_montazMosiadz;
	} SmallWorld;
};

#endif