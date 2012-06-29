#pragma once
#ifndef _MONAD_FACTORY_H_
#define _MONAD_FACTORY_H_

#include "DXUT.h"
#include <boost/shared_ptr.hpp>
#include "SDKMesh.h"
#include "FileManager.h"
#include "MonadException.h"
#include "MonadThread.h"
#include "MonadEvents.h"
#include "MonadStreaming.h"
#include "MonadShader.h"

extern volatile bool bFactoryIsReady;
class CMonadFactory;
typedef CMonadAutoPtr<CDXUTSDKMesh> CModelStub;

template <class typeInternalType>
class CRoboPrototype abstract
{
protected:
	friend CMonadFactory;

	void OnCreateResource( CFileHolder & pFileHolder ) throw(...)
	{
		DXUTTRACE( L"Factory: %s %s\n", pFileHolder.GetFilename().c_str(), pFileHolder.GetExtension().c_str() );

		assert( NULL != pFileHolder.GetBuffer() );
		OnCreateDevice( pFileHolder );
		insert( pFileHolder.GetFilename(), mMember );
	}
	virtual void OnCreateDevice( CFileHolder & pFileHolder ) throw(...) = 0 ;
	const typeInternalType & GetMemberProduct() const { return mMember; }
	static typeInternalType & GetMember( const std::wstring & pFilename ) throw(...)
	{
		CMemberMap::iterator it = GetMembers().find( pFilename );
		if( GetMembers().end() == it )
		{
			THROW_EXC_ONERROR( MonadException::FileNotFound, ERROR_FILE_NOT_FOUND, ( L"Error while getting Robo Member-" + pFilename ).c_str() );
		}
		return it->second;
	}
	typedef std::map<std::wstring, typeInternalType> CMemberMap;
	static void insert( const std::wstring & pFilename, const typeInternalType & pInternalType ) throw(...)
	{
		if( !GetMembers().insert( std::make_pair( pFilename, pInternalType ) ).second )
			THROW_EXC( MonadException::ClassAlreadyExists, E_FAIL, L"Error while inserting Robo Member");
	}
	static CMemberMap & GetMembers()
	{
		static CMemberMap mMembers;
		return mMembers;
	}

	typeInternalType mMember;
};

struct CIFX sealed : public CMonadIPtrBasic<IUnknown>, private MonadEvent::COnEvent
{
	CIFX();
	ID3DXEffect * GetFX9();
	ID3DBlob * GetFX11();

protected:
	virtual void OnResetDevice();
	virtual void OnLostDevice();
};

class CRoboMesh : public CRoboPrototype<CModelStub>
{
protected:
	virtual void OnCreateDevice( CFileHolder & pFileHolder ) throw(...);
};

class CRoboTexture : public CRoboPrototype< CMonadIPtrBasic<IUnknown> >
{
protected:
	virtual void OnCreateDevice( CFileHolder & pFileHolder ) throw(...);
};

class CRoboShader : public CRoboPrototype<CIFX>
{
protected:
	virtual void OnCreateDevice( CFileHolder & pFileHolder ) throw(...);
};

class CMonadFactory
{
public:
	CMonadFactory();
	~CMonadFactory();
	// Managing methods
	virtual void OnCreateResource( CFileHolder & pFileHolder ) throw(...);

	// Resources accessors (wrappers)
	const CCue & GetCue( const std::wstring & pFilename ) const;
	IUnknown * GetTexture( const std::wstring & pFilename ) const;
	CDXUTSDKMesh * GetMesh( const std::wstring & pFilename ) const;
	const LPD3DXEFFECT GetShader( const std::wstring & pFilename ) const;
	ID3DBlob* GetShader11( const std::wstring & pFilename ) const;
	const CMonadShaderConfig & GetFXConfig( const std::wstring & pFilename ) const;
	void SetCue( const std::wstring & pFilename, const CCue & pCue );
	void SetFXConfig( const std::wstring & pFilename, const CMonadShaderConfig & pConfig );

protected:
	typedef std::map<const std::wstring, const CCue> CCueMap;
	typedef std::map<const std::wstring, const CMonadShaderConfig> CFXMap;
	static CCueMap & GetMusic();
	CFXMap mConfigs;
};

#endif