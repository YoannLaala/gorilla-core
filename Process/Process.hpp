#ifndef _CORE_PROCESS_HPP_
#define _CORE_PROCESS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Argument.hpp"
#include <Core/String/Path.hpp>
#include <Core/Container/Vector.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Process
	{
	public:
		Process();
		Process(const char* _szExecutablePath, const char* _szArgument = nullptr);
		~Process();

		void		Initialize	(const char* _szExecutablePath, const char* _szArgument = nullptr);
		bool		Execute		(String* _pResultOut = nullptr);

		inline const Path&	GetPath			() const { return m_kPath; }
		void				SetArgument		(const char* _szArgument = nullptr);

	private:
		void		ReadHandle			(void* hValue, String& _sValueOut) const;

	private:
		Path		m_kPath;
		String		m_sCommandLine;			
	};
}

#endif