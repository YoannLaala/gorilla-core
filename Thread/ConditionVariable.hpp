#ifndef _CORE_CONDITION_VARIABLE_HPP_
#define _CORE_CONDITION_VARIABLE_HPP_

#include <Core/Thread/Mutex.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class ConditionVariable
	{
	public:
		ConditionVariable();
		~ConditionVariable();

		void Wait();
		void Signal();

	private:
		Mutex m_Mutex;
		void* m_pHandle;
	};
}

#endif