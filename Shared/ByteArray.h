#pragma once
#ifndef _MONAD_BYTE_ARRAY_H_
#define _MONAD_BYTE_ARRAY_H_

#include <boost/shared_array.hpp>
#include <atlbase.h>

// A few definitons made of templates
typedef boost::shared_array<BYTE> CAutoVectorBytePtr;
typedef CAutoVectorPtr<BYTE> CAutoSimpleVectorBytePtr;

#endif // _MONAD_BYTE_ARRAY_H_