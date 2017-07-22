/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Thread/ConditionVariable.hpp>
#include <Core/Thread/Mutex.hpp>
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-05-15
	ConditionVariable::ConditionVariable()
	{
		CONDITION_VARIABLE* pConditionVariable = new CONDITION_VARIABLE();
		m_pHandle = pConditionVariable;
		InitializeConditionVariable(pConditionVariable);
	}

	//!	@brief		Constructor
	//!	@date		2015-05-15
	ConditionVariable::~ConditionVariable()
	{
		CONDITION_VARIABLE* pConditionVariable = static_cast<CONDITION_VARIABLE*>(m_pHandle);
		delete pConditionVariable;
	}

	//!	@brief		Constructor
	//!	@date		2015-05-15
	void ConditionVariable::Wait()
	{
		CONDITION_VARIABLE* pConditionVariable = static_cast<CONDITION_VARIABLE*>(m_pHandle);
		CRITICAL_SECTION* pCriticalSection = static_cast<CRITICAL_SECTION*>(m_Mutex.m_pHandle);
		SleepConditionVariableCS(pConditionVariable, pCriticalSection, INFINITE);
	}

	//!	@brief		Constructor
	//!	@date		2015-05-15
	void ConditionVariable::Signal()
	{
		CONDITION_VARIABLE* pConditionVariable = static_cast<CONDITION_VARIABLE*>(m_pHandle);
		WakeAllConditionVariable(pConditionVariable);
	}
}