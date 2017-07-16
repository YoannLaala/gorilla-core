#ifndef _HASH_HPP_
#define _HASH_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/String/String.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Hash
	{
	public:
		static uint32			Generate			(const char* _szKey);
		static uint32			Generate			(const void* _pKey, uint32 _uiLength);
		static inline uint32	Generate			(uint16 _uiKey) { return Generate(&_uiKey, sizeof(uint16)); }
		static inline uint32	Generate			(uint32 _uiKey) { return Generate(&_uiKey, sizeof(uint32)); }
		static inline uint32	Generate			(uint64 _uiKey) { return Generate(&_uiKey, sizeof(uint64)); }
		static inline uint32	Generate			(const String& _sKey) { return Generate(_sKey.GetBuffer()); }
	};
}

#endif