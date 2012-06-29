#pragma once
#ifndef _MONAD_C_PROTO_H_
#define _MONAD_C_PROTO_H_

//-----------------------------------------------------------------------------
// CDestructable
// Destructable root of classes
//-----------------------------------------------------------------------------
struct CDestructable abstract
{
	virtual ~CDestructable() throw() { }
};

/*//-----------------------------------------------------------------------------
// CAccessible
// Temprary ID
//-----------------------------------------------------------------------------
struct CAccessible abstract : public CDestructable
{
	ULONG_PTR GetID() const;
};

struct CImplementsResourceFile abstract
{
public:
	CImplementsResourceFile( const std::wstring & pFilename );
	virtual bool operator==(const std::wstring & other) const;
	virtual bool operator==(const CImplementsResourceFile & other) const;
	friend bool operator==(const std::wstring & other, const CImplementsResourceFile & me) { return me == other; }

protected:
	std::wstring mFilename;
};
*/

#endif // _MONAD_C_PROTO_H_