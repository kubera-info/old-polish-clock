#include "DXUT.h"
#include <string>
#include <exception>
#include <sstream>
#include <boost/algorithm/string.hpp>

struct CParseStream : public std::wistringstream
{
	CParseStream( const std::wstring& s )
		: std::wistringstream( s )
	{
		imbue( std::locale::classic() );
	}
	void ThrowIfFail()
	{
	    if( bad() )
			throw std::bad_cast( "bad_cast: Conversion from string" );
	}
};

struct CIntoStream : public std::wostringstream
{
	CIntoStream()
	{
		imbue( std::locale::classic() );
	}
};

float FloatFromString( const std::wstring& s )
{
	float t;
	CParseStream iss( s );
	iss >> t;
	iss.ThrowIfFail();

	return t;
}

ULONG_PTR ULongPtrFromString( const std::wstring& s )
{
	ULONG_PTR t;
	CParseStream iss( s );
	iss >> t;
	iss.ThrowIfFail();

	return t;
}

UINT UIntFromString( const std::wstring& s )
{
	UINT t;
	CParseStream iss( s );
	iss >> t;
	iss.ThrowIfFail();

	return t;
}

bool BoolFromString( const std::wstring& s )
{
	bool t;
	std::wstring tmpLower( s );
	boost::algorithm::to_lower( tmpLower );
	CParseStream iss( tmpLower );
	iss >> t;
	iss.ThrowIfFail();

	return t;
}

std::wstring StringFromHRESULT( HRESULT pHr )
{
	CIntoStream oss;
	oss << std::hex;
	oss << pHr;

	return oss.str();
}

std::wstring StringFromULongPtr( ULONG_PTR pUL )
{
	CIntoStream oss;
	oss << pUL;

	return oss.str();
}
