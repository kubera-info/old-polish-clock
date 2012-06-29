#include "DXUT.h"
#include "CasingCore.h"
#include "MonadMath.h"

static const float skala = .12f;
const float biggest = .76f;
const float biggesty = .928f;
const float to_front = .12f;
const float back_z = -0.021f;
const float NewZBack( 0.18900000 );
float NewZFront = .240;
float Org;

#define MAKE_TEX( VERT, TU, TV ) { VERT.tu = TU; VERT.tv = TV; }

static MonadMath::CEllipse El(8, skala / 2, skala /5, M_PI);

static VERTEXTEX LeftUpper, RightUpper;
static VERTEXTEX RightLower, LeftLower;
static VERTEXTEX LeftUpper2, RightUpper2;
static VERTEXTEX RightLower2, LeftLower2;

static VERTEXTEX BackLeftUpper, BackRightUpper;
static VERTEXTEX BackRightLower, BackLeftLower;
static VERTEXTEX BackLeftUpper2, BackRightUpper2;
static VERTEXTEX BackRightLower2, BackLeftLower2;

VERTEXTEX a, b, c, d;
VERTEXTEX aI, bI, cI, dI;
VERTEXTEX aFirst, bFirst, cFirst, dFirst;

void Romb( VERTEXTEX & pAlterMe )
{
	float mLaterBack = .038f;
	float mDelta = .04f;
	pAlterMe.Position.z -= mLaterBack;

	if( pAlterMe.Position.x > 0.0f )
		pAlterMe.Position.x -= mDelta;
	else
		pAlterMe.Position.x += mDelta;

	if( pAlterMe.Position.y > 0.0f )
		pAlterMe.Position.y -= mDelta;
	else
		pAlterMe.Position.y += mDelta;

}

void Border( VERTEXPLAIN & pAlterMe )
{
	float mDelta = .02f;

	if( pAlterMe.Position.x < 0.0f )
		pAlterMe.Position.x -= mDelta;
	else
		pAlterMe.Position.x += mDelta;

	if( pAlterMe.Position.y < 0.0f )
		pAlterMe.Position.y -= mDelta;
	else
		pAlterMe.Position.y += mDelta;

}

void BorderWood( VERTEXTEX & pAlterMe )
{
	float mDelta = .021f;

	if( pAlterMe.Position.x < 0.0f )
		pAlterMe.Position.x -= mDelta;
	else
		pAlterMe.Position.x += mDelta;

	if( pAlterMe.Position.y < 0.0f )
		pAlterMe.Position.y -= mDelta;
	else
		pAlterMe.Position.y += mDelta;

}

CWoodCasingMeshCreator::CWoodCasingMeshCreator()
{
	for( float iR = 0; El.GetSidesCnt() > iR; iR ++ )
	{
		float const move = skala * iR / ( El.GetSidesCnt() * 2 );
		float const move2 = skala * ( iR + 1 ) / ( El.GetSidesCnt() * 2 );

		LeftUpper.Normal = RightUpper.Normal = LeftLower.Normal = RightLower.Normal = XMFLOAT3( 0.0f, 0.0f, 1.0f );
		LeftUpper.Position.z = RightUpper.Position.z = .7f * -El[ (DWORD) iR ].y + NewZFront;
		LeftLower.Position.z = RightLower.Position.z = .7f * -El[ (DWORD) iR + 1 ].y + NewZFront;

		LeftUpper.Position.x = -biggest + move;
		RightUpper.Position.x = -LeftUpper.Position.x;
		LeftUpper.Position.y = RightUpper.Position.y = biggesty - move;

		LeftLower.Position.x = -biggest + move2;
		RightLower.Position.x = -LeftLower.Position.x;
		LeftLower.Position.y = RightLower.Position.y = biggesty - move2;

		LeftLower.tu = LeftLower.Position.x; LeftLower.tv = LeftLower.Position.y;
		LeftUpper.tu = LeftUpper.Position.x; LeftUpper.tv = LeftUpper.Position.y;
		RightLower.tu = RightLower.Position.x; RightLower.tv = RightLower.Position.y;
		RightUpper.tu = RightUpper.Position.x; RightUpper.tv = RightUpper.Position.y;

		Front.AddTriangle( LeftUpper, LeftLower, RightUpper, true );
		Front.AddTriangle( LeftLower, RightLower, RightUpper, true );

		LeftUpper2 = LeftUpper; RightUpper2 = RightUpper;
		RightLower2 = RightLower; LeftLower2 = LeftLower;

		LeftUpper2.Position.y = RightUpper2.Position.y = -RightUpper.Position.y;
		LeftLower2.Position.y = RightLower2.Position.y = -RightLower.Position.y;

		LeftLower2.tu = LeftLower2.Position.x; LeftLower2.tv = LeftLower2.Position.y;
		LeftUpper2.tu = LeftUpper2.Position.x; LeftUpper2.tv = LeftUpper2.Position.y;
		RightLower2.tu = RightLower2.Position.x; RightLower2.tv = RightLower2.Position.y;
		RightUpper2.tu = RightUpper2.Position.x; RightUpper2.tv = RightUpper2.Position.y;

		Front.AddTriangle( LeftUpper2, RightUpper2, LeftLower2, true );
		Front.AddTriangle( LeftLower2, RightUpper2, RightLower2, true );

		LeftLower.tv = LeftLower.Position.x; LeftLower.tu = LeftLower.Position.y;
		LeftUpper.tv = LeftUpper.Position.x; LeftUpper.tu = LeftUpper.Position.y;
		RightLower.tv = RightLower.Position.x; RightLower.tu = RightLower.Position.y;
		RightUpper.tv = RightUpper.Position.x; RightUpper.tu = RightUpper.Position.y;

		LeftLower2.tv = LeftLower2.Position.x; LeftLower2.tu = LeftLower2.Position.y;
		LeftUpper2.tv = LeftUpper2.Position.x; LeftUpper2.tu = LeftUpper2.Position.y;
		RightLower2.tv = RightLower2.Position.x; RightLower2.tu = RightLower2.Position.y;
		RightUpper2.tv = RightUpper2.Position.x; RightUpper2.tu = RightUpper2.Position.y;

		Front.AddTriangle( LeftUpper, LeftLower2, LeftLower, true );
		Front.AddTriangle( LeftLower2, LeftUpper, LeftUpper2, true );

		Front.AddTriangle( RightUpper, RightLower, RightLower2, true );
		Front.AddTriangle( RightLower2, RightUpper2, RightUpper, true );

		//const float depth = -0.5f;

		if( 0.0f == iR )
		{
			aFirst = LeftUpper; bFirst = RightUpper;
			cFirst = RightLower; cFirst = LeftLower;

			aFirst.Position.z = 0.12;
			bFirst.Position.z = 0.12;
			cFirst.Position.z = 0.12;
			dFirst.Position.z = 0.12;

		}
		else if( (float) El.GetSidesCnt() -1 == iR )
		{
			float make_front = 0.155f;

			LeftLower.Position.z = make_front;
			LeftLower2.Position.z = make_front;
			RightLower.Position.z = make_front;
			RightLower2.Position.z = make_front;

			BackLeftLower = LeftLower;
			BackLeftLower2 = LeftLower2;

			BackLeftLower.Position.z = back_z;
			BackLeftLower2.Position.z = back_z;

			//------------------------
			BackRightLower = RightLower;
			BackRightLower2 = RightLower2;

			BackRightLower.Position.z = back_z;
			BackRightLower2.Position.z = back_z;

			MAKE_TEX( LeftLower, 1.0f, 0.0f );
			MAKE_TEX( LeftLower2, 0.0f, 0.0f );
			MAKE_TEX( BackLeftLower, 1.0f, .4f );
			MAKE_TEX( BackLeftLower2, 0.0f, .3f );

			MAKE_TEX( RightLower, 0.0f, 0.0f );
			MAKE_TEX( RightLower2, 1.0f, 0.0f );
			MAKE_TEX( BackRightLower, 0.0f, .3f );
			MAKE_TEX( BackRightLower2, 1.0f, .5f );

			Casing.AddTriangle( LeftLower, LeftLower2, BackLeftLower2, true );
			Casing.AddTriangle( BackLeftLower2, BackLeftLower, LeftLower, true );

			Casing.AddTriangle( RightLower, BackRightLower2, RightLower2, true );
			Casing.AddTriangle( BackRightLower2, RightLower, BackRightLower, true );

			MAKE_TEX( LeftLower, 1.0f, 0.3f );
			MAKE_TEX( LeftLower2, 0.0f, 0.3f );
			MAKE_TEX( BackLeftLower, 1.0f, .6f );
			MAKE_TEX( BackLeftLower2, 0.0f, .67f );

			MAKE_TEX( RightLower, 0.0f, 0.3f );
			MAKE_TEX( RightLower2, 1.0f, 0.3f );
			MAKE_TEX( BackRightLower, 0.0f, .63f );
			MAKE_TEX( BackRightLower2, 1.0f, .74f );

			//------------------------
			Casing.AddTriangle( RightLower, LeftLower, BackLeftLower, true );
			Casing.AddTriangle( RightLower, BackLeftLower, BackRightLower, true );

			//------------------------
			Casing.AddTriangle( RightLower2, BackLeftLower2, LeftLower2, true );
			Casing.AddTriangle( RightLower2, BackRightLower2, BackLeftLower2, true );
			Org = RightLower.Position.z;

			BackLeftLower.Position.z = NewZBack;
			BackRightLower.Position.z = NewZBack;
			BackLeftLower2.Position.z = NewZBack;
			BackRightLower2.Position.z = NewZBack;

			Glass.AddTriangle( BackLeftLower, BackRightLower2, BackRightLower, true );
			Glass.AddTriangle( BackLeftLower2, BackRightLower2, BackLeftLower, true );

			{
				VERTEXPLAIN SideGlassA, SideGlassB, SideGlassC, SideGlassD;

				SideGlassA.Position.z = ( NewZBack );
				SideGlassD.Position.z = ( NewZBack );
				SideGlassB.Position.z = ( NewZBack * 0.65f + Org * 1.35f ) / 2.0f;
				SideGlassC.Position.z = ( NewZBack * 0.65f + Org * 1.35f ) / 2.0f;

				SideGlassA.Position.x = BackLeftLower.Position.x;
				SideGlassB.Position.x = BackLeftLower.Position.x;
				SideGlassC.Position.x = BackLeftLower.Position.x;
				SideGlassD.Position.x = BackLeftLower.Position.x;
				SideGlassA.Position.y = BackLeftLower.Position.y;
				SideGlassB.Position.y = BackLeftLower.Position.y;
				SideGlassC.Position.y = -BackLeftLower.Position.y;
				SideGlassD.Position.y = -BackLeftLower.Position.y;
				Border( SideGlassA );
				Border( SideGlassB );
				Border( SideGlassC );
				Border( SideGlassD );
				Glass2.AddTriangle( SideGlassA, SideGlassC, SideGlassB, true );
				Glass2.AddTriangle( SideGlassC, SideGlassA, SideGlassD, true );

				SideGlassA.Position.x = -BackLeftLower.Position.x;
				SideGlassB.Position.x = -BackLeftLower.Position.x;
				SideGlassC.Position.x = -BackLeftLower.Position.x;
				SideGlassD.Position.x = -BackLeftLower.Position.x;
				SideGlassA.Position.y = BackLeftLower.Position.y;
				SideGlassB.Position.y = BackLeftLower.Position.y;
				SideGlassC.Position.y = -BackLeftLower.Position.y;
				SideGlassD.Position.y = -BackLeftLower.Position.y;
				Border( SideGlassA );
				Border( SideGlassB );
				Border( SideGlassC );
				Border( SideGlassD );
				Glass2.AddTriangle( SideGlassA, SideGlassB, SideGlassC, true );
				Glass2.AddTriangle( SideGlassC, SideGlassD, SideGlassA, true );

				SideGlassA.Position.x = BackLeftLower.Position.x;
				SideGlassB.Position.x = BackLeftLower.Position.x;
				SideGlassC.Position.x = -BackLeftLower.Position.x;
				SideGlassD.Position.x = -BackLeftLower.Position.x;
				SideGlassA.Position.y = BackLeftLower.Position.y;
				SideGlassB.Position.y = BackLeftLower.Position.y;
				SideGlassC.Position.y = BackLeftLower.Position.y;
				SideGlassD.Position.y = BackLeftLower.Position.y;
				Border( SideGlassA );
				Border( SideGlassB );
				Border( SideGlassC );
				Border( SideGlassD );
				Glass2.AddTriangle( SideGlassA, SideGlassB, SideGlassC, true );
				Glass2.AddTriangle( SideGlassC, SideGlassD, SideGlassA, true );

				SideGlassA.Position.x = BackLeftLower.Position.x;
				SideGlassB.Position.x = BackLeftLower.Position.x;
				SideGlassC.Position.x = -BackLeftLower.Position.x;
				SideGlassD.Position.x = -BackLeftLower.Position.x;
				SideGlassA.Position.y = -BackLeftLower.Position.y;
				SideGlassB.Position.y = -BackLeftLower.Position.y;
				SideGlassC.Position.y = -BackLeftLower.Position.y;
				SideGlassD.Position.y = -BackLeftLower.Position.y;
				Border( SideGlassA );
				Border( SideGlassB );
				Border( SideGlassC );
				Border( SideGlassD );
				Glass2.AddTriangle( SideGlassA, SideGlassC, SideGlassB, true );
				Glass2.AddTriangle( SideGlassC, SideGlassA, SideGlassD, true );
			}

			{
				VERTEXTEX SideGlassA, SideGlassB, SideGlassC, SideGlassD;

				SideGlassA.Position.z = NewZBack;
				SideGlassD.Position.z = NewZBack;
				SideGlassB.Position.z = Org;
				SideGlassC.Position.z = Org;

				SideGlassA.Position.x = BackLeftLower.Position.x;
				SideGlassB.Position.x = BackLeftLower.Position.x;
				SideGlassC.Position.x = BackLeftLower.Position.x;
				SideGlassD.Position.x = BackLeftLower.Position.x;
				SideGlassA.Position.y = BackLeftLower.Position.y;
				SideGlassB.Position.y = BackLeftLower.Position.y;
				SideGlassC.Position.y = -BackLeftLower.Position.y;
				SideGlassD.Position.y = -BackLeftLower.Position.y;
				MAKE_TEX( SideGlassA, 1.0f, 0.0f );
				MAKE_TEX( SideGlassB, 0.0f, 0.0f );
				MAKE_TEX( SideGlassC, 1.0f, .5f );
				MAKE_TEX( SideGlassD, 0.0f, .55f );
				BorderWood( SideGlassA );
				BorderWood( SideGlassB );
				BorderWood( SideGlassC );
				BorderWood( SideGlassD );
				Front2.AddTriangle( SideGlassA, SideGlassC, SideGlassB, true );
				Front2.AddTriangle( SideGlassC, SideGlassA, SideGlassD, true );

				SideGlassA.Position.x = -BackLeftLower.Position.x;
				SideGlassB.Position.x = -BackLeftLower.Position.x;
				SideGlassC.Position.x = -BackLeftLower.Position.x;
				SideGlassD.Position.x = -BackLeftLower.Position.x;
				SideGlassA.Position.y = BackLeftLower.Position.y;
				SideGlassB.Position.y = BackLeftLower.Position.y;
				SideGlassC.Position.y = -BackLeftLower.Position.y;
				SideGlassD.Position.y = -BackLeftLower.Position.y;
				MAKE_TEX( SideGlassA, 1.0f, 0.0f );
				MAKE_TEX( SideGlassB, 0.0f, 0.0f );
				MAKE_TEX( SideGlassC, 1.0f, .6f );
				MAKE_TEX( SideGlassD, 0.0f, .62f );
				BorderWood( SideGlassA );
				BorderWood( SideGlassB );
				BorderWood( SideGlassC );
				BorderWood( SideGlassD );
				Front2.AddTriangle( SideGlassA, SideGlassB, SideGlassC, true );
				Front2.AddTriangle( SideGlassC, SideGlassD, SideGlassA, true );

				SideGlassA.Position.x = BackLeftLower.Position.x;
				SideGlassB.Position.x = BackLeftLower.Position.x;
				SideGlassC.Position.x = -BackLeftLower.Position.x;
				SideGlassD.Position.x = -BackLeftLower.Position.x;
				SideGlassA.Position.y = BackLeftLower.Position.y;
				SideGlassB.Position.y = BackLeftLower.Position.y;
				SideGlassC.Position.y = BackLeftLower.Position.y;
				SideGlassD.Position.y = BackLeftLower.Position.y;
				MAKE_TEX( SideGlassA, 1.0f, 0.0f );
				MAKE_TEX( SideGlassB, 0.0f, 0.0f );
				MAKE_TEX( SideGlassC, 1.0f, .43f );
				MAKE_TEX( SideGlassD, 0.0f, .47f );
				BorderWood( SideGlassA );
				BorderWood( SideGlassB );
				BorderWood( SideGlassC );
				BorderWood( SideGlassD );
				Front2.AddTriangle( SideGlassA, SideGlassB, SideGlassC, true );
				Front2.AddTriangle( SideGlassC, SideGlassD, SideGlassA, true );

				SideGlassA.Position.x = BackLeftLower.Position.x;
				SideGlassB.Position.x = BackLeftLower.Position.x;
				SideGlassC.Position.x = -BackLeftLower.Position.x;
				SideGlassD.Position.x = -BackLeftLower.Position.x;
				SideGlassA.Position.y = -BackLeftLower.Position.y;
				SideGlassB.Position.y = -BackLeftLower.Position.y;
				SideGlassC.Position.y = -BackLeftLower.Position.y;
				SideGlassD.Position.y = -BackLeftLower.Position.y;
				MAKE_TEX( SideGlassA, 1.0f, 0.0f );
				MAKE_TEX( SideGlassB, 0.0f, 0.0f );
				MAKE_TEX( SideGlassC, 1.0f, .7f );
				MAKE_TEX( SideGlassD, 0.0f, .74f );
				BorderWood( SideGlassA );
				BorderWood( SideGlassB );
				BorderWood( SideGlassC );
				BorderWood( SideGlassD );
				Front2.AddTriangle( SideGlassA, SideGlassC, SideGlassB, true );
				Front2.AddTriangle( SideGlassC, SideGlassA, SideGlassD, true );
			}

			LeftLower.Position.z = NewZFront;
			RightLower.Position.z = NewZFront;
			LeftLower2.Position.z = NewZFront;
			RightLower2.Position.z = NewZFront;

			MAKE_TEX( LeftLower, 1.0f, 0.0f );
			MAKE_TEX( LeftLower2, 0.0f, 0.0f );
			MAKE_TEX( BackLeftLower, 1.0f, .4f );
			MAKE_TEX( BackLeftLower2, 0.0f, .3f );

			MAKE_TEX( RightLower, 0.0f, 0.0f );
			MAKE_TEX( RightLower2, 1.0f, 0.0f );
			MAKE_TEX( BackRightLower, 0.0f, .3f );
			MAKE_TEX( BackRightLower2, 1.0f, .5f );

			Front.AddTriangle( LeftLower, LeftLower2, BackLeftLower2, true );
			Front.AddTriangle( BackLeftLower2, BackLeftLower, LeftLower, true );

			MAKE_TEX( LeftLower, 1.0f, 0.3f );
			MAKE_TEX( LeftLower2, 0.0f, 0.3f );
			MAKE_TEX( BackLeftLower, 1.0f, .6f );
			MAKE_TEX( BackLeftLower2, 0.0f, .67f );

			MAKE_TEX( RightLower, 0.0f, 0.3f );
			MAKE_TEX( RightLower2, 1.0f, 0.3f );
			MAKE_TEX( BackRightLower, 0.0f, .63f );
			MAKE_TEX( BackRightLower2, 1.0f, .74f );

			Front.AddTriangle( RightLower, BackRightLower2, RightLower2, true );
			Front.AddTriangle( BackRightLower2, RightLower, BackRightLower, true );

			//------------------------
			MAKE_TEX( LeftLower, 1.0f, 0.0f );
			MAKE_TEX( LeftLower2, 0.0f, 0.0f );
			MAKE_TEX( BackLeftLower, 1.0f, .11f );
			MAKE_TEX( BackLeftLower2, 0.0f, .1f );

			MAKE_TEX( RightLower, 0.0f, 0.0f );
			MAKE_TEX( RightLower2, 1.0f, 0.0f );
			MAKE_TEX( BackRightLower, 0.0f, .1f );
			MAKE_TEX( BackRightLower2, 1.0f, .1f );

			Front.AddTriangle( RightLower, LeftLower, BackLeftLower, true );
			Front.AddTriangle( RightLower, BackLeftLower, BackRightLower, true );

			//------------------------
			MAKE_TEX( LeftLower, 1.0f, 0.3f );
			MAKE_TEX( LeftLower2, 0.0f, 0.3f );
			MAKE_TEX( BackLeftLower, 1.0f, .4f );
			MAKE_TEX( BackLeftLower2, 0.0f, .42f );

			MAKE_TEX( RightLower, 0.0f, 0.3f );
			MAKE_TEX( RightLower2, 1.0f, 0.3f );
			MAKE_TEX( BackRightLower, 0.0f, .4f );
			MAKE_TEX( BackRightLower2, 1.0f, .4f );

			Front.AddTriangle( RightLower2, BackLeftLower2, LeftLower2, true );
			Front.AddTriangle( RightLower2, BackRightLower2, BackLeftLower2, true );
			//------------------------
			// Internal Border
			const float InternalDetla = 0.07f;
			aI = a = LeftLower;
			bI = b = RightLower;
			a.Position.z = aI.Position.z = b.Position.z = bI.Position.z = c.Position.z = cI.Position.z = d.Position.z = dI.Position.z = -0.021f;
			aI.Position.x = a.Position.x + InternalDetla;
			bI.Position.x = b.Position.x - InternalDetla;
			aI.Position.y = bI.Position.y = a.Position.y - InternalDetla;
			c.Position.y = d.Position.y = -a.Position.y;
			cI.Position.y = dI.Position.y = -aI.Position.y;
			c.Position.x = b.Position.x;
			d.Position.x = a.Position.x;
			cI.Position.x = bI.Position.x;
			dI.Position.x = aI.Position.x;

			MAKE_TEX( a, 0, 0.0f );
			MAKE_TEX( aI, 0, 0.13f );
			MAKE_TEX( b, 1, .0f );
			MAKE_TEX( bI, 1, .13f );

			MAKE_TEX( c, 0, 0.2f );
			MAKE_TEX( cI, 0, 0.33f );
			MAKE_TEX( d, 1, .2f );
			MAKE_TEX( dI, 1, .33f );


			Casing.AddTriangle( a, aI, b, true );
			Casing.AddTriangle( b, aI, bI, true );

			Casing.AddTriangle( c, cI, d, true );
			Casing.AddTriangle( d, cI, dI, true );

			MAKE_TEX( a, 1, 0.4f );
			MAKE_TEX( d, 0, 0.4f );
			MAKE_TEX( dI, 0, .53f );
			MAKE_TEX( aI, 1, .53f );

			MAKE_TEX( c, 1, 0.7f );
			MAKE_TEX( b, 0, 0.7f );
			MAKE_TEX( bI, 0, .93f );
			MAKE_TEX( cI, 1, .93f );

			Casing.AddTriangle( a, dI, aI, true );
			Casing.AddTriangle( dI, a, d, true );

			Casing.AddTriangle( bI, c, b, true );
			Casing.AddTriangle( c, bI, cI, true );


		}

	}

	aI = a; bI = b;
	aFirst.Position.z = Org;
	bFirst.Position.z = Org;
	cFirst.Position.z = Org;
	dFirst.Position.z = Org;
	aI.Position.z = Org;
	bI.Position.z = Org;
	cI.Position.z = Org;
	dI.Position.z = Org;
	cFirst.Position.y = dFirst.Position.y = -aFirst.Position.y;
	cI.Position.y = dI.Position.y = -aI.Position.y;
	cFirst.Position.x = bFirst.Position.x;
	dFirst.Position.x = aFirst.Position.x;
	cI.Position.x = bI.Position.x;
	dI.Position.x = aI.Position.x;

	MAKE_TEX( aFirst, 0, 0.0f );
	MAKE_TEX( aI, 0, 0.13f );
	MAKE_TEX( bFirst, 1, .0f );
	MAKE_TEX( bI, 1, .13f );

	MAKE_TEX( cFirst, 0, 0.2f );
	MAKE_TEX( cI, 0, 0.33f );
	MAKE_TEX( dFirst, 1, .2f );
	MAKE_TEX( dI, 1, .33f );

	Casing.AddTriangle( aFirst, aI, bFirst, true );
	Casing.AddTriangle( bFirst, aI, bI, true );

	Casing.AddTriangle( cFirst, cI, dFirst, true );
	Casing.AddTriangle( dFirst, cI, dI, true );

	MAKE_TEX( aFirst, 1, 0.3f );
	MAKE_TEX( dFirst, 0, 0.3f );
	MAKE_TEX( dI, 0, .43f );
	MAKE_TEX( aI, 1, .43f );

	MAKE_TEX( cFirst, 1, 0.6f );
	MAKE_TEX( bFirst, 0, 0.6f );
	MAKE_TEX( bI, 0, .83f );
	MAKE_TEX( cI, 1, .83f );

	Casing.AddTriangle( aFirst, dI, aI, true );
	Casing.AddTriangle( dI, aFirst, dFirst, true );

	Casing.AddTriangle( bI, cFirst, bFirst, true );
	Casing.AddTriangle( cFirst, bI, cI, true );

	VERTEXTEX aLast( aFirst ), bLast( bFirst ), cLast( cFirst ), dLast( dFirst );
	const float myLast = .2f;
	aLast.Position.z -= myLast;
	bLast.Position.z -= myLast;
	cLast.Position.z -= myLast;
	dLast.Position.z -= myLast;

	MAKE_TEX( bFirst, 0, 0.0f );
	MAKE_TEX( bLast, 0, .14);
	MAKE_TEX( cFirst, 1, 0.0f );
	MAKE_TEX( cLast, 1, .22 );

	MAKE_TEX( aFirst, 0, 0.2f );
	MAKE_TEX( aLast, 0, .34);
	MAKE_TEX( dFirst, 1, 0.2f );
	MAKE_TEX( dLast, 1, .32 );

	Side.AddTriangle( aFirst, aLast, dFirst, true );
	Side.AddTriangle( aLast, dLast, dFirst, true );

	Side.AddTriangle( cFirst, cLast, bFirst, true );
	Side.AddTriangle( bLast, bFirst, cLast, true );

	MAKE_TEX( aFirst, 0, 0.4f );
	MAKE_TEX( bFirst, 0, 0.5f );
	MAKE_TEX( bLast, 1, .23f );
	MAKE_TEX( aLast, 1, .03f );

	MAKE_TEX( cFirst, 0, 0.5f );
	MAKE_TEX( dFirst, 0, 0.6f );
	MAKE_TEX( dLast, 1, .33f );
	MAKE_TEX( cLast, 1, .13f );

	Side.AddTriangle( cFirst, dFirst, cLast, true );
	Side.AddTriangle( dLast, cLast, dFirst, true );

	Side.AddTriangle( aFirst, bFirst, aLast, true );
	Side.AddTriangle( bLast, aLast, bFirst, true );

	VERTEXTEX Romb_aFirst( aLast );
	VERTEXTEX Romb_bFirst( bLast );
	VERTEXTEX Romb_cFirst( cLast );
	VERTEXTEX Romb_dFirst( dLast );
	VERTEXTEX Romb_aLast( aLast );
	VERTEXTEX Romb_bLast( bLast );
	VERTEXTEX Romb_cLast( cLast );
	VERTEXTEX Romb_dLast( dLast );
	Romb( Romb_aLast );
	Romb( Romb_bLast );
	Romb( Romb_cLast );
	Romb( Romb_dLast );

	MAKE_TEX( Romb_bFirst, 0, 0.0f );
	MAKE_TEX( Romb_bLast, 0, .18);
	MAKE_TEX( Romb_cFirst, 1, 0.0f );
	MAKE_TEX( Romb_cLast, 1, .2 );

	MAKE_TEX( Romb_aFirst, 0, 0.2f );
	MAKE_TEX( Romb_aLast, 0, .30);
	MAKE_TEX( Romb_dFirst, 1, 0.2f );
	MAKE_TEX( Romb_dLast, 1, .32 );

	Side.AddTriangle( Romb_aFirst, Romb_aLast, Romb_dFirst, true );
	Side.AddTriangle( Romb_aLast, Romb_dLast, Romb_dFirst, true );

	Side.AddTriangle( Romb_cFirst, Romb_cLast, Romb_bFirst, true );
	Side.AddTriangle( Romb_bLast, Romb_bFirst, Romb_cLast, true );

	MAKE_TEX( Romb_aFirst, 0, 0.3f );
	MAKE_TEX( Romb_bFirst, 0, 0.33f );
	MAKE_TEX( Romb_bLast, 1, .23f );
	MAKE_TEX( Romb_aLast, 1, .03f );

	MAKE_TEX( Romb_cFirst, 0, 0.3f );
	MAKE_TEX( Romb_dFirst, 0, 0.4f );
	MAKE_TEX( Romb_dLast, 1, .33f );
	MAKE_TEX( Romb_cLast, 1, .13f );

	Side.AddTriangle( Romb_cFirst, Romb_dFirst, Romb_cLast, true );
	Side.AddTriangle( Romb_dLast, Romb_cLast, Romb_dFirst, true );

	Side.AddTriangle( Romb_aFirst, Romb_bFirst, Romb_aLast, true );
	Side.AddTriangle( Romb_bLast, Romb_aLast, Romb_bFirst, true );

	VERTEXTEX door_aLast( aFirst );
	VERTEXTEX door_bLast( bFirst );
	VERTEXTEX door_cLast( cFirst );
	VERTEXTEX door_dLast( dFirst );

	VERTEXTEX door_aFirst( door_aLast );
	VERTEXTEX door_bFirst( door_bLast );
	VERTEXTEX door_cFirst( door_cLast );
	VERTEXTEX door_dFirst( door_dLast );

	door_aFirst.Position.z = NewZFront;
	door_bFirst.Position.z = NewZFront;
	door_cFirst.Position.z = NewZFront;
	door_dFirst.Position.z = NewZFront;

	const float doorOut = .0053f;
	door_aLast.Position.z += doorOut;
	door_bLast.Position.z += doorOut;
	door_cLast.Position.z += doorOut;
	door_dLast.Position.z += doorOut;

	MAKE_TEX( door_bFirst, 0, 0.0f );
	MAKE_TEX( door_bLast, 0, .18);
	MAKE_TEX( door_cFirst, 1, 0.0f );
	MAKE_TEX( door_cLast, 1, .2 );

	MAKE_TEX( door_aFirst, 0, 0.2f );
	MAKE_TEX( door_aLast, 0, .30);
	MAKE_TEX( door_dFirst, 1, 0.2f );
	MAKE_TEX( door_dLast, 1, .32 );

	Front.AddTriangle( door_aFirst, door_aLast, door_dFirst, true );
	Front.AddTriangle( door_aLast, door_dLast, door_dFirst, true );

	Front.AddTriangle( door_cFirst, door_cLast, door_bFirst, true );
	Front.AddTriangle( door_bLast, door_bFirst, door_cLast, true );

	MAKE_TEX( door_aFirst, 0, 0.3f );
	MAKE_TEX( door_bFirst, 0, 0.33f );
	MAKE_TEX( door_bLast, 1, .23f );
	MAKE_TEX( door_aLast, 1, .03f );

	MAKE_TEX( door_cFirst, 0, 0.3f );
	MAKE_TEX( door_dFirst, 0, 0.4f );
	MAKE_TEX( door_dLast, 1, .33f );
	MAKE_TEX( door_cLast, 1, .13f );

	Front.AddTriangle( door_cFirst, door_dFirst, door_cLast, true );
	Front.AddTriangle( door_dLast, door_cLast, door_dFirst, true );

	Front.AddTriangle( door_aFirst, door_bFirst, door_aLast, true );
	Front.AddTriangle( door_bLast, door_aLast, door_bFirst, true );

	/*Casing.CommitMesh();
	Casing.SaveMesh( L"casing.x" );

	Glass.CommitMesh();
	Glass.SaveMesh( L"glass.x" );*/

	Glass2.CommitMesh();
	Glass2.SaveMesh( L"glass2.x" );

	//Front2.CommitMesh();
	//Front2.SaveMesh( L"front2.x" );

	/*Front.CommitMesh();
	Front.SaveMesh( L"front.x" );

	Side.CommitMesh();
	Side.SaveMesh( L"side.x" );*/
}