/******************************************************************************
**	Includes
******************************************************************************/
#include "Mutex.hpp"
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-05-15
	Mutex::Mutex()
	{
		CRITICAL_SECTION* pCriticalSection = new CRITICAL_SECTION();
		m_pHandle = pCriticalSection;
		InitializeCriticalSection(pCriticalSection);
	}

	//!	@brief		Destructor
	//!	@date		2015-05-15
	Mutex::~Mutex()
	{
		LPCRITICAL_SECTION pCriticalSection = static_cast<LPCRITICAL_SECTION>(m_pHandle);
		DeleteCriticalSection(pCriticalSection);
		delete pCriticalSection;
	}

	//!	@brief		Lock
	//!	@date		2015-05-15
	void Mutex::Lock()
	{
		LPCRITICAL_SECTION pCriticalSection = static_cast<LPCRITICAL_SECTION>(m_pHandle);
		EnterCriticalSection(pCriticalSection);
	}

	//!	@brief		Unlock
	//!	@date		2015-05-15
	void Mutex::Unlock()
	{
		LPCRITICAL_SECTION pCriticalSection = static_cast<LPCRITICAL_SECTION>(m_pHandle);
		LeaveCriticalSection(pCriticalSection);
	}
}