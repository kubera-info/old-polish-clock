#pragma once
#ifndef _MONAD_WINDOW_H_
#define _MONAD_WINDOW_H_

#include <windows.h>
#include <wincodec.h>
#include <atlbase.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <boost/shared_ptr.hpp>
#include "MonadException.h"
#include "MonadTemplates.h"
#include "MonadThreadBuilder.h"
#include "resource.h"
#include "MonadCom.h"
#include "define.h"

extern bool m_bWaitForInputIdle;

extern const WCHAR * SAVERWNDCLASS;

extern volatile HBITMAP hbmp;

LRESULT CALLBACK MonadStaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

struct CWindowClass abstract : boost::noncopyable
{
	CWindowClass( const std::wstring & pClassName );
	void RegisterClass( HINSTANCE hInstance );
	virtual void FillClass( HINSTANCE hInstance, WNDCLASS & pRegisterMe ) = 0; // Do not alter the class name
	operator LPCWSTR () const throw();
	~CWindowClass();

private:
	const std::wstring m_szClassName;
};

struct CSatelliteWindowClass : CWindowClass
{
	CSatelliteWindowClass();
	virtual void FillClass( HINSTANCE hInstance, WNDCLASS & pRegisterMe );
};
struct CPreviewWindowClass : public CWindowClass
{
	CPreviewWindowClass();
	virtual void FillClass( HINSTANCE hInstance, WNDCLASS & pRegisterMe );
};
struct CFullWindowClass : public CWindowClass
{
	CFullWindowClass();
	virtual void FillClass( HINSTANCE hInstance, WNDCLASS & pRegisterMe );
};

struct CSatelliteManager;
struct CSatellite sealed : public CMonadThread, boost::noncopyable
{
	CSatellite( HINSTANCE hInstance, RECT pMonitorRect, const CSatelliteManager & pMyMan );
	~CSatellite();
	virtual unsigned Routine();
	const RECT & GetRect() const throw();
	bool operator<( const CSatellite & pRightFormat ) const throw()
	{
		return GetRect() < pRightFormat.GetRect();
	}

	void OnPaint(HWND hWnd);
	HRESULT CreateDeviceResources(HWND hWnd);
    CComPtr<ID2D1HwndRenderTarget>  m_pRT;
    CComPtr<ID2D1Bitmap>            m_pD2DBitmap;

private:
	volatile HWND m_hWnd;
	const RECT m_Rect;
	volatile HINSTANCE m_hInstance;

	const CSatelliteManager & refMyMan;   
};

struct CSatelliteManager : boost::noncopyable
{
	CSatelliteManager();
    CComPtr<ID2D1Factory>           m_pD2DFactory;
	CComPtr<IWICBitmap> mWICBitmap;

private:
	CCoInitialize mComInit;
    CComPtr<IWICImagingFactory>		m_pIWICFactory;	
	struct CAutoSatellite sealed : private boost::shared_ptr<CSatellite>
	{
		explicit CAutoSatellite( CSatellite * pSatellite ) : boost::shared_ptr<CSatellite>( pSatellite ) {}
		bool operator<( const CAutoSatellite & pRightFormat ) const throw();
		bool operator==( const CAutoSatellite & pRightFormat ) throw(); // forbidden
		void StartWindowLoop() const;
	};
	typedef std::set<CAutoSatellite> CSatellites;
	CSatellites mSatellites;
};

#endif // _MONAD_WINDOW_H_