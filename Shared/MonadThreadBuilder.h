#pragma once
#ifndef _MONAD_THREAD_BUILDER_H_
#define _MONAD_THREAD_BUILDER_H_

class CMonadThread abstract
{
public:
	CMonadThread() throw();
	void Run() throw();
	virtual ~CMonadThread();
	DWORD ExitCode() const throw();

protected:
	virtual unsigned Routine() = 0;

private:
	HANDLE hThread;
	DWORD dwThreadId;
	static unsigned __stdcall Wrapper( void* lpThreadParameter ) throw();
};

#endif