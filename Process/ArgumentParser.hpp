#ifndef _CORE_ARGUMENT_PARSER_HPP_
#define _CORE_ARGUMENT_PARSER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Argument.hpp"
#include <Core/Container/Vector.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class ArgumentParser
	{
	public:
		ArgumentParser(const char* _szTitle, const char*_szDescription, const char* _szVersion);
		~ArgumentParser();

		void	Add				(const char* _szPattern, const char* _szDescription = nullptr, bool _bIsRequired = true);
		bool	Parse			(int argc, const char** argv, String* pUsageOut = nullptr);
		void	GenerateUsage	(String& _sUsageOut) const;

		template <class T>
		T		Get				(const char* _szPattern);

	private:
		String m_sTitle;
		String m_sDescription;
		String m_sVersion;
		Vector<ArgumentExtended> m_vArgument;
	};

	//!	@brief		Get
	//!	@details	Cast the value of a specific argument
	//!	@date		2015-04-05
	template <class T>
	T ArgumentParser::Get(const char* _szPattern)
	{
		T kValue;
		const uint32 uiArgumentCount = m_vArgument.GetSize();
		for(uint32 uiArgument = 0; uiArgument < uiArgumentCount; ++uiArgument)
		{
			const ArgumentExtended& kArgument = m_vArgument[uiArgument];
			if(kArgument.Match(_szPattern))
			{
				kArgument.Get(kValue);
				break; 
			}
		}

		return kValue;
	}
}

#endif