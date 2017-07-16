#ifndef _CORE_MEMORY_READER_HPP_
#define _CORE_MEMORY_READER_HPP_
 
/******************************************************************************
**	Includes
******************************************************************************/
#include "StreamReader.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class MemoryReader : public StreamReader
	{
	public:
		MemoryReader();
		MemoryReader(const void* _pBuffer, uint32 _uiSize);
		~MemoryReader();

		inline virtual void		Seek			(uint32 _uiPosition) { m_uiPosition = _uiPosition; }
		inline virtual uint32	GetPosition		() const { return m_uiPosition; }
		inline virtual uint32	GetSize			() const { return m_uiSize; }

		inline uint8& operator[](uint32 _uiIndex) { return m_pBuffer[_uiIndex]; }

		using StreamReader::Read;
		void Read(void* _pBuffer, uint32 _uiSize) override;

	protected:
		uint8*	m_pBuffer;
		uint32	m_uiPosition;
		uint32	m_uiSize;
	};
}

#endif