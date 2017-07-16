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

	//!	@brief		Get
	//!	@date		2015-04-05
	const String& Argument::Get() const 
	{ 
		return m_sValue; 
	}

	//!	@brief		Get
	//!	@date		2015-04-05
	void Argument::Get(bool& _bValueOut) const
	{ 
		_bValueOut = atoi(m_sValue.GetBuffer()) != 0; 
	}

	//!	@brief		Get
	//!	@date		2015-04-05
	void Argument::Get(uint32& _uiValueOut) const 
	{ 
		_uiValueOut = (uint32)atoi(m_sValue.GetBuffer()); 
	}

	//!	@brief		Get
	//!	@date		2015-04-05
	void Argument::Get(int& _iValueOut) const 
	{ 
		_iValueOut = atoi(m_sValue.GetBuffer()); 
	}

	//!	@brief		Get
	//!	@date		2015-04-05
	void Argument::Get(String& _sValueOut) const 
	{ 
		_sValueOut = m_sValue; 
	}
}