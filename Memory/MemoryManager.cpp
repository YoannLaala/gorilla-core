/******************************************************************************
**	Includes
******************************************************************************/
#include "MemoryManager.hpp"
#include "MemoryAllocation.hpp"
#include "MemoryDomain.hpp"
#include <new.h>

/******************************************************************************
**	Class Definition
******************************************************************************/

#ifdef MEMORY_MANAGER_ACTIVATED
Gorilla::MemoryManager*Gorilla:: MemoryManager::m_pInstance = new (Gorilla::Memory::Allocate(sizeof(Gorilla::MemoryManager))) Gorilla::MemoryManager();
#else
Gorilla::MemoryManager* Gorilla::MemoryManager::m_pInstance = NULL;
#endif

namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-04
	MemoryManager::MemoryManager() 
	{ 
		// Default domain
		m_uiDomain = 0;

		// Add default MemoryDomain with first allocation
		size_t uiAllocationSize;
		MemoryDomain* pMemoryDomain = new (Allocate(sizeof(MemoryDomain), uiAllocationSize)) MemoryDomain();
		pMemoryDomain->Add(sizeof(MemoryManager) + sizeof(MemoryDomain));
		//m_vMemoryDomain.SetBuffer(pMemoryDomain, 1, 1);
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	MemoryManager::~MemoryManager() 
	{ 
		m_vMemoryDomain.Clear();
	}


	//!	@brief		Allocate
	//!	@details	Allocate memory add update current MemoryDomain
	//!	@date		2015-04-04
	void* MemoryManager::Allocate(const size_t _uiSize)
	{
		size_t uiAllocationSize;
		void* pMemory = (MemoryAllocation*)Allocate(_uiSize, uiAllocationSize);
		//m_vMemoryDomain[m_uiDomain].Add(uiAllocationSize);

		return pMemory;
	}

	//!	@brief		Allocate
	//!	@details	Allocate memory with more information
	//!	@date		2015-04-04
	void* MemoryManager::Allocate(const size_t _uiSize, size_t& _uiSizeExtended)
	{
		_uiSizeExtended = sizeof(MemoryAllocation) + _uiSize;
		MemoryAllocation* pMemoryAllocation = (MemoryAllocation*)Memory::Allocate(_uiSizeExtended);
		pMemoryAllocation->Initialize(m_uiDomain, _uiSize);

		return ++pMemoryAllocation;
	}

	//!	@brief		Deallocate
	//!	@details	Deallocate the memory and update the MemoryDomain associated
	//!	@date		2015-04-04
	void MemoryManager::Deallocate(void* _pValue)
	{
		MemoryAllocation* pMemory = (MemoryAllocation*)((char*)_pValue - sizeof(MemoryAllocation));
		//m_vMemoryDomain[pMemory->GetDomain()].Remove(pMemory->GetSize());
		Memory::Deallocate(pMemory);
	}

	//!	@brief		SetDomain
	//!	@details	Set the current domain for the next allocation
	//!	@date		2015-04-04
	void MemoryManager::SetDomain(const uint8 _uiDomain, const char* _szName)
	{
		// Reset the default memory domain to allocate other MemoryDomain
		m_uiDomain = 0;
		while(_uiDomain >= m_vMemoryDomain.GetSize())
		{
			m_vMemoryDomain.Add();
		}

		// Initialize new one if needed
		MemoryDomain& kMemoryDomain = m_vMemoryDomain[_uiDomain];
		if(!kMemoryDomain.IsInitialized())
		{
			kMemoryDomain.SetName(_szName);
		}
		m_uiDomain = _uiDomain;
	}
}