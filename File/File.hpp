#ifndef _FILE_HPP_
#define _FILE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/String/Path.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	// Interface on HANDLE (void*)
	class File
	{
	public:
		uint32	Read		(void* _pBuffer, uint32 _uiSize);
		uint32	Write		(const void* _pBuffer, uint32 _uiSize);
		void	Seek		(uint32 _uiPosition);

		uint32 GetSize		() const;
		uint32 GetPosition	() const;
		uint32 GetAttribute	() const;
		uint64 GetLastWrite	() const;
	};
}
#endif