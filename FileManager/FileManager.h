#pragma once
#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include "DXUT.h"
#include "MonadThread.h"
#include "MonadTemplates.h"
#include <atlbase.h>

// extern const std::wstring szExtSnd;
extern const std::wstring szExtStrm;
extern const std::wstring szExtFx;
extern const std::wstring szExtMesh;
// extern const std::wstring szExtDb;
extern const std::wstring szExtTex;

DWORD GetResFileSize( const std::wstring & pPath );

struct CPaths : public CStringArray
{
	void add_path( const std::wstring & pFolder );
};

class CFileSearcher
{
public:
	CFileSearcher & operator=( const CFileSearcher & Source ); // forbiddens
	CFileSearcher();
	CFileSearcher( const CFileSearcher & Source ); // forbidden
	~CFileSearcher();
	inline void UnregisterTreasure() throw();
	void Run( const CStringArray & pSearchPaths ) throw(...);
	bool IsFinished() /* volatile */ const throw();

protected:
	inline void RegisterTreasure() throw();
	void AddOneFileToList( const std::wstring & pFolder, const WIN32_FIND_DATA & pFindFileData );
	/* volatile */ LONG_PTR mCntOfFound;
	MonadThread::CThreadPoolManager mThreadPool;
};

//
// class CFileHolder
// The class loads and holds a binary file
//
class CFileHolder : public MonadThread::CRunnableFromQueue
{
public:
	CFileHolder & operator=( const CFileHolder & Source );
	CFileHolder( const CFileHolder & Source );
	// CFileHolder( const std::wstring & pFolder, const std::wstring & pFilename );
	virtual ~CFileHolder();
	const std::wstring GetPath() const; // Full path
	const std::wstring GetFilename() const; // The filename without extension
	const std::wstring GetExtension() const; // A file extension without the dot
	const DWORD GetSize() const; // The size of this file
	BYTE * GetBuffer() const; // The buffer of file, that has ASCII(0) concatenated (for ASCII and UNICODE files)
	// The processing of thread
	virtual void ProcessQueue() throw(...);
	virtual void Run() throw(...); // Loads declared file into the virtual memory

	void DetachFileHolder();
	void ReleaseFileHolder();

protected:
	friend CFileSearcher;
	CFileHolder(); 
	CFileHolder( const std::wstring & pFolder, const std::wstring & pFilename, const DWORD pFileSize );

	volatile const WCHAR * volatile mPath;
	volatile DWORD mFileSize;
	volatile BYTE * volatile mBuffer; // A file in memory
};

extern CAutoPtr<CFileSearcher> g_pFileManager;

#endif