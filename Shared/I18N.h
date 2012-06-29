#pragma once
#ifndef _MONAD_I18N_H_
#define _MONAD_I18N_H_

#include <windows.h>
#include <boost/utility.hpp>
#include <string>

namespace MonadI18N
{
	enum MONAD_DEF_LANG
	{
		LANG_PL = MAKELANGID( LANG_POLISH, SUBLANG_POLISH_POLAND ),
		LANG_RU = MAKELANGID( LANG_RUSSIAN, SUBLANG_RUSSIAN_RUSSIA ),
		LANG_EN = MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US )
	};

	MONAD_DEF_LANG GetDefaultLanguage() throw();
	MONAD_DEF_LANG GetLanguage() throw();
	const std::wstring & GetLanguageString() throw();
	const std::wstring & GetLanguageNameString() throw();

	void SetLanguage( MONAD_DEF_LANG pLang ) throw();
	void SetIULocale();

	struct CI18NString : boost::noncopyable
	{
		CI18NString(
			const std::wstring & p_szPl,
			const std::wstring & p_szRu,
			const std::wstring & p_szEn );
		CI18NString(
			const std::wstring & p_szNeutral );
		operator const std::wstring & () const throw();
		operator LPCWSTR () const throw();
		const std::wstring & GetI18NString( MONAD_DEF_LANG pLang = GetLanguage() ) const throw();
		const CI18NString operator+( const CI18NString &other ) const;


	protected:
		const std::wstring m_szPl;
		const std::wstring m_szRu;
		const std::wstring m_szEn;
	};

	const std::wstring & GetOK();
	const std::wstring & GetCancel();
	const std::wstring & GetYes();
	const std::wstring & GetNo();
	const std::wstring & GetOPC();
	const std::wstring & LanguageSel();

	int MonadMessageBox( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType );

	std::string UnicodeToAnsi( const std::wstring & pUnicode );
	std::wstring AnsiToUnicode( const std::string & pANSI );
}
#endif // _MONAD_I18N_H_