/******************************************************************************
**	Includes
******************************************************************************/
#include "LogManager.hpp"
#include <Core/Thread/Thread.hpp>
#include <stdio.h>
#include <windows.h>

/******************************************************************************
**	Define
******************************************************************************/
#define LOG_MAX_LENGTH 2048

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	String LogManager::s_aBuffer[LOG_MAX_THREAD];
	uint32 LogManager::s_aThread[LOG_MAX_THREAD] { (uint32)-1, (uint32)-1, (uint32)-1, (uint32)-1, (uint32)-1, (uint32)-1, (uint32)-1, (uint32)-1 };
	uint32 LogManager::s_uiIndex = (uint32)-1;

	//!	@brief		BuildBuffer
	//!	@date		2015-04-04
	void BuildBuffer(const char* _szFormat, va_list pArg, String& sBufferOut)
	{
		// Format buffer
		const uint32 uiLength = sBufferOut.GetLength();
		const uint32 uiFormatSize = (uint32)vsprintf_s(&sBufferOut[uiLength], LOG_MAX_LENGTH - uiLength, _szFormat, pArg);
		const uint32 uiCRIndex = uiLength + uiFormatSize;
		sBufferOut.Resize(uiCRIndex + 1);
		sBufferOut[uiCRIndex] = '\n';
	}

	//!	@brief		BuildBuffer
	//!	@date		2015-04-04
	void BuildBuffer(const char* _szFilePath, uint32 _uiLine, const char* _szFormat, va_list pArg, String& sBufferOut)
	{
		// Filepath and line
		sBufferOut.Append(_szFilePath);
		sBufferOut.Append("(", 1);
		sBufferOut.Append(_uiLine);
		sBufferOut.Append("): ", 3);
		
		BuildBuffer(_szFormat, pArg, sBufferOut);
	}

	//!	@brief		Print a log
	//!	@date		2015-04-04
	void LogManager::Print(const char* _szFormat, ...) 
	{
		// Type
		String& sBuffer = GetBufferAvailable();
		
		// Generic information with Message
		va_list pArg;
		va_start(pArg, _szFormat);
		BuildBuffer(_szFormat, pArg, sBuffer);
		va_end(pArg);

		printf_s(sBuffer.GetBuffer());
	}

	//!	@brief		Print a log
	//!	@date		2015-04-04
	void LogManager::Print(ELog::Type _eLog, const char* _szFormat, ...) 
	{
		// Generic information with Message
		String& sBuffer = GetBufferAvailable();

		va_list pArg;
		va_start(pArg, _szFormat);
		BuildBuffer(_szFormat, pArg, sBuffer);
		va_end(pArg);

		printf_s(sBuffer.GetBuffer());
		SIGNAL_SEND(LogChanged, _eLog, sBuffer.GetBuffer());
	}

	//!	@brief		Print a log
	//!	@date		2015-04-04
	void LogManager::Print(ELog::Type _eLog, const char* _szFilePath, uint32 _uiLine, const char* _szFormat, ...) 
	{
		// Type
		String& sBuffer = GetBufferAvailable();
		if(_eLog != ELog::Normal)
		{
			sBuffer.Append("[", 1);
			sBuffer.Append(ELog::Name[_eLog]);
			sBuffer.Append("] ", 2);
		}

		// Generic information with Message
		va_list pArg;
		va_start(pArg, _szFormat);
		BuildBuffer(_szFilePath, _uiLine, _szFormat, pArg, sBuffer);
		va_end(pArg);

		OutputDebugString(sBuffer.GetBuffer());
	}

	//!	@brief		Assert
	//!	@date		2015-04-04
	void LogManager::Assert(bool _bCondition, const char* _szFilePath, uint32 _uiLine, const char* _szFormat, ...)
	{			
		if(!_bCondition)
		{
			String& sBuffer = GetBufferAvailable();

			// Generic information with Message
			va_list pArg;
			va_start(pArg, _szFormat);
			BuildBuffer(_szFilePath, _uiLine, _szFormat, pArg, sBuffer);
			va_end(pArg);

			// Display a MessageBox
			int iMessageBox = MessageBox(NULL, sBuffer.GetBuffer(), "Assert", MB_CANCELTRYCONTINUE | MB_ICONERROR);
			switch (iMessageBox)
			{
				case IDCANCEL:
				{
					exit(-1);
					break;
				}

				case IDTRYAGAIN:
				{
					__debugbreak();
					break;
				}
			}
		}
	}

	//!	@brief		GetBufferAvailable
	//!	@date		2016-05-22
	String& LogManager::GetBufferAvailable()
	{
		uint32 uiThreadId = Thread::GetId();
		for(uint32 uiIndex = 0; uiIndex < LOG_MAX_THREAD; ++uiIndex)
		{
			if(s_aThread[uiIndex] == uiThreadId)
			{
				return s_aBuffer[uiIndex].Set("");
			}
		}

		// Create a new Entry for this thread
		uint32 uiIndex = InterlockedIncrementAcquire(&s_uiIndex);
		ASSERT(uiIndex < LOG_MAX_THREAD, "[LogManager] LOG_MAX_THREAD limit");

		s_aThread[uiIndex] = uiThreadId;
		String& sBuffer = s_aBuffer[uiIndex];
		sBuffer.Resize(LOG_MAX_LENGTH);
		return sBuffer.Set("");
	}
}