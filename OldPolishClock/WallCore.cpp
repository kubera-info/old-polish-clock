#include "DXUT.h"
#include "MeshCore.h"
#include "WallCore.h"
#include "OPC-Globals.h"

namespace OPCWallCore
{
	// 250 x 65 mm
	const float size_x_in_mm = 250.0f;
	const float size_y_in_mm =  65.0f;
	const float size_x = 1.0f;
	const float size_y = size_x * size_y_in_mm / size_x_in_mm;
	const float size_z = size_y / 4.0f;
}

static const LONG_PTR cnt_of_rows = 20;
static const LONG_PTR cnt_of_bricks = 14;
static const float move_z_factor = -1.0f;
static LONG_PTR idxVert;
static LONG_PTR idxHoriz;

namespace OPCWallCore
{
#ifdef DEBUG
	const float back_left = 0.0f;
	const float back_right = 1.0f;
	const float back_bottom = 0.0f;
	const float back_top = 1.0f;

	const float eps_side = 1.0f / 40.0f;
	const float eps = 1.0f / 20.0f;

	const float front_left = 0.0f + eps_side;
	const float front_right = 1.0f - eps_side;
	const float front_bottom = ( 0.0f + eps );
	const float front_top = ( 1.0f - eps );
#endif
}

#ifdef DEBUG

using namespace OPCWallCore;

CBrickMeshCreator::CBrickMeshCreator()
{

	CMeshBuilderTex Plaster;
	VERTEXTEX a, b, c, d, center;
	VERTEXTEX a_back, b_back, c_back, d_back;
	a.Normal.x = 0.0f;
	a.Normal.y = 0.0f;
	a.Normal.z = -1.0f;
	center.Normal = b.Normal = c.Normal = d.Normal = a.Normal;
	center.Position.z = a.Position.z = b.Position.z = c.Position.z = d.Position.z = 0.0f;
	center.Position.x = center.Position.y = 0.0f;
	center.tu = 0.5f; center.tv = 0.5f;
	a.Position.y = b.Position.y = -size_y; a.tv = front_top; b.tv = front_top;
	c.Position.y = d.Position.y = size_y; c.tv = front_bottom; d.tv = front_bottom;
	a.Position.x = d.Position.x = -size_x; a.tu = front_left; d.tu = front_left;
	b.Position.x = c.Position.x = size_x; b.tu = front_right; c.tu = front_right;

	a_back = a; a_back.tu = back_left; a_back.tv = back_top;
	b_back = b; b_back.tu = back_right; b_back.tv = back_top;
	c_back = c; c_back.tu = back_right; c_back.tv = back_bottom;
	d_back = d; d_back.tu = back_left; d_back.tv = back_bottom;
	a_back.Position.z = b_back.Position.z = c_back.Position.z = d_back.Position.z = - size_z;

	// Additional vertex for better color interpolation
	Plaster.AddTriangle( center, a, b, true );
	Plaster.AddTriangle( center, b, c, true );
	Plaster.AddTriangle( center, c, d, true );
	Plaster.AddTriangle( center, d, a, true );
	//Plaster.AddQuad( a, b, c, d, true );

	Plaster.AddQuad( a, a_back, b_back, b, true );
	Plaster.AddQuad( d, d_back, a_back, a, true );
	Plaster.AddQuad( b_back, c_back, c, b, true );
	Plaster.AddQuad( c, c_back, d_back, d, true );

	Plaster.CommitMesh();

	struct CInstance : public CMeshBuilderTex
	{
		void OnBrickVertex( VERTEXTEX & a, float x, float y )
		{
			XMVECTOR res = XMVector3Transform(
				XMLoadFloat3( &a.Position ),
				XMMatrixTranslation( x, y, -1.0 ) );
			XMStoreFloat3( &a.Position, res );

			if( 0.5 > x - floor( x ) )
				a.tu = ( 1.0f - a.tu );
			if( 0.5 < x - floor( x ) )
				a.tv = ( 1.0f - a.tv );
			a.tv += floor( ( ( x * y - floor( x * y ) ) * 7.0f ) + .5f );
			a.tv *= .125f;
		}
		virtual void OnParseTriangle( const VERTEXTEX & a, const VERTEXTEX & b, const VERTEXTEX & c )
		{
			VERTEXTEX newA( a ), newB( b ), newC( c );
			const float move_y_factor = float( idxVert ) / float( cnt_of_rows ) - 0.5f;
			const float move_x_factor = float( idxHoriz ) / float( cnt_of_bricks ) - 0.5f;
			const float margin_factor = .07f;
			const float scale_y_factor = ( .5f + margin_factor ) * float ( cnt_of_rows );
			const float scale_x_factor = ( 2.0f + margin_factor ) * float ( cnt_of_bricks );
			const float offset_factor = idxVert % 2 ? ( ( 2 + margin_factor ) / 2.0f ) : 0;

			const float Move_x = offset_factor + move_x_factor * scale_x_factor;
			const float Move_y = move_y_factor * scale_y_factor;

			OnBrickVertex( newA, Move_x, Move_y );
			OnBrickVertex( newB, Move_x, Move_y );
			OnBrickVertex( newC, Move_x, Move_y );

			AddTriangle( newA, newB, newC );
		}
	} mInstance;

	for( idxVert = 0; cnt_of_rows > idxVert; idxVert ++ )
		for( idxHoriz = 0; cnt_of_bricks > idxHoriz; idxHoriz ++ )
			mInstance.ParseMesh( Plaster.GetMesh() );

	mInstance.CommitMesh( false, false );
	mInstance.SaveMesh( L"brick.x" );

}

void AlterPos( VERTEXPLAIN & a )
{
	static const XMMATRIX mBack( XMMatrixScaling( 15.5f, 6.5f, 1.0f ) );
	XMVECTOR res = XMVector3Transform(
		XMLoadFloat3( &a.Position ),
		mBack );
	XMStoreFloat3( &a.Position, res );
}

CPlasterMeshCreator::CPlasterMeshCreator()
{
	const int iStepCnt = 7;
	const float Step = 2.0f / float( iStepCnt + 1 );
	float x = -1.0f;

	CMeshBuilderPlain Plaster;
	VERTEXPLAIN a, b, c, d, center;

	a.Normal.x = 0.0f;
	a.Normal.y = 0.0f;
	a.Normal.z = -1.0f;
	center.Normal = b.Normal = c.Normal = d.Normal = a.Normal;
	center.Position.z = b.Position.z = c.Position.z = d.Position.z = a.Position.z = - OPCWallCore::size_z - 1.0f;

	for( int indX = 0; indX < iStepCnt; indX ++ )
	{
		float y = -1.0f;
		for( int indY = 0; indY < iStepCnt; indY ++ )
		{
			a.Position.x = d.Position.x = x;
			b.Position.x = c.Position.x = x + Step;
			a.Position.y = b.Position.y = y;
			c.Position.y = d.Position.y = y + Step;
			center.Position.x = x + Step / 2.0f;
			center.Position.y = y + Step / 2.0f;

			VERTEXPLAIN newA( a ), newB( b ), newC( c ), newD( d ), newCenter( center );

			AlterPos( newA );
			AlterPos( newB );
			AlterPos( newC );
			AlterPos( newD );
			AlterPos( newCenter );

			Plaster.AddTriangle( newCenter, newA, newB, true );
			Plaster.AddTriangle( newCenter, newB, newC, true );
			Plaster.AddTriangle( newCenter, newC, newD, true );
			Plaster.AddTriangle( newCenter, newD, newA, true );

			y += Step;
		}
		x += Step;
	}
	Plaster.CommitMesh();
	Plaster.NormalizeMesh();
	Plaster.SaveMesh( L"square.x" );
}

#endif