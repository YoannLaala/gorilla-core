/******************************************************************************
**	Includes
******************************************************************************/
#include "MemoryAllocation.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-04
	MemoryAllocation::MemoryAllocation()
	{

	}

	//!	@brief		Initialize
	//!	@date		2015-04-04
	void MemoryAllocation::Initialize(const size_t _uiDomain, const size_t _uiSize)
	{
		m_uiDomain = _uiDomain;
		m_uiSize = _uiSize;
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	MemoryAllocation::~MemoryAllocation()
	{
		// Nothing to do
	}
}