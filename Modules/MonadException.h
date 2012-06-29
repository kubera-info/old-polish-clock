//
// MonadException.dll
//

#pragma once
#ifndef _MONAD_EXCEPTION_H_
#define _MONAD_EXCEPTION_H_

#include "DictOPC.h"
#include "DXUT.h"
#include <exception>

namespace MonadException
{
	bool ExceptionInProgress() throw();
	void SetWindowParent( HWND pParent ) throw();

	enum CORE_EXCEPTIONS
	{
		MONAD_EXCEPT_OUTBOUND = 0,
		MONAD_EXCEPT_SYSTEM = 1,
		MONAD_EXCEPT_INTERNAL = 2,
		MONAD_EXCEPT_FILE = 3,
		MONAD_EXCEPT_XAUDIO = 4,
		MONAD_EXCEPT_DSHOW = 5,
		MONAD_EXCEPT_DIRECT3D = 6,
		MONAD_EXCEPT_DATABASE = 6,
		MONAD_EXCEPT_ATL = 8
	};

	struct CGenericMonadException
	{
	public:
		~CGenericMonadException();
		LONG GetErrorId() const;
		HRESULT GetHRESULT() const;
		void ErrorMessageBox() const;
		void operator=( const CGenericMonadException & Exc );
		CGenericMonadException( const CGenericMonadException & Exc );
		CGenericMonadException();
		explicit CGenericMonadException(
			const WCHAR * pPlace,
			const char * pWhat );

	protected:
		CGenericMonadException(
			const LONG pMonadExceptionClassId,
			const LONG pMonadExceptionSubtypeId,
			const HRESULT p_hr,
			const WCHAR * pPlace,
			const WCHAR * pStage,
			const WCHAR * pMessage
			);

		volatile LONG m_MonadExceptionClassId;
		volatile LONG m_MonadExceptionSubtypeId;
		volatile HRESULT m_hr;
		WCHAR * volatile m_Place;
		WCHAR * volatile m_Message;
		WCHAR * volatile m_Stage;
	};

	template<
		LONG_PTR MajorClassId
	>
	struct CMonadGeneralExc abstract : public CGenericMonadException
	{
	protected:
		CMonadGeneralExc(
			const LONG pMonadExceptionSubtypeId,
			const HRESULT p_hr,
			const WCHAR * pPlace,
			const WCHAR * pStage,
			const WCHAR * pMessage)
			:
		CGenericMonadException(
			MajorClassId,
			pMonadExceptionSubtypeId,
			p_hr,
			pPlace,
			pStage,
			pMessage)
		{
		}
	};

#define CREATE_MONAD_EXCEPTION_CLASS( CLASS_NAME, SUBTYPE, DEFAULT_MESSAGE, BASE_CLASS ) \
	struct CLASS_NAME : public BASE_CLASS \
	{ \
	CLASS_NAME ( \
	const HRESULT p_hr, \
	const WCHAR * pPlace, \
	const WCHAR * pStage, \
	const WCHAR * pMessage = DEFAULT_MESSAGE \
	) \
	: \
	BASE_CLASS ( \
	SUBTYPE, \
	p_hr, \
	pPlace, \
	pStage, \
	pMessage) \
	{ } \
	} \

	//
	// Macros for throwing exceptions
	//

	// Throw that class
#define CRE_EXC( MONAD_EXC_CLASS, MONAD_EXC_HRES, MONAD_EXC_STAGE ) \
	MONAD_EXC_CLASS( MONAD_EXC_HRES, (WCHAR *) TEXT(__FILE__) L"; " TEXT(__FUNCTION__), (WCHAR *) MONAD_EXC_STAGE)

#define THROW_EXC( MONAD_EXC_CLASS, MONAD_EXC_HRES, MONAD_EXC_STAGE ) \
	{ \
	throw CRE_EXC( MONAD_EXC_CLASS, MONAD_EXC_HRES, MONAD_EXC_STAGE ); \
	} \

	// Throw if zero or NULL
#define THROW_EXC_IFZ( X, MONAD_EXC_CLASS, MONAD_EXC_STAGE ) \
	{if( 0 == X ) THROW_EXC( MONAD_EXC_CLASS, E_FAIL, MONAD_EXC_STAGE ) }

	// Throw Out Of Memory Exception
#define THROW_EXC_OUTOFMEM( X, MONAD_EXC_STAGE) \
	{if( NULL == X ) THROW_EXC( MonadException::OutOfMemory, E_OUTOFMEMORY, MONAD_EXC_STAGE ) }

	// Throw if not zero
#define THROW_EXC_ONERROR( MONAD_EXC_CLASS, MONAD_ERROR_CODE, MONAD_EXC_STAGE) \
	{if( ERROR_SUCCESS != MONAD_ERROR_CODE ) THROW_EXC( MONAD_EXC_CLASS, HRESULT_FROM_WIN32( MONAD_ERROR_CODE ), MONAD_EXC_STAGE ) }

	// Throw if FAILED( HRESULT )
#define THROW_EXC_IFFAILED( MONAD_EXC_CLASS, MONAD_EXC_HRES, MONAD_EXC_STAGE) \
	{if( FAILED( MONAD_EXC_HRES ) ) THROW_EXC( MONAD_EXC_CLASS, MONAD_EXC_HRES, MONAD_EXC_STAGE ) }

	// Throw if FAILED( HRESULT )
#define THROW_EXC_D3D( MONAD_EXC_HRES, MONAD_EXC_STAGE) \
	{if( FAILED( MONAD_EXC_HRES ) ) THROW_EXC( MonadException::Driver3DFailed, MONAD_EXC_HRES, MONAD_EXC_STAGE ) }

	typedef CMonadGeneralExc<MONAD_EXCEPT_OUTBOUND> CMonadExcOutbound_;
	typedef CMonadGeneralExc<MONAD_EXCEPT_SYSTEM> CMonadExcSystem_;
	typedef CMonadGeneralExc<MONAD_EXCEPT_INTERNAL> CMonadExcInternal_;
	typedef CMonadGeneralExc<MONAD_EXCEPT_FILE> CMonadExcFile_;
	typedef CMonadGeneralExc<MONAD_EXCEPT_XAUDIO> CMonadExcAudio_;
	typedef CMonadGeneralExc<MONAD_EXCEPT_DIRECT3D> CMonadExcDirect3D_;
	typedef CMonadGeneralExc<MONAD_EXCEPT_DATABASE> CMonadExcDB_;
	typedef CMonadGeneralExc<MONAD_EXCEPT_ATL> CMonadExcATL_;

	CREATE_MONAD_EXCEPTION_CLASS(Unknown, 0, MonadStrings::gErrStrUnknownError, CMonadExcOutbound_);
	CREATE_MONAD_EXCEPTION_CLASS(NullValue, 1, L"An unexpected NULL value.", CMonadExcOutbound_);
	CREATE_MONAD_EXCEPTION_CLASS(InvalidCall, 2, L"Invalid call.", CMonadExcOutbound_);

	// Out of memory
	CREATE_MONAD_EXCEPTION_CLASS(OutOfMemory, 0, L"Out of memory", CMonadExcSystem_);
	CREATE_MONAD_EXCEPTION_CLASS(NotInit, 1, L"Called object has not been initialized.", CMonadExcSystem_);

	// File IO Error
	CREATE_MONAD_EXCEPTION_CLASS(IOError, 0, MonadStrings::gErrStrIO, CMonadExcFile_);
	CREATE_MONAD_EXCEPTION_CLASS(FileNotFound, 1, MonadStrings::gErrStrFileNotFound2, CMonadExcFile_);
	CREATE_MONAD_EXCEPTION_CLASS(FileCorrupt, 2, MonadStrings::gErrStrFileBroken2, CMonadExcFile_);
	CREATE_MONAD_EXCEPTION_CLASS(InvalidXML, 3, L"XML has invalid form", CMonadExcFile_);

	CREATE_MONAD_EXCEPTION_CLASS(SkillNotImplemented, 0, L"This class can not do it.", CMonadExcInternal_);
	CREATE_MONAD_EXCEPTION_CLASS(ClassAlreadyExists, 1, L"Overwritting has been stopped.", CMonadExcInternal_);
	CREATE_MONAD_EXCEPTION_CLASS(InvalidClass, 2, L"Can not convert class.", CMonadExcInternal_);

	// XAudio2
	CREATE_MONAD_EXCEPTION_CLASS(AudioFailed, 0, MonadStrings::gErrStrNoSound2, CMonadExcAudio_);

	// Runtime
	CREATE_MONAD_EXCEPTION_CLASS(COMInitialization, 0, L"COM Ininitalization failed.", CMonadExcSystem_);
	CREATE_MONAD_EXCEPTION_CLASS(JobInProgress, 1, L"Job already in progress.", CMonadExcSystem_);

	// Database
	CREATE_MONAD_EXCEPTION_CLASS(NoDataFound, 0, MonadStrings::gErrStrFileNotFound2, CMonadExcDB_);
	CREATE_MONAD_EXCEPTION_CLASS(DupValOnIndex, 1, L"Duplicate Value on Index", CMonadExcDB_);

	// DirectX
	CREATE_MONAD_EXCEPTION_CLASS(Driver3DFailed, 0, L"3D Subsystem Exception.\n", CMonadExcDirect3D_);
	CREATE_MONAD_EXCEPTION_CLASS(CapsNotValid, 1, MonadStrings::gErrStrNoCaps2, CMonadExcDirect3D_);

	// DirectX
	CREATE_MONAD_EXCEPTION_CLASS(ATLFailed, 0, L"Runtime problem.\n"
							 L"ATL Exception.", CMonadExcATL_);

	void RegisterException( const CGenericMonadException & pThrown );
	
	void ShowRegisteredException();
}

#endif