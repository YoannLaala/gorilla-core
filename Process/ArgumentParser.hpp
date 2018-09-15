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
		T			Get			(const char* _szPattern) const;

		inline bool Has			(const char* _szPattern) const { return GetArgument(_szPattern) != nullptr; }

	private:
		const ArgumentExtended* GetArgument(const char* _szPattern) const;

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
	T ArgumentParser::Get(const char* _szPattern) const
	{
		const ArgumentExtended* pArgument = GetArgument(_szPattern);
		if(pArgument) return pArgument->Get<T>();

		return T();		
	}
}

#endif