/******************************************************************************
**	Includes
******************************************************************************/
#include "ClassManager.hpp"
#include <Core/Hash.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla 
{
	//!	@brief		Get
	//!	@date		2015-05-08
	Class* ClassManager::Get(uint32 _eClass, const char* _szClassName)
	{
		uint32 uiId = Hash::Generate(_szClassName);
		return Get(_eClass, uiId);
	}

	//!	@brief		Get
	//!	@date		2015-05-08
	Class* ClassManager::Get(uint32 _eClass, uint32 _uiId)
	{
		if(_eClass < CLASS_SECTION_MAX)
		{
			return m_aValue[_eClass][_uiId];
		}

		return nullptr;
	}

	//!	@brief		Set
	//!	@details	Register a class allowing create instance at runtime from name
	//!	@date		2015-05-08
	void ClassManager::Set(uint32 _eClass, Class* _pClass)	
	{	
		m_aValue[_eClass].Add(_pClass->GetId(), _pClass);
	}
}
