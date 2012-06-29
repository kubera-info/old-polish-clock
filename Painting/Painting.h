#pragma once
#ifndef _PAINTING_H_
#define _PAINTING_H_

#include "Manager.h"
#include "ClockFace.h"
#include "OPC-Shader.h"

enum PAINTING_POS
{
	PAINTING_LEFT = 1,
	PAINTING_RIGHT = -1
};

class CArtGeneric abstract : public CWorldGrCombine
{
protected:
	CArtGeneric( const std::wstring & pPictureFilename, const XMFLOAT4X4 & pTransform );
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );

	struct CWoodenBorder : public CTexturedShader
	{
		CWoodenBorder( const std::wstring & pBorderFilename ) : 
		  CTexturedShader ( "TBlackWood", L"wood", pBorderFilename )
		  {
		  }
	} m_border;

	CMetallic m_pipe;
	const XMFLOAT4X4 mPos;
};

class CPaintingGeneric abstract : public CArtGeneric
{
protected:
	CPaintingGeneric( const std::wstring & pPictureFilename, const XMFLOAT4X4 & pTransform );

	struct CCanvas : public CTexturedShader
	{
		CCanvas( const std::wstring & pPictureFilename, const std::wstring & pCanvasFilename ) : 
		  CTexturedShader( "TDiffuseTex", pCanvasFilename, pPictureFilename )
		  {
		  }
	} m_canvas;
};

class CDuplexGeneric abstract : public CArtGeneric
{
protected:
	CDuplexGeneric(
		const std::wstring & pPictureFilename,
		const std::wstring & pCanvasFilename,
		const XMFLOAT4X4 & pTransform );

	struct CCanvas : public CWoodenShader
	{
		CCanvas( const std::wstring & pPictureFilename, const std::wstring & pCanvasFilename ) : 
		  CWoodenShader( pPictureFilename, pCanvasFilename, g_Duplex )
		  {
		  }
	} m_canvas;
};

struct CPainting : public CPaintingGeneric
{
	CPainting( const PAINTING_POS pPos );
	XMFLOAT4X4 Init( const PAINTING_POS pPos );
};

struct CJesusPainting : public CPaintingGeneric
{
	CJesusPainting( const PAINTING_POS pPos );
	XMFLOAT4X4 Init( const PAINTING_POS pPos );
};

struct CLandscapePainting : public CPaintingGeneric
{
	CLandscapePainting( const PAINTING_POS pPos );
	XMFLOAT4X4 Init( const PAINTING_POS pPos );
};

struct CNapoleonPainting : public CPaintingGeneric
{
	CNapoleonPainting( const PAINTING_POS pPos );
	XMFLOAT4X4 Init( const PAINTING_POS pPos );
};

struct CGirl : public CTexturedShader
{
	CGirl( const PAINTING_POS pPos );
	XMFLOAT4X4 Init( const PAINTING_POS pPos );
	virtual void OnNewMatrix( XMMATRIX * pMatrixToAlter );

protected:
	const XMFLOAT4X4 mPos;
};

struct CAMPainting : public CWorldGrCombine
{
	CAMPainting( const PAINTING_POS pPos );

	struct CAdamPainting : public CDuplexGeneric
	{
		CAdamPainting( const PAINTING_POS pPos );
		XMFLOAT4X4 Init( const PAINTING_POS pPos );
	} m_adam;

	struct CMickiewiczPainting : public CDuplexGeneric
	{
		CMickiewiczPainting( const PAINTING_POS pPos );
		XMFLOAT4X4 Init( const PAINTING_POS pPos );
	} m_mickiewicz;
};
#endif