/******************************************************************************
**	Includes
******************************************************************************/
#include "MemoryReader.hpp"
#include <string.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-09-16
	MemoryReader::MemoryReader()
		: m_pBuffer(0)
		, m_uiSize(0)
		, m_uiPosition(0)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-09-16
	MemoryReader::MemoryReader(const void* _pBuffer, uint32 _uiSize)
		: m_pBuffer(static_cast<uint8*>(const_cast<void*>(_pBuffer)))
		, m_uiSize(_uiSize)
		, m_uiPosition(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-09-16
	MemoryReader::~MemoryReader()
	{
		// Nothing to do
	}

	//!	@brief		Read
	//!	@date		2015-09-16
	void MemoryReader::Read(void* _pValue, uint32 _uiSize)
	{
		memcpy_s(_pValue, _uiSize, &m_pBuffer[m_uiPosition], _uiSize);
		m_uiPosition += _uiSize;
	}
}