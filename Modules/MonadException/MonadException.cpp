// MonadException.cpp : Defines the exported functions for the DLL application.
//

#include "DXUT.h"
#include <string>
#include "I18N.h"
#include "MonadException.h"
#include "SingleCall.h"
#include "DictOPC.h"
#include "SingleCall.h"
#include "MonadConvert.h"
#include "MonadLock.h"
#include "MonadWindow.h"

namespace MonadException
{
	static volatile bool gExcInProgr = false;

	/*void MoveString( LPWSTR & pDest, LPWSTR & pSrc )
	{
	pDest = pSrc;
	pSrc = NULL;
	}*/

	bool ExceptionInProgress() throw()
	{
		return gExcInProgr;
	}
	HWND & GetWindowParent() throw()
	{
		static HWND hParent( NULL );

		return hParent;
	}
	void SetWindowParent( HWND pParent ) throw()
	{
		GetWindowParent() = pParent;
	}

	CGenericMonadException & GetThrown()
	{
		static CGenericMonadException gRegisterThrown;

		return gRegisterThrown;
	}

	void RegisterException( const CGenericMonadException & pThrown )
	{
		static MonadThread::CMutex OneException;
		static CSealedSingleCall bSingleCall;
		OneException.Enter();
		if( !bSingleCall )
		{
			GetThrown() = pThrown;
			if( DXUTGetHWND() )
				PostMessage( DXUTGetHWND(), WM_APP_EXIT, EXIT_FAILURE, 0 );			
		}
		OneException.Leave();
	}

	void ShowRegisteredException()
	{
		static CSealedSingleCall bOneMessage;
		if( gExcInProgr && !bOneMessage )
			GetThrown().ErrorMessageBox();
	}

	void ErrorMessageBox( LPCWSTR pMessage )
	{
		MonadMessageBox(
			GetWindowParent(), 
			pMessage,
			MonadStrings::gErrStrExceptionTitle,
			MB_ICONERROR | MB_APPLMODAL );
	}

	CGenericMonadException::CGenericMonadException(
		const LONG pMonadExceptionClassId,
		const LONG pMonadExceptionSubtypeId,
		const HRESULT p_hr,
		const WCHAR * pPlace,
		const WCHAR * pStage,
		const WCHAR * pMessage
		)
		:
	m_MonadExceptionClassId( pMonadExceptionClassId ),
		m_MonadExceptionSubtypeId( pMonadExceptionSubtypeId ),
		m_hr( p_hr ),
		m_Place( _wcsdup( pPlace ) ),
		m_Message( _wcsdup( pMessage ) ),
		m_Stage( _wcsdup( pStage ) )
	{
		gExcInProgr = TRUE;
		OutputDebugString( L"Monad exception has being thrown:\n" );
		OutputDebugString( m_Place );
		OutputDebugString( L"\n" );
		OutputDebugString( m_Message );
		OutputDebugString( L"\n" );
		OutputDebugString( m_Stage );
		OutputDebugString( L"\n" );
	}

	CGenericMonadException::CGenericMonadException( const CGenericMonadException & Exc )
	{
		*this = Exc;
	}

	CGenericMonadException::CGenericMonadException()
		:
	m_MonadExceptionClassId( -1 ),
		m_MonadExceptionSubtypeId( -1 ),
		m_hr( S_OK ),
		m_Place( NULL ),
		m_Message( NULL ),
		m_Stage( NULL )
	{
	}
	CGenericMonadException::CGenericMonadException(
		const WCHAR * pPlace,
		const char * pWhat ) :
	m_MonadExceptionClassId( MONAD_EXCEPT_SYSTEM ),
		m_MonadExceptionSubtypeId( 0 ),
		m_hr( E_FAIL ),
		m_Place( _wcsdup( pPlace ) ),
		m_Message( _wcsdup( AnsiToUnicode( pWhat ).c_str() ) ),
		m_Stage( _wcsdup( L"std exception" ) )
	{
		OutputDebugString( L"Standard exception has being thrown:\n" );
		OutputDebugString( m_Place );
		OutputDebugString( L"\n" );
		OutputDebugString( m_Message );
		OutputDebugString( L"\n" );
		OutputDebugString( m_Stage );
		OutputDebugString( L"\n" );
	}
	void CGenericMonadException::operator=( const CGenericMonadException & Exc )
	{
		m_MonadExceptionClassId = Exc.m_MonadExceptionClassId;
		m_MonadExceptionSubtypeId = Exc.m_MonadExceptionSubtypeId;
		m_hr = Exc.m_hr;

		m_Place = Exc.m_Place;
		m_Message = Exc.m_Message;
		m_Stage = Exc.m_Stage;
		//MoveString( m_Place, Exc.m_Place );
		//MoveString( m_Message, Exc.m_Message );
		//MoveString( m_Stage, Exc.m_Stage );
	}

	CGenericMonadException::~CGenericMonadException()
	{	
		/*if( m_Place )
		free( ( void * ) m_Place );
		if( m_Message )
		free( ( void * ) m_Message );
		if( m_Stage )
		free( ( void * ) m_Stage );*/

		// System will do it while application will be closed
	}

	long CGenericMonadException::GetErrorId() const
	{
		return ( m_MonadExceptionClassId << 16 ) | m_MonadExceptionSubtypeId;
	}

	HRESULT CGenericMonadException::GetHRESULT() const
	{
		return m_hr;
	}

	void CGenericMonadException::ErrorMessageBox() const
	{
		const std::wstring HR( L"\n--------------------\n" );
		std::wstring Report( m_Message );
		Report += L'\n';
		Report += HR;
#ifdef DEBUG
		Report += L"Place: ";
		Report += m_Place;
		Report += HR;
#endif
		Report += L"Operation: ";
		Report += m_Stage;
		Report += HR;
		Report += L"Class: 0x";
		Report += StringFromHRESULT( GetErrorId() );
		Report += HR;
		Report += L"Code: 0x";
		Report += StringFromHRESULT( m_hr );
		const TCHAR * szErrStr = DXGetErrorString( m_hr );
		Report += L'\n';
		Report += szErrStr;
		Report += L'\n';
		const TCHAR * szErrStrDesc = DXGetErrorDescription( m_hr );
		Report += szErrStrDesc;

		::MonadException::ErrorMessageBox( Report.c_str() );
	}
}