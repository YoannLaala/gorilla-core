/******************************************************************************
**	Includes
******************************************************************************/
#include "Argument.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-05
	Argument::Argument()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-05
	Argument::~Argument()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-04-05
	void Argument::Initialize(const char* _szPattern, const char* _szValue)
	{
		SetPattern(_szPattern);
		SetValue(_szValue);
	}
}