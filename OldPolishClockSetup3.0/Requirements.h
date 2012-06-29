#pragma once
#ifndef _MONAD_REQUIREMENTS_H_
#define _MONAD_REQUIREMENTS_H_

#include "System.h"

struct CMonadDeploymentVersion : public CMonadWindowsVersion
{
	bool Is64Bit() const throw();
	// bool IsSSE2() const throw();
	bool IsWindowsServer() const throw();

	bool IsInstalled() const throw();
	const std::wstring & InstalledMajor() const throw();
	const std::wstring & InstalledMinor() const throw();
} extern gVersion;

ostream & operator<< ( ostream &Out, const CMonadDeploymentVersion &SrcVer );

#endif