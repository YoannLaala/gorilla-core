#ifndef _MEMORY_MANAGER_HPP_
#define _MEMORY_MANAGER_HPP_

//#define MEMORY_MANAGER_ACTIVATED

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Container/Vector.hpp>

/******************************************************************************
**	Forward Declarations
******************************************************************************/
namespace Gorilla
{
	class MemoryDomain;
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class MemoryManager
	{	
	public:
		static MemoryManager*	GetInstance		() { static MemoryManager m_sMemoryManager; return &m_sMemoryManager; }

		void					SetDomain		(const uint8 _uiDomain, const char* _szName);
		void*					Allocate		(const size_t _uiSize);
		void					Deallocate		(void* _pValue);

	private:
		MemoryManager();
		~MemoryManager();
		void* Allocate(const size_t _uiSize, size_t& _uiSizeExtended);

	private:
		static MemoryManager*			m_pInstance;
		size_t							m_uiSize;
		size_t							m_uiDomain;
		Vector<MemoryDomain>			m_vMemoryDomain;
	};
}

#define MEMORY_DOMAIN(_id_) MemoryManager::GetInstance()->SetDomain(_id_, #_id_)
#ifdef MEMORY_MANAGER_ACTIVATED
inline void* operator new(size_t _uiSize) { return Gorilla::MemoryManager::GetInstance()->Allocate(_uiSize);	}																																														
inline void* operator new[](size_t _uiSize) { return Gorilla::MemoryManager::GetInstance()->Allocate(_uiSize); }																																																			
inline void operator delete(void* _pValue) { return Gorilla::MemoryManager::GetInstance()->Deallocate(_pValue); }																										
inline void operator delete[](void* _pValue) { return Gorilla::MemoryManager::GetInstance()->Deallocate(_pValue); }	
#endif

#endif