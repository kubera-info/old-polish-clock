#include "DXUT.h"
#include "MonadCatch.h"
#include "MonadWindow.h"
#include "MonadMonitor.h"
#include "MonadException.h"
#include <boost/foreach.hpp>
#include "MonadConvert.h"
#include "DeviceLevel.h"

const WCHAR * SAVERWNDCLASS = L"MonadWindowClass3_0";
static const WCHAR * SATELLITEWNDCLASS = L"MonadSatWindowClass3_0";
static const FLOAT DEFAULT_DPI = 96.f;

bool m_bWaitForInputIdle;

volatile HBITMAP hbmp( NULL );

static void RenderWall( HWND hWnd )
{
	HDC hdc;                 // device context (DC) for window

	// Create a device context (DC) to hold the bitmap.  
	// The bitmap is copied from this DC to the window's DC  
	// whenever it must be drawn.  

	HDC hdcCompat;    // DC for copying bitmap
	hdc = GetDC( hWnd ); 
	hdcCompat = CreateCompatibleDC( hdc ); 
	ReleaseDC( hWnd, hdc );
	SelectObject( hdcCompat, hbmp );
	PAINTSTRUCT ps;          // paint data for BeginPaint and EndPaint
	if( BeginPaint( hWnd, &ps ) )
	{
		RECT mCurrRect;
		GetClientRect( hWnd, &mCurrRect );
		StretchBlt(
			ps.hdc,
			0,
			0,
			mCurrRect.right,
			mCurrRect.bottom,
			hdcCompat,
			0,
			0,
			1366,
			768,
			SRCCOPY );
		EndPaint( hWnd, &ps );
	}
	DeleteDC(hdcCompat);
}

CWindowClass::CWindowClass( const std::wstring & pClassName )
: m_szClassName( pClassName )
{
}
void CWindowClass::RegisterClass( HINSTANCE hInstance )
{
	WNDCLASS Class;
	ZeroMemory( &Class, sizeof( Class ) );
	Class.lpszClassName = m_szClassName.c_str();
	Class.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_MAIN_ICON ) );
	FillClass( hInstance, Class );
	THROW_EXC_IFZ( ::RegisterClass( &Class ), MonadException::InvalidCall, L"RegisterClass" );
}
CWindowClass::operator LPCWSTR () const throw()
{
	return m_szClassName.c_str();
}
CWindowClass::~CWindowClass()
{
	UnregisterClass( m_szClassName.c_str(), NULL );
}

LRESULT CALLBACK MonadStaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_CLOSE:
		{
			DestroyWindow( hWnd );
			break;
		}
	case WM_USER :
		{
			// All initialization messages have gone through.  Allow
			// 500ms of idle time, then proceed with initialization.
			SetTimer( hWnd, 1, 500, NULL );
			break;
		}
	case WM_TIMER :
		{
			// Initial idle time is done, proceed with initialization.
			m_bWaitForInputIdle = false;
			KillTimer( hWnd, 1 );
			break;
		}
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}
static LRESULT CALLBACK MonadStaticWndProc2( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_SETCURSOR:
		{ 
			SetCursor( NULL ); 
			break;
		}
	case WM_SIZE:
		{
			if( DXUT_D3D11_DEVICE == DeviceLevel )
			{
				CSatellite * pThis = reinterpret_cast<CSatellite *> (GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if (pThis->m_pRT)
				{
					D2D1_SIZE_U size = D2D1::SizeU(LOWORD(lParam), HIWORD(lParam));

					// If we couldn't resize, release the device and we'll recreate it
					// during the next render pass.
					if (FAILED(pThis->m_pRT->Resize(size)))
					{
						pThis->m_pRT.Release();
						pThis->m_pD2DBitmap.Release();
					}
				}
			}
			break;
		}
	case WM_PAINT:
		{
			if( DXUT_D3D11_DEVICE == DeviceLevel )
			{
				CSatellite * pThis = reinterpret_cast<CSatellite *> (GetWindowLongPtr(hWnd, GWLP_USERDATA));
				pThis->OnPaint( hWnd );
			}
			else
				RenderWall( hWnd );
			break;
		}
	case WM_LBUTTONDOWN:;
	case WM_LBUTTONUP:;
	case WM_LBUTTONDBLCLK:;
	case WM_MBUTTONDOWN:;
	case WM_MBUTTONUP:;
	case WM_MBUTTONDBLCLK:;
	case WM_RBUTTONDOWN:;
	case WM_RBUTTONUP:;
	case WM_RBUTTONDBLCLK:;
	case WM_XBUTTONDOWN:;
	case WM_XBUTTONUP:;
	case WM_XBUTTONDBLCLK:;
	case WM_MOUSEWHEEL:
		{
			PostMessage( DXUTGetHWND(), WM_APP_EXIT, 0, 0 );
			break;
		}
	case WM_CLOSE:
		{
			DestroyWindow( hWnd );
			break;
		}
	case WM_NCCREATE :
		{
			LPCREATESTRUCT pcs = reinterpret_cast<LPCREATESTRUCT> ( lParam );
			CSatellite * pThis = reinterpret_cast<CSatellite *> ( pcs->lpCreateParams );

			SetWindowLongPtr( hWnd, GWLP_USERDATA, PtrToUlong( pThis ) );

			// no break
		}
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}


CSatelliteWindowClass::CSatelliteWindowClass()
: CWindowClass( SATELLITEWNDCLASS )
{
}
void CSatelliteWindowClass::FillClass( HINSTANCE hInstance, WNDCLASS & pRegisterMe )
{
	// Register the windows class
	pRegisterMe.lpfnWndProc = MonadStaticWndProc2;
	pRegisterMe.hInstance = hInstance;
	pRegisterMe.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
}
CPreviewWindowClass::CPreviewWindowClass() : CWindowClass( SAVERWNDCLASS ) {}
void CPreviewWindowClass::FillClass( HINSTANCE hInstance, WNDCLASS & pRegisterMe )
{
	pRegisterMe.hCursor        = LoadCursor( NULL, IDC_ARROW );
	pRegisterMe.hbrBackground  = (HBRUSH) GetStockObject(BLACK_BRUSH);
	pRegisterMe.hInstance      = hInstance; 
	pRegisterMe.lpfnWndProc    = MonadStaticWndProc;
}
CFullWindowClass::CFullWindowClass() : CWindowClass( SAVERWNDCLASS ) {}
void CFullWindowClass::FillClass( HINSTANCE hInstance, WNDCLASS & pRegisterMe )
{
	pRegisterMe.lpfnWndProc = DXUTStaticWndProc;
	pRegisterMe.hInstance = hInstance;
	pRegisterMe.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
}

CSatellite::CSatellite( HINSTANCE hInstance, RECT pMonitorRect, const CSatelliteManager & pMyMan )
: m_hWnd( NULL ), m_Rect( pMonitorRect ), m_hInstance( hInstance ), refMyMan( pMyMan )
{
}

unsigned CSatellite::Routine()
{
	try
	{
		m_hWnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, SATELLITEWNDCLASS, GetOPC().c_str(), WS_MAXIMIZE | WS_POPUP | WS_VISIBLE /* WS_THICKFRAME */,
			m_Rect.left, m_Rect.top, ( m_Rect.right - m_Rect.left ), ( m_Rect.bottom - m_Rect.top ), 0,
			NULL, m_hInstance, this );

		THROW_EXC_IFZ( m_hWnd, MonadException::NullValue, L"Cannot Create Satellite" );

		MSG msg;
		msg.message = WM_NULL;
		do
		{
			if( GetMessage( &msg, NULL, 0, 0 ) > 0 )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		while ( WM_CLOSE != msg.message );

		return ERROR_SUCCESS;
	}
	CATCH;

	return EXIT_FAILURE;
}
const RECT & CSatellite::GetRect() const throw()
{
	return m_Rect;
}
HRESULT CSatellite::CreateDeviceResources(HWND hWnd)
{
	HRESULT hr = S_OK;

	if (!m_pRT)
	{
		if (SUCCEEDED(hr))
		{
			// Create a D2D render target properties
			D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();

			// Set the DPI to be the default system DPI to allow direct mapping
			// between image pixels and desktop pixels in different system DPI settings
			renderTargetProperties.dpiX = DEFAULT_DPI;
			renderTargetProperties.dpiY = DEFAULT_DPI;

			// Create a D2D render target
			D2D1_SIZE_U size = D2D1::SizeU(GetRect().right - GetRect().left, GetRect().bottom - GetRect().top);

			hr = refMyMan.m_pD2DFactory->CreateHwndRenderTarget(
				renderTargetProperties,
				D2D1::HwndRenderTargetProperties(hWnd, size),
				&m_pRT
				);
		}
	}

	return hr;
}

void CSatellite::OnPaint(HWND hWnd)
{
	HRESULT hr = S_OK;
	PAINTSTRUCT ps;

	if (BeginPaint(hWnd, &ps))
	{
		// Create render target if not yet created
		hr = CreateDeviceResources(hWnd);

		if (SUCCEEDED(hr) && !(m_pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
		{
			m_pRT->BeginDraw();

			m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

			// Clear the background
			// m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

			D2D1_SIZE_F rtSize = m_pRT->GetSize();

			// Create a rectangle same size of current window
			D2D1_RECT_F rectangle = D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height);

			// D2DBitmap may have been released due to device loss. 
			// If so, re-create it from the source bitmap
			if (refMyMan.mWICBitmap && !m_pD2DBitmap)
			{
				m_pRT->CreateBitmapFromWicBitmap(refMyMan.mWICBitmap, NULL, &m_pD2DBitmap);
			}

			// Draws an image and scales it to the current window size
			if (m_pD2DBitmap)
			{
				m_pRT->DrawBitmap(m_pD2DBitmap, rectangle);
			}

			hr = m_pRT->EndDraw();

			// In case of device loss, discard D2D render target and D2DBitmap
			// They will be re-create in the next rendering pass
			if (hr == D2DERR_RECREATE_TARGET)
			{
				m_pD2DBitmap.Release();
				m_pRT.Release();
				// Force a re-render
				hr = InvalidateRect(hWnd, NULL, TRUE)? S_OK : E_FAIL;
			}
		}

		EndPaint(hWnd, &ps);
	}
}  

CSatellite::~CSatellite()
{
	PostMessage( m_hWnd, WM_CLOSE, 0, 0 );
}

CSatelliteManager::CSatelliteManager()
{
	const CMonitors Monitors( GetNonPrimaryMonitors() );
	if( DXUT_D3D11_DEVICE == DeviceLevel && !Monitors.empty() )
	{
		HRESULT hr;
		// Create WIC factory
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_pIWICFactory)
			);

		if (SUCCEEDED(hr))
		{
			// Create D2D factory
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_pD2DFactory);
		}

		if (SUCCEEDED(hr))
		{
			m_pIWICFactory->CreateBitmapFromHBITMAP(
				hbmp,
				NULL,
				WICBitmapIgnoreAlpha,
				&mWICBitmap
				);
		}
		THROW_EXC_IFFAILED( MonadException::Unknown, hr, L"Bricks 11" );
	}

	HINSTANCE hInstance( GetModuleHandle( NULL ) );
	CSatellites TemporarySatellite, ToDelete;

	BOOST_FOREACH( const RECT & rc, Monitors )
		TemporarySatellite.insert( CAutoSatellite ( new CSatellite( hInstance, rc, *this ) ) );

	std::set_difference(
		mSatellites.begin(),
		mSatellites.end(),
		TemporarySatellite.begin(),
		TemporarySatellite.end(),
		std::inserter( ToDelete, ToDelete.end() ) );

	BOOST_FOREACH( const CAutoSatellite & Satellite, ToDelete )
		mSatellites.erase( Satellite );

	BOOST_FOREACH( const CAutoSatellite & Satellite, TemporarySatellite )
		if( mSatellites.insert( Satellite ).second )
			Satellite.StartWindowLoop();
}
bool CSatelliteManager::CAutoSatellite::operator<( const CAutoSatellite & pRightFormat ) const throw()
{
	return *get() < *pRightFormat.get();
}
void CSatelliteManager::CAutoSatellite::StartWindowLoop() const
{
	get()->Run();
}
