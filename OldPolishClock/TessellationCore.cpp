#include "DXUT.h"
#include "TessellationCore.h"

CStaticTesselation::CStaticTesselation(
				   const float pWidth,
				   const float pHeight, 
				   const float pZ,
				   const float pCntHorizontal, 
				   const float pCntVertical )
				   :
	mWidth( pWidth ),
	mHeight( pHeight ),
	mCntHorizontal( pCntHorizontal ),
	mCntVertical( pCntVertical )
{

	for( float id_x = 0; id_x < pCntHorizontal; id_x ++ )
	{
		for( float id_y = 0; id_y < pCntVertical; id_y ++ )
		{
			VERTEXTEX a, b, c, d, center;
			a.Normal.x = 0.0f;
			a.Normal.y = 0.0f;
			a.Normal.z = -1.0f;
			center.Normal = b.Normal = c.Normal = d.Normal = a.Normal;
			center.Position.z = a.Position.z = b.Position.z = c.Position.z = d.Position.z = pZ;
			center.Position.x = LerpXByWidth( id_x + .5f );
			center.Position.y = LerpYByHeight( id_y + .5f );
			center.tu = LerpXOneByWidth( id_x + .5f ); center.tv = LerpYOneByHeight( id_y + 0.5f );
			a.Position.y = b.Position.y = LerpYByHeight( id_y ); a.tv = LerpYOneByHeight( id_y ); b.tv = LerpYOneByHeight( id_y );
			c.Position.y = d.Position.y = LerpYByHeight( id_y + 1.0f ); c.tv = LerpYOneByHeight( id_y + 1.0f ); d.tv = LerpYOneByHeight( id_y + 1.0f );
			a.Position.x = d.Position.x = LerpXByWidth( id_x ); a.tu = LerpXOneByWidth( id_x ); d.tu = LerpXOneByWidth( id_x );
			b.Position.x = c.Position.x = LerpXByWidth( id_x + 1); b.tu = LerpXOneByWidth( id_x + 1.0f ); c.tu = LerpXOneByWidth( id_x + 1.0f );

			// Additional vertex for better color interpolation
			Box.AddTriangle( center, a, b, true );
			Box.AddTriangle( center, b, c, true );
			Box.AddTriangle( center, c, d, true );
			Box.AddTriangle( center, d, a, true );
		}
	}

	Box.CommitMesh();
	Box.NormalizeMesh();
}

const float CStaticTesselation::LerpXByWidth( const float pFactor ) const
{
	return pFactor * mWidth / mCntHorizontal - mWidth / 2;
}
const float CStaticTesselation::LerpYByHeight( const float pFactor ) const
{
	return pFactor * mHeight / mCntVertical - mHeight / 2;
}
const float CStaticTesselation::LerpXOneByWidth( const float pFactor ) const
{
	return 1.0f - pFactor / mCntHorizontal;
}
const float CStaticTesselation::LerpYOneByHeight( const float pFactor ) const
{
	return 1.0f - pFactor / mCntVertical;
}
