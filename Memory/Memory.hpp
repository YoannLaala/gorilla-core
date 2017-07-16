#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

/******************************************************************************
**	Include
******************************************************************************/
#include "../TypeDef.hpp"

/******************************************************************************
**	Defines
******************************************************************************/
#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }
#define SAFE_DELETE(p) { if(p) { delete p; p = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] p; p = nullptr; } }
#define SAFE_RELEASE_AND_DELETE(p) { if(p) { p->Release(); delete p; p = nullptr; } }
#define SAFE_CLOSE(p) { if(p) { ::CloseHandle(p); p = nullptr; } }

namespace Gorilla
{
	namespace EMemory
	{
		enum Type : uint8
		{
			Stack = 0,
			Heap,
			Count,
		};	
	}
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Memory
	{
	public:
		static void*	Allocate	(const size_t _uiSize);
		static void		Deallocate	(void* _pValue);

		//!	@brief		AllocateArray
		//!	@details	Allocate an array and call constructor on Element
		//!	@date		2015-04-04
		template <class T>
		static T* AllocateArray(const size_t _uiElementCount)
		{
			char* pMemory = (char*)Allocate(_uiElementCount * sizeof(T));			
			for(size_t uiElement = 0; uiElement < _uiElementCount; ++uiElement)
			{
				new (pMemory + uiElement * sizeof(T)) T;
			}
		
			return (T*)pMemory;
		}

		//!	@brief		DeallocateArray
		//!	@details	Deallocate an array and call destructor on Element
		//!	@date		2015-04-04
		template <class T>
		static void DeallocateArray(T* _pValue, size_t _uiElementCount)
		{	
			for(size_t uiElement = 0; uiElement < _uiElementCount; ++uiElement)
			{
				_pValue[uiElement]->~T();
			}
			Deallocate(_pValue);
		}
	};
}

#endif