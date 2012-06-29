#pragma once
#ifndef _MONAD_STREAM_H_
#define _MONAD_STREAM_H_

#include <ostream>
#include <sstream>

std::ostream & operator<< ( std::ostream &Out, const WCHAR * SrcVer );

std::ostream & operator<< ( std::ostream &Out, const std::wstring &SrcVer );

std::ostream & operator<< ( std::ostream &Out, const CTime &SrcVer );

#endif // _MONAD_STREAM_H_