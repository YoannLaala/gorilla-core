/******************************************************************************
**	Includes
******************************************************************************/
#include "Memory.hpp"
#include <new> 

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Allocate
	//!	@details	Allocate some memory using malloc
	//!	@date		2015-04-04
	void* Memory::Allocate(const size_t _uiSize)
	{
		void* pMemory = malloc(_uiSize);
		if(!pMemory)
		{
			throw std::bad_alloc();
		}
		
		return pMemory;
	}

	//!	@brief		Deallocate
	//!	@details	Deallocate the memory using free
	//!	@date		2015-04-04
	void Memory::Deallocate(void* _pValue)
	{
		free(_pValue);
	}
}