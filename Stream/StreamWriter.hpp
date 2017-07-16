#ifndef _CORE_STREAM_WRITER_HPP_
#define _CORE_STREAM_WRITER_HPP_
 
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
	class StreamWriter
	{
	public:
		inline virtual bool		CanWrite		() const = 0;

		inline virtual void		Seek			(uint32 _uiPosition) = 0;
		inline virtual uint32	GetPosition		() const = 0;
		inline virtual uint32	GetSize			() const = 0;

		inline void	Write (bool _bValue) 		{ Write(&_bValue, sizeof(bool)); }
		inline void	Write (uint8 _uiValue)		{ Write(&_uiValue, sizeof(uint8)); }
		inline void	Write (uint16 _uiValue)		{ Write(&_uiValue, sizeof(uint16)); }
		inline void	Write (uint32 _uiValue)		{ Write(&_uiValue, sizeof(uint32)); }
		inline void	Write (uint64 _uiValue)		{ Write(&_uiValue, sizeof(uint64)); }
		inline void	Write (int8 _iValue)		{ Write(&_iValue, sizeof(int8)); }
		inline void	Write (int16 _iValue)		{ Write(&_iValue, sizeof(int16)); }
		inline void	Write (int32 _iValue)		{ Write(&_iValue, sizeof(int32)); }
		inline void	Write (int64 _iValue) 		{ Write(&_iValue, sizeof(int64)); }
		inline void	Write (float32 _fValue) 	{ Write(&_fValue, sizeof(float32)); }
		inline void	Write (float64 _fValue) 	{ Write(&_fValue, sizeof(float64)); }
		inline void	Write (const String& _sValue) { Write(_sValue.GetLength()); Write(_sValue.GetBuffer(), _sValue.GetLength()); }

		template <class T>
		inline void Write(const Vector<T>& _vValue);

		virtual void Write(const void* _pValue, uint32 _uiSize) = 0;
	};

	template <class T>
	void StreamWriter::Write(const Vector<T>& _vValue)
	{
		Write(_vValue.GetSize());
		Write(&_vValue[0], _vValue.GetSize() * sizeof(T));
	}
}

#endif