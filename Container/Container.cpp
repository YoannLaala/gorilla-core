/******************************************************************************
**	Includes
******************************************************************************/
#include "Container.hpp"
#include <string> // memcpy_s
/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	namespace Container
	{
		// Copy
		template <>	void Copy<bool>(bool* _pDestination, const bool* _pSource, uint32 _uiSize) { memcpy_s(_pDestination, _uiSize, _pSource, _uiSize); }
		template <>	void Copy<uint8>(uint8* _pDestination, const uint8* _pSource, uint32 _uiSize) { memcpy_s(_pDestination, _uiSize, _pSource, _uiSize); }
		template <>	void Copy<uint16>(uint16* _pDestination, const uint16* _pSource, uint32 _uiSize) { const uint32 uiByteCount = _uiSize * sizeof(uint16); memcpy_s(_pDestination, uiByteCount, _pSource, uiByteCount); }
		template <>	void Copy<uint32>(uint32* _pDestination, const uint32* _pSource, uint32 _uiSize) { const uint32 uiByteCount = _uiSize * sizeof(uint32); memcpy_s(_pDestination, uiByteCount, _pSource, uiByteCount); }
		template <>	void Copy<uint64>(uint64* _pDestination, const uint64* _pSource, uint32 _uiSize) { const uint32 uiByteCount = _uiSize * sizeof(uint64); memcpy_s(_pDestination, uiByteCount, _pSource, uiByteCount); }
		template <>	void Copy<int8>(int8* _pDestination, const int8* _pSource, uint32 _uiSize) { memcpy_s(_pDestination, _uiSize, _pSource, _uiSize); }
		template <>	void Copy<int16>(int16* _pDestination, const int16* _pSource, uint32 _uiSize) { const uint32 uiByteCount = _uiSize * sizeof(int16); memcpy_s(_pDestination, uiByteCount, _pSource, uiByteCount); }
		template <>	void Copy<int32>(int32* _pDestination, const int32* _pSource, uint32 _uiSize) { const uint32 uiByteCount = _uiSize * sizeof(int32); memcpy_s(_pDestination, uiByteCount, _pSource, uiByteCount); }
		template <>	void Copy<int64>(int64* _pDestination, const int64* _pSource, uint32 _uiSize) { const uint32 uiByteCount = _uiSize * sizeof(int64); memcpy_s(_pDestination, uiByteCount, _pSource, uiByteCount); }
		template <>	void Copy<float32>(float32* _pDestination, const float32* _pSource, uint32 _uiSize) { const uint32 uiByteCount = _uiSize * sizeof(float32); memcpy_s(_pDestination, uiByteCount, _pSource, uiByteCount); }
		template <>	void Copy<float64>(float64* _pDestination, const float64* _pSource, uint32 _uiSize) { const uint32 uiByteCount = _uiSize * sizeof(float64); memcpy_s(_pDestination, uiByteCount, _pSource, uiByteCount); }

		void CopyString(char* _pDestination, const char* _pSource, uint32 _uiSize)
		{
			strcpy_s(_pDestination, _uiSize, _pSource);
		}

		void Set(void* _pDestination, int32 _iValue, uint32 uiSize)
		{
			memset(_pDestination, _iValue, uiSize);
		}
	}
}