#pragma once
#ifndef _CLOCK_CASING_H_
#define _CLOCK_CASING_H_

#include "OPC-Shader.h"
#include "Metallic.h"

class CWoodenCasing : public CWoodenShader
{
public:
	CWoodenCasing();
};
class CWoodenFront : public CTexturedShader
{
public:
	CWoodenFront();
};
class CWoodenFront2 : public CWoodenShader
{
public:
	CWoodenFront2();
};
class CWoodenSide : public CTexturedShader
{
public:
	CWoodenSide();
};
class CGlassCasing : public CGlassShader
{
public:
	CGlassCasing();
};
class CGlassBorderCasing : public CAmbientGlassShader
{
public:
	CGlassBorderCasing();
};
struct CBackCasing : public CWoodenShader
{
public:
	CBackCasing();
};
struct CBoxCasing : public CTexturedShader
{
public:
	CBoxCasing();
};
struct CWoodenTop : public CWoodenShader
{
public:
	CWoodenTop();
};
struct CBackNails : public CMetallic
{
public:
	CBackNails();
};
struct CSideNails : public CMetallic
{
public:
	CSideNails();
};
struct CWoodenNails : public CMetallic
{
public:
	CWoodenNails();
};
/*struct CHR : public CMetallic
{
public:
	CHR();
};*/

struct CPendulum : public CMetallic
{
public:
	CPendulum();
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );
	// virtual bool IsDynamic() { return true; }
};
#endif