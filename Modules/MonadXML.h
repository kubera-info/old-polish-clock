#pragma once
#ifndef _MONAD_XML_H_
#define _MONAD_XML_H_

#include <windows.h>
#include <msxml.h>
#include <objsafe.h>
#include <objbase.h>
#include <atlbase.h>
#include <string>
#include "MonadCOM.h"
#include "MonadException.h"

struct CXMLArrayLoader
{
	CXMLArrayLoader( const char * pXML );

protected:
	std::wstring GetValue( const std::wstring & pID, const std::wstring & pColumnName );
	CCoInitialize mComInit;
	struct TElem
	{
		CComPtr<IXMLDOMElement> elem;
		CComPtr<IXMLDOMNodeList> nlist;
		int pos;
		long clen;
		//
		TElem() : elem(0), nlist(0), pos(-1), clen(0) {}
		TElem(int _clen) : elem(0), nlist(0), pos(-1), clen(_clen) {}
		TElem(CComPtr<IXMLDOMElement> _elem) : elem(_elem), nlist(0), pos(-1), clen(0) {get();}
		TElem(CComPtr<IXMLDOMNodeList> _nlist) : elem(0), nlist(_nlist), pos(0), clen(0) {get();}
		void get()
		{ 
			if (pos!=-1)
			{ 
				elem=0;
				CComPtr<IXMLDOMNode> inode;
				nlist->get_item(pos,&inode);
				if (inode==0)
					return;
				DOMNodeType type; inode->get_nodeType(&type);
				if (type!=NODE_ELEMENT) 
					return;
				CComQIPtr<IXMLDOMElement> e(inode);
				elem=e;
			}

			clen=0; 
			if (elem!=0)
			{ 
				CComPtr<IXMLDOMNodeList> iNodeList;
				elem->get_childNodes(&iNodeList);
				iNodeList->get_length(&clen);  
			}
		}
		std::wstring name() const
		{
			if (!elem) 
				return L"";
			CComBSTR bn;
			THROW_EXC_IFFAILED( MonadException::InvalidXML, elem->get_tagName(&bn), L"Error in XML name" );
			return std::wstring( ( LPCWSTR ) bn );
		}
		std::wstring attr(const std::wstring & name) const
		{
			if (!elem) 
				return GetEmpty();
			CComBSTR bname( name.c_str() );
			CComVariant val(VT_EMPTY);
			elem->getAttribute(bname,&val);
			if (val.vt==VT_BSTR)
				return val.bstrVal;
			THROW_EXC( MonadException::InvalidXML, E_FAIL, L"Error in XML: No such attr" );
		}
		const std::wstring val() const
		{ 
			if (!elem) 
				return GetEmpty();
			CComVariant val(VT_EMPTY);
			elem->get_nodeTypedValue(&val);
			if (val.vt==VT_BSTR) 
				return val.bstrVal;
			THROW_EXC( MonadException::InvalidClass, E_FAIL, L"Error in XML: No such value" );
		}
		TElem subnode(const std::wstring & name) const
		{ 
			if (!elem) return TElem();
			for (TElem c=begin(); c!=end(); c++)
			{ 
				if (c.name()==name)
					return c;
			}
			return TElem();
		}
		const std::wstring subval(const std::wstring & name) const
		{
			if (!elem) 
				return GetEmpty();
			TElem c=subnode(name);
			return c.val();
		}
		TElem begin() const
		{ 
			if (!elem) 
				return TElem();
			CComPtr<IXMLDOMNodeList> iNodeList;
			elem->get_childNodes(&iNodeList);
			return TElem(iNodeList);
		}
		TElem end() const
		{
			return TElem(clen);
		}
		TElem operator++(int)
		{ 
			if (pos!=-1)
			{
				pos++; 
				get();
			}
			return *this;
		}
		bool operator!=(const TElem &e) const
		{ 
			return pos!=e.clen;
		}
		static const std::wstring & GetEmpty()
		{
			static const std::wstring sEmpty;

			return sEmpty;
		}
	} mXMLParser;
	CComPtr<IXMLDOMElement> iRootElm;
	CComPtr<IXMLDOMDocument> iXMLDoc;
};
#endif