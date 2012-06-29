#include "DXUT.h"
#include "MonadTime.h"
#include "MonadTemplates.h"
#include "MonadEvents.h"
#include "DeviceLevel.h"
#include "Singleton.h"

namespace MonadEvent
{
#ifdef DEBUG
#define ADD_SKILL(x,y) AddSkill( x, new y() )
#else
#define ADD_SKILL(x,y) AddSkill( new y() )
#endif

	COnEvent::COnEvent()
	{
		RegisterDestroyEvent();
	}
	COnEvent::COnEvent( const COnEvent & Source )
	{
		CopyEvents( &Source );
	}
	void COnEvent::CopyEvents( const COnEvent * Source )
	{
		BOOST_FOREACH( CSkillCollector::value_type & x, VariableRegistry() )
		{
			CSkilledObjects::const_iterator it( x->find( const_cast< COnEvent * > ( Source ) ) );
			if( x->end() != it )
				x->insert( this );
		}
	}

	COnEvent::~COnEvent() throw()
	{
		// Free resources and remove dead pointers
		BOOST_FOREACH( CSkillCollector::value_type & x, VariableRegistry() )
			x->erase( this );
	}
	void COnEvent::RegisterEvent( const ULONG_PTR pEvent )
	{
		VariableRegistry()[ pEvent ]->insert( this );
	}
	void COnEvent::UnregisterEvent( const ULONG_PTR pEvent ) throw()
	{
		VariableRegistry()[ pEvent ]->erase( this );
	}
	void COnEvent::RegisterDestroyEvent()
	{
		RegisterEvent( EventDestroyDevice );
	}
	void COnEvent::RegisterLostEvent()
	{
		RegisterEvent( EventLostDevice );
	}
	void COnEvent::RegisterResetEvent()
	{
		RegisterEvent( EventResetDevice );
	}
	void COnEvent::RegisterNewSecondEvent()
	{
		RegisterEvent( EventNewSecond );
	}

	void COnEvent::FireEvent( const ULONG_PTR pEvent )
	{
		VariableRegistry()[ pEvent ]->for_each_functor();
	}
	COnEvent::CSkillCollector & COnEvent::VariableRegistry()
	{
		static CSkillCollector mReg;

		return mReg;
	}
	COnEvent::CSkillCollector::CSkillCollector()
	{
		reserve( EventVectorSize );
		struct CResetDevice : public CSkilledObjects::CObjFunctor
		{
			virtual void operator()( CSkilledObjects::SetValue pObj )
			{
				pObj->OnResetDevice();
			}
		};
		struct CResetLogic : public CSkilledObjects::CObjFunctor
		{
			virtual void operator()( CSkilledObjects::SetValue pObj )
			{
				pObj->OnResetLogic();
			}
		};
		struct CResetLogicDX9 : public CSkilledObjects::CObjFunctor
		{
			virtual void operator()( CSkilledObjects::SetValue pObj )
			{
				pObj->OnResetLogicDX9();
			}
		};
		struct CResetLogicDX11 : public CSkilledObjects::CObjFunctor
		{
			virtual void operator()( CSkilledObjects::SetValue pObj )
			{
				pObj->OnResetLogicDX11();
			}
		};
		struct CFrameMoveStub : public CSkilledObjects::CObjFunctor
		{
			virtual void operator()( CSkilledObjects::SetValue pObj )
			{
				pObj->OnFrameMoveStub();
			}
		};
		struct CLostDevice : public CSkilledObjects::CObjFunctor
		{
			virtual void operator()( CSkilledObjects::SetValue pObj )
			{
				pObj->OnLostDevice();
			}
		};
		struct CDestroyDevice : public CSkilledObjects::CObjFunctor
		{
			virtual void operator()( CSkilledObjects::SetValue pObj )
			{
				pObj->OnDestroyDevice();
			}
		};
		struct CNewSecond : public CSkilledObjects::CObjFunctor
		{
			virtual void operator()( CSkilledObjects::SetValue pObj )
			{
				pObj->OnNewSecond();
			}
		};

		ADD_SKILL( EventResetDevice, CResetDevice );
		ADD_SKILL( EventResetLogic, CResetLogic );
		ADD_SKILL( EventResetLogicDX9, CResetLogicDX9 );
		ADD_SKILL( EventResetLogicDX11, CResetLogicDX11 );
		ADD_SKILL( EventFrameMoveStub, CFrameMoveStub );
		ADD_SKILL( EventLostDevice, CLostDevice );
		ADD_SKILL( EventDestroyDevice, CDestroyDevice );
		ADD_SKILL( EventNewSecond, CNewSecond );
	}
	void COnEvent::CSkillCollector::AddSkill(
#ifdef DEBUG
		const ULONG_PTR pSkill,
#endif
		CSkilledObjects::CObjFunctor * pFunctor )
	{
		assert( pSkill == size() );
		push_back( CAutoSkilledObjects( new CSkilledObjects( pFunctor ) ) );
	}

	COnEvent::CSkilledObjects::CSkilledObjects( CObjFunctor * pFunctor )
		: m_pFunctor( pFunctor )
	{
	}
	COnEvent::CSkilledObjects::~CSkilledObjects()
	{
		delete m_pFunctor;
	}
	void COnEvent::CSkilledObjects::for_each_functor()
	{
		for_each( m_pFunctor );
	}
}