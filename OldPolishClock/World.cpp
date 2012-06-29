#include "DXUT.h"
#include "World.h"
#include "Light.h"
#include "Singleton.h"
#include "OldPolishClock.h"
#include "Wall.h"

CPictureFactory gPicturesFactory;

CPictureFactory::CPictureFactory()
:
m_szGirl( L"Girl" ),
m_szJesus( L"Jesus" ),
m_szBender( L"Ostap" ),
m_szLandscape( L"Landscape" ),
m_szNapoleon( L"Napoleon" ),
m_szAdam( L"Adam" )
{
	class CRandomString mPictures;
	mPictures.AddString( m_szGirl );
	mPictures.AddString( m_szJesus );
	mPictures.AddString( m_szBender );
	mPictures.AddString( m_szLandscape );
	mPictures.AddString( m_szNapoleon );
	mPictures.AddString( m_szAdam );

	m_szLeft = mPictures;
	mPictures.RemoveString( m_szLeft );
	m_szRight = mPictures;
}
CMonad * CPictureFactory::CreateMonad( const std::wstring & p_szSide, const PAINTING_POS pPos )
{
	if( m_szGirl == p_szSide )
		return new CGirl( pPos );
	else if( m_szJesus == p_szSide )
		return new CJesusPainting( pPos );
	else if( m_szBender == p_szSide )
		return new CPainting( pPos );
	else if( m_szLandscape == p_szSide )
		return new CLandscapePainting( pPos );
	else if( m_szNapoleon == p_szSide )
		return new CNapoleonPainting( pPos );
	else
		return new CAMPainting( pPos );
}
const std::wstring & CPictureFactory::GetLeft() const throw()
{
	return m_szLeft;
}
const std::wstring & CPictureFactory::GetRight() const throw()
{
	return m_szRight;
}

CWorld::CWorld()
{	
	m_first.Attach( gPicturesFactory.CreateMonad( gPicturesFactory.GetLeft(), PAINTING_LEFT ) );
	m_second.Attach( gPicturesFactory.CreateMonad( gPicturesFactory.GetRight(), PAINTING_RIGHT ) );
	AddRenderedObject( &m_clock );
	if( m_first )
		AddRenderedObject( m_first );
	if( m_second )
		AddRenderedObject( m_second );
	AddRenderedObject( &m_mount );
	AddRenderedObject( &m_wall );
}