#pragma once
#ifndef _OLD_POLISH_CLOCK_WALL_H_
#define _OLD_POLISH_CLOCK_WALL_H_

#include "Manager.h"
#include "OPC-Shader.h"

class CBrickWall : public CWorldGrCombine
{
public:
	CBrickWall() {
		AddRenderedObject( &m_bricksFX );
		AddRenderedObject( &m_plaster );
	}

protected:
	class CBrickFX : public CTexturedShader
	{
	public:
		CBrickFX();
	} m_bricksFX;
	class CPlaster : public CMaterialShader
	{
	public:
		CPlaster();
	} m_plaster;
};

struct CWoodenMount : public CWoodenShader
{
	CWoodenMount();
};
#endif