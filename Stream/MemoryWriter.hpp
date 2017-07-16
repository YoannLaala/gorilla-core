#ifndef _CORE_MEMORY_WRITER_HPP_
#define _CORE_MEMORY_WRITER_HPP_
 
/******************************************************************************
**	Includes
******************************************************************************/
#include "StreamWriter.hpp"
#include <Core/Container/Vector.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class MemoryWriter : public StreamWriter
	{
	public:
		MemoryWriter();
		~MemoryWriter();

		inline virtual bool		CanWrite		() const override { return true; } 

		inline virtual void		Seek			(uint32 _uiPosition) override { m_uiPosition = _uiPosition; }
		inline virtual uint32	GetPosition		() const override { return m_uiPosition; }
		inline virtual uint32	GetSize			() const override { return m_vStream.GetSize(); }

		inline uint8& operator[](uint32 _uiIndex) { return m_vStream[_uiIndex]; }

		using StreamWriter::Write;
		void Write (const void* _pValue, uint32 _uiSize) override;

	protected:
		Vector<uint8>	m_vStream;
		uint32			m_uiPosition;
	};
}

#endif