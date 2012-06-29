#pragma once
#ifndef _MONAD_EVENT_H_
#define _MONAD_EVENT_H_

#include <boost/utility.hpp>
#include "CProto.h"
#include "MonadTemplates.h"
#include <boost/shared_ptr.hpp>
#include "MonadTemplates.h"

namespace MonadEvent
{
	enum // The order is important in a vector storage
	{
		EventResetDevice,
		EventResetLogic,
		EventResetLogicDX9,
		EventResetLogicDX11,
		EventFrameMoveStub,
		EventLostDevice,
		EventDestroyDevice,
		EventNewSecond,
		//
		EventVectorSize
	};

	// This class assumes usage in main thread only
	class COnEvent abstract : public CDestructable
	{
	public:
		static void FireEvent( const ULONG_PTR pEvent );

		void operator=( const COnEvent & Source ); // Denied

	protected:
		COnEvent( const COnEvent & Source );
		COnEvent();
		virtual ~COnEvent() throw();
		virtual void OnResetDevice() {}
		virtual void OnResetLogic() {}
		virtual void OnResetLogicDX9() {}
		virtual void OnResetLogicDX11() {}
		virtual void OnFrameMoveStub() {}
		virtual void OnLostDevice() {}
		virtual void OnDestroyDevice() {}
		virtual void OnNewSecond() {}
		void RegisterEvent( const ULONG_PTR pEvent );
		void UnregisterEvent( const ULONG_PTR pEvent ) throw();
		void RegisterLostEvent();
		void RegisterResetEvent();
		void RegisterNewSecondEvent();

	private:
		void RegisterDestroyEvent();
		typedef CMonadSet<COnEvent *> CGenericSkills;
		struct CSkilledObjects : public CGenericSkills, boost::noncopyable
		{
			CSkilledObjects( CObjFunctor * pFunctor );
			CSkilledObjects(); // Denied
			~CSkilledObjects();
			void for_each_functor();

		protected:
			CObjFunctor * m_pFunctor;
		};
		typedef boost::shared_ptr<CSkilledObjects> CAutoSkilledObjects;
		typedef std::vector<CAutoSkilledObjects> CSkillVectors;
		struct CSkillCollector : public CSkillVectors
		{
			CSkillCollector();

		private:
			void AddSkill( 
#ifdef DEBUG
				const ULONG_PTR pSkill,
#endif
				CSkilledObjects::CObjFunctor * pFunctor );
		};
		static CSkillCollector & VariableRegistry();
		void CopyEvents( const COnEvent * Source );
	};
}
#endif