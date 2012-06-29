#include "DXUT.h"
#include "NutCore.h"
#include "MonadMath.h"
#include "Singleton.h"

/*// Import from previous version
static DWORD g_wJakoscGr = /*1000* / 800;
#define PRZEL(x) ((DWORD) ((float) (x) * g_wJakoscGr / 1000))
#define WYCOFANIE		(.20f)
#define STOZEK_Z        (WYCOFANIE * 1.50f)
#define STOZEK_TYL      (WYCOFANIE * 1.33f)
#define WSP_KATA (0.22f)
#define WSP_KATA_SRUBY (0.55f)

typedef 
struct 
{
	float maly_r;
	float wiekszy_r;
	float duzy_r;
	float gwint_r; // Najwiêkszy promieñ œruby
	float koniec_r; // Promieñ przeciêcia sto¿ka
}
proporcje_nakretki;

CNutMeshCreator::CNutMeshCreator()
{
	mNut.CloneFromMesh( Singleton->MonadFactory.GetMesh( L"brass" ).mModel );

	proporcje_nakretki m_rozm;
	m_rozm.duzy_r = 0.08f;
	m_rozm.wiekszy_r = 0.075f;
	m_rozm.maly_r = 0.05f;
	m_rozm.gwint_r = 0.027f;
	m_rozm.koniec_r = 0.0139f;
	{
		MonadMath::CEllipse m_okrag(6, 1.0f, NULL, -M_PI_MUL_2, 0.73f);

		for( DWORD iSide = 0; iSide < m_okrag.GetSidesCnt(); ++ iSide )
		{

			const float x1 = m_okrag[iSide].x * m_rozm.maly_r;
			const float y1 = m_okrag[iSide].y * m_rozm.maly_r;
			const float x2 = m_okrag[( iSide + 1 ) % m_okrag.GetSidesCnt() ].x * m_rozm.maly_r;
			const float y2 = m_okrag[( iSide + 1 ) % m_okrag.GetSidesCnt() ].y * m_rozm.maly_r;

			VERTEXTEX a, b, c, d;
			a.Position.x = b.Position.x = x1;
			c.Position.x = d.Position.x = x2;
			a.Position.y = b.Position.y = y1;
			c.Position.y = d.Position.y = y2;
			a.Position.z = d.Position.z = STOZEK_Z + .03f;
			b.Position.z = c.Position.z = STOZEK_TYL + .03f;
			a.tu = b.tu = 0.0f;
			c.tu = d.tu = 0.7f;
			a.tv = c.tv = 0.4f;
			b.tv = d.tv = 1.0f;

			mNut.AddTriangle( a, b, c, true );
			mNut.AddTriangle( c, d, a, true );
		}

	}
	{
		MonadMath::CEllipse m_okrag(36 << 1, 1.0f);

		for(DWORD m_ind = 0; m_ind < m_okrag.GetSidesCnt(); m_ind ++)
		{
			const float m_r = (m_ind & 1 ? m_rozm.duzy_r : m_rozm.wiekszy_r);
			const float m_z = - m_r * WSP_KATA + STOZEK_Z;

			const float m_r2 = ( ( m_ind + 1 ) & 1 ? m_rozm.duzy_r : m_rozm.wiekszy_r);
			const float m_z2 = - m_r2 * WSP_KATA + STOZEK_Z;

			const float x1 = m_okrag[ m_ind ].x * m_r;
			const float y1 = m_okrag[ m_ind ].y * m_r;

			const float x2 = m_okrag[( m_ind + 1 ) % m_okrag.GetSidesCnt() ].x * m_r2;
			const float y2 = m_okrag[( m_ind + 1 ) % m_okrag.GetSidesCnt() ].y * m_r2;

			const float z = min( m_z, m_z2 ) - .016f;

			VERTEXTEX a, b, c, d;
			a.Position.x = b.Position.x = x1;
			a.Position.z = m_z;
			b.Position.z = z;
			c.Position.x = d.Position.x = x2;
			a.Position.y = b.Position.y = y1;
			c.Position.y = d.Position.y = y2;
			c.Position.z = z;
			d.Position.z = m_z2;
			a.tu = b.tu = 0.1f;
			c.tu = d.tu = 0.3f;
			a.tv = c.tv = 0.3f;
			b.tv = d.tv = 7.0f;

			mNut.AddTriangle( a, c, b, true );
			mNut.AddTriangle( c, a, d, true );
		}
	}
	mNut.CommitMesh();
	mNut.SaveMesh( L"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\C++\\Direct3D\\Edu\\Res\\Auto\\brass2.mesh" );
}

CSteelMeshCreator::CSteelMeshCreator()
{
	mSteel.CloneFromMesh( Singleton->MonadFactory.GetMesh( L"steel" ).mModel );

	proporcje_nakretki m_rozm;
	m_rozm.duzy_r = 0.08f;
	m_rozm.wiekszy_r = 0.075f;
	m_rozm.maly_r = 0.05f;
	m_rozm.gwint_r = 0.027f;
	m_rozm.koniec_r = 0.0139f;

	MonadMath::CEllipse m_okrag(PRZEL(48), 1.0f);

	for(DWORD m_ind = 0; m_ind < m_okrag.GetSidesCnt(); m_ind ++)
	{
			const float x1 = m_okrag[m_ind].x * m_rozm.gwint_r;
			const float y1 = m_okrag[m_ind].y * m_rozm.gwint_r;
			const float x2 = m_okrag[( m_ind + 1 ) % m_okrag.GetSidesCnt() ].x * m_rozm.gwint_r;
			const float y2 = m_okrag[( m_ind + 1 ) % m_okrag.GetSidesCnt() ].y * m_rozm.gwint_r;
			static float m_z = - m_rozm.gwint_r * WSP_KATA_SRUBY + STOZEK_Z + 0.07f;

			VERTEXTEX a, b, c, d;
			a.Position.x = b.Position.x = x1;
			c.Position.x = d.Position.x = x2;
			a.Position.y = b.Position.y = y1;
			c.Position.y = d.Position.y = y2;
			a.Position.z = d.Position.z = m_z;
			b.Position.z = c.Position.z = m_z - .25f;
			a.tu = b.tu = 0.0f;
			c.tu = d.tu = 0.0f;
			a.tv = c.tv = 1.0f;
			b.tv = d.tv = 1.0f;

			mSteel.AddTriangle( a, c, b, true );
			mSteel.AddTriangle( c, a, d, true );
	}

	mSteel.CommitMesh();
	mSteel.SaveMesh( L"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Samples\\C++\\Direct3D\\Edu\\Res\\Auto\\steel2.mesh" );
}
*/