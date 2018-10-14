#ifndef _CONTAINER_HPP_
#define _CONTAINER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/TypeDef.hpp>
#include <Core/Memory/Memory.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	namespace Container
	{
		//!	@brief		Copy
		//!	@details	Copy Custom class
		//!	@date		2015-04-04
		template <typename TYPE>
		void inline Copy(TYPE* _pDestination, const TYPE* _pSource, uint32 _uiSize)
		{
			// Copy Data
			for (uint32 iElement = 0; iElement < _uiSize; ++iElement)
			{
				_pDestination[iElement] = _pSource[iElement];
			}
		}

		// Allow fast copy for primitive types
		template <>	void Copy<bool>(bool* _pDestination, const bool* _pSource, uint32 _uiSize);
		template <>	void Copy<uint8>(uint8* _pDestination, const uint8* _pSource, uint32 _uiSize);
		template <>	void Copy<uint16>(uint16* _pDestination, const uint16* _pSource, uint32 _uiSize);
		template <>	void Copy<uint32>(uint32* _pDestination, const uint32* _pSource, uint32 _uiSize);
		template <>	void Copy<uint64>(uint64* _pDestination, const uint64* _pSource, uint32 _uiSize);
		template <>	void Copy<int8>(int8* _pDestination, const int8* _pSource, uint32 _uiSize);
		template <>	void Copy<int16>(int16* _pDestination, const int16* _pSource, uint32 _uiSize);
		template <>	void Copy<int32>(int32* _pDestination, const int32* _pSource, uint32 _uiSize);
		template <>	void Copy<int64>(int64* _pDestination, const int64* _pSource, uint32 _uiSize);
		template <>	void Copy<float32>(float32* _pDestination, const float32* _pSource, uint32 _uiSize);
		template <>	void Copy<float64>(float64* _pDestination, const float64* _pSource, uint32 _uiSize);

		void CopyString(char* _pDestination, const char* _pSource, uint32 _uiSize);

		void Set(void* _pDestination, int32 _iValue, uint32 uiSize);
	}
}

#endif