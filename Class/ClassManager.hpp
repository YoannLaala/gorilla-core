#ifndef _CLASS_MANAGER_HPP_
#define _CLASS_MANAGER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/TypeDef.hpp>
#include <Core/Container/HashMap.hpp>
#include <Core/Class/Class.hpp>
#include <Core/Signal.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define CLASS_SECTION_MAX 3

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla 
{
	class ClassManager
	{
	public:
		struct EClass
		{
			enum Type
			{
				Custom = 0,

				Count,
			};
		};

		inline static ClassManager*	GetInstance() { static ClassManager kInstance; return &kInstance; }

		inline HashMap<uint32, Class*>&			GetAll	(uint32 _eClass) { return m_aValue[_eClass]; }
		Class*									Get		(uint32 _eClass, const char* _szClassName);
		Class*									Get		(uint32 _eClass, uint32 _uiId);
		void									Set		(uint32 _eClass, Class* _pClass);
		
	private:
		HashMap<uint32, Class*> m_aValue[CLASS_SECTION_MAX];
	};
}

#endif