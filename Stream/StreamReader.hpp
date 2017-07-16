#ifndef _CORE_STREAM_READER_HPP_
#define _CORE_STREAM_READER_HPP_
 
/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Container/Vector.hpp>
#include <Core/String/String.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class StreamReader
	{
	public:
		inline virtual void		Seek			(uint32 _uiPosition) = 0;
		inline virtual uint32	GetPosition		() const = 0;
		inline virtual uint32	GetSize			() const = 0;

		inline void	Read (bool& _bValue) 		{ Read(&_bValue, sizeof(bool)); }
		inline void Read (uint8& _uiValue) 		{ Read(&_uiValue, sizeof(uint8)); }
		inline void Read (uint16& _uiValue) 	{ Read(&_uiValue, sizeof(uint16)); }
		inline void Read (uint32& _uiValue) 	{ Read(&_uiValue, sizeof(uint32)); }
		inline void Read (uint64& _uiValue) 	{ Read(&_uiValue, sizeof(uint64)); }
		inline void Read (int8& _iValue)		{ Read(&_iValue, sizeof(int8)); }
		inline void Read (int16& _iValue)		{ Read(&_iValue, sizeof(int16)); }
		inline void Read (int32& _iValue)		{ Read(&_iValue, sizeof(int32)); }
		inline void Read (int64& _iValue) 		{ Read(&_iValue, sizeof(int64)); }
		inline void	Read (float32& _fValue) 	{ Read(&_fValue, sizeof(float32)); }
		inline void	Read (float64& _fValue) 	{ Read(&_fValue, sizeof(float64)); }
		inline void	Read (String& _sValue)		{ uint32 uiLength; Read(uiLength); _sValue.Resize(uiLength); Read(&_sValue[0], uiLength); }

		template <class T>
		inline void Read(Vector<T>& _vValue);

		virtual void Read(void* _pValue, uint32 _uiSize) = 0;
	};

	template <class T>
	void StreamReader::Read(Vector<T>& _vValue)
	{
		// Get the size of the vector first
		uint32 uiSize;
		Read(uiSize);

		// Allocate and copy
		_vValue.Resize(uiSize);
		Read(&_vValue[0], uiSize * sizeof(T));
	}
}

#endif