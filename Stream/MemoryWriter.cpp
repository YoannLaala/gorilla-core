/******************************************************************************
**	Includes
******************************************************************************/
#include "MemoryWriter.hpp"
#include <string.h>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-09-16
	MemoryWriter::MemoryWriter()
		: m_uiPosition(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-09-16
	MemoryWriter::~MemoryWriter()
	{
		// Nothing to do
	}

	//!	@brief		Write
	//!	@date		2015-09-16
	void MemoryWriter::Write(const void* _pValue, uint32 _uiSize)
	{
		m_vStream.Expand(_uiSize);
		memcpy_s(&m_vStream[m_uiPosition], _uiSize, _pValue, _uiSize); 
		m_uiPosition += _uiSize;
	}
}