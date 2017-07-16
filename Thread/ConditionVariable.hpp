#ifndef _CORE_CONDITION_VARIABLE_HPP_
#define _CORE_CONDITION_VARIABLE_HPP_

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla
{
	class Mutex;
}

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

		void Wait(const Mutex& _kMutex);
		void Signal();

	private:
		void* m_pHandle;
	};
}

#endif