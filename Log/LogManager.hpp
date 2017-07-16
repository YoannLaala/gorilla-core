#ifndef _LOG_MANAGER_HPP_
#define _LOG_MANAGER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/String/String.hpp>
#include <Core/Container/Vector.hpp>
#include <Core/Signal.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#ifdef DEBUG
	#define LOG_INTERNAL_PRINT(_format_, ...)		Gorilla::LogManager::Print(Gorilla::ELog::Normal, __FILE__, __LINE__, _format_, __VA_ARGS__);
	#define LOG_INTERNAL_WARNING(_format_, ...)		Gorilla::LogManager::Print(Gorilla::ELog::Warning, __FILE__, __LINE__, _format_, __VA_ARGS__);
	#define LOG_INTERNAL_ERROR(_format_, ...)		Gorilla::LogManager::Print(Gorilla::ELog::Error, __FILE__, __LINE__, _format_, __VA_ARGS__);
	#define ASSERT(_condition_, _format_, ...)		Gorilla::LogManager::Assert(_condition_, __FILE__, __LINE__, _format_, __VA_ARGS__);
#else
	#define LOG_INTERNAL_PRINT(_format_, ...)		
	#define LOG_INTERNAL_WARNING(_format_, ...)		
	#define LOG_INTERNAL_ERROR(_format_, ...)		
	#define ASSERT(_condition_, _format_, ...)		
#endif

#define LOG_MAX_THREAD 8

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla
{
	namespace ELog
	{
		enum Type
		{
			Normal = 0,
			Warning,
			Error,

			Count,
		};

		static const char* Name[ELog::Count] = 
		{
			"Normal",
			"Warning",
			"Error"
		};
	}
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class LogManager
	{
	public:	
		SIGNAL_SENDER(LogChanged, ELog::Type /*_eLog*/, const char* /*_szMessage*/);

		static void	Print		(ELog::Type _eLog, const char* _szFilePath, uint32 _uiLine, const char* _szFormat, ...);
		static void	Assert		(bool _bCondition, const char* _szFilePath, uint32 _uiLine, const char* _szFormat, ...);

		void		Print		(const char* _szFormat, ...);
		void		Print		(ELog::Type _eLog, const char* _szFormat, ...);

	private:
		static String& GetBufferAvailable();
		
	private:
		static String s_aBuffer[LOG_MAX_THREAD];
		static uint32 s_aThread[LOG_MAX_THREAD];
		static uint32 s_uiIndex;
	};
}

#endif