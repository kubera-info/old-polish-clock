#include "DXUT.h"
#include "MonadXML.h"
#include "Singleton.h"
#include "MonadConvert.h"
#include "MonadStreaming.h"
#include "MonadTTS.h"
#include "Path.h"

static const std::wstring g_szSemantics( L"type" );
static const std::wstring g_szName( L"name" );

CXMLArrayLoader::CXMLArrayLoader( const char * pXML )
{
	iXMLDoc.CoCreateInstance(__uuidof(DOMDocument));
	VARIANT_BOOL bSuccess( 0 );
	THROW_EXC_IFFAILED( MonadException::InvalidXML, iXMLDoc->loadXML( CComBSTR( pXML ), &bSuccess ), L"Loading XML" );
	THROW_EXC_IFZ( bSuccess, MonadException::NullValue, L"Failed formal the XML test" );
	THROW_EXC_IFFAILED( MonadException::InvalidXML, iXMLDoc->get_documentElement(&iRootElm), L"No root in XML" );

	TElem eroot(iRootElm);

#ifdef DEBUG
	if( eroot.name() != L"opc" )
		THROW_EXC( MonadException::InvalidXML, E_FAIL, L"Not XML of main" );
#endif
	for ( TElem c=eroot.begin(); c!=eroot.end(); c++ ) 
	{
		const std::wstring szFilename( c.attr( g_szName ) );

		if( c.name() == L"cue" )
		{
			const CCue NewCue(
				FloatFromString( c.attr( L"bpm" ) ),
				FloatFromString( c.attr( L"bar" ) ),
				FloatFromString( c.attr( L"phrase" ) ) );
			
			Singleton->MonadFactory.SetCue( szFilename, NewCue );
			CWaveStreaming::AddStream( szFilename );
#ifdef DEBUG
					const std::wstring dx11 = L"Res\\Stream\\";
					const std::wstring ext = L".monad-strm";
					const std::wstring path = ( GetRootPath() + dx11 + szFilename + ext );
					assert( PathFileExists( path.c_str() ) );
#endif
		}
		else if( c.name() == L"time" )
		{
			for( TElem lang=c.begin(); lang!=c.end(); lang++ ) 
			{
				const std::wstring Lang( lang.name() );
				const ULONG_PTR UL_Name( ULongPtrFromString( szFilename ) );
				for( TElem strm=lang.begin(); strm!=lang.end(); strm++ )
				{
#ifdef DEBUG
					const std::wstring dx11 = L"Res\\Stream\\";
					const std::wstring ext2 = L".monad-snd";
					const std::wstring path2 = ( GetRootPath() + dx11 + strm.val() + ext2 );
					assert( PathFileExists( path2.c_str() ) );
#endif
					PushTTSDefinition( Lang, UL_Name, strm.val() );
				}
			}
		}
		else if( c.name() == L"technique" )
		{
#ifdef DEBUG
			const std::wstring dx11 = L"Res\\DX11\\";
			const std::wstring ext = L".monad-fx";
			const std::wstring path = ( GetRootPath() + dx11 + c.attr( L"vs" ) + ext );
			const std::wstring path2 = ( GetRootPath() + dx11 + c.attr( L"ps" ) + ext );
			assert( PathFileExists( path.c_str() ) );
			assert( PathFileExists( path2.c_str() ) );
#endif
			CMonadShaderConfig NewConfig( c.attr( L"vs" ), c.attr( L"ps" ) );
			Singleton->MonadFactory.SetFXConfig( szFilename, NewConfig );
		}
#ifdef DEBUG
		else
			THROW_EXC( MonadException::InvalidXML, E_FAIL, L"Unknown type in XML" );
#endif
	}
}