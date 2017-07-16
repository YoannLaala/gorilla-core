/******************************************************************************
**	Includes
******************************************************************************/
#include "Thread.hpp"
#include <Core/String/String.hpp>
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	Vector<Thread::Header> Thread::s_vHeader;

	//!	@brief		Create
	//!	@date		2015-05-15
	uint32 Thread::Create(const char* _szName, fnThreadFunction _pFunction, void* _pData /*= NULL*/, EPriority::Type _ePriority /*= EPriority::Normal*/)
	{
		// Create the thread with a default stack size of 1MB
		uint32 uiThreadId;
		HANDLE hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_pFunction, _pData, 0, (LPDWORD)&uiThreadId);

		// Handle Priority
		static const int aPriority[EPriority::Count] = 
		{
			THREAD_PRIORITY_LOWEST,
			THREAD_PRIORITY_NORMAL,
			THREAD_PRIORITY_HIGHEST,
		};
		SetThreadPriority(hThread, aPriority[_ePriority]);

		// Name of the Thread
		const DWORD MS_VC_EXCEPTION=0x406D1388;
		THREADNAME_INFO kInfo;
		kInfo.uiType = 0x1000;
		kInfo.szName = _szName;
		kInfo.dwFlags = 0;
		__try
		{
			RaiseException( MS_VC_EXCEPTION, 0, sizeof(THREADNAME_INFO)/sizeof(ULONG_PTR), (ULONG_PTR*)&kInfo);
		}
		__except(EXCEPTION_EXECUTE_HANDLER) { }

		// Header
		Header& kHeader = s_vHeader.Add();
		kHeader.Name = _szName;
		kHeader.Identifier = uiThreadId;
		kHeader.Index = (uint8)s_vHeader.GetSize() - 1;
		kHeader.Priority = _ePriority;

		return uiThreadId;
	}

	//!	@brief		GetName
	//!	@date		2015-05-15
	const String& Thread::GetName()
	{
		return GetHeader()->Name;
	}

	//!	@brief		GetId
	//!	@date		2015-05-15
	uint32 Thread::GetId()
	{
		return ::GetCurrentThreadId();
	}

	//!	@brief		GetPriority
	//!	@date		2015-05-15
	Thread::EPriority::Type Thread::GetPriority()
	{
		return GetHeader()->Priority;
	}

	//!	@brief		GetIndex
	//!	@date		2015-05-15
	uint8 Thread::GetIndex()
	{
		return GetHeader()->Index;
	}

	//!	@brief		IsMainThread
	//!	@date		2015-05-15
	bool Thread::IsMainThread()
	{
		static uint32 uiId = GetCurrentThreadId();
		return uiId == GetCurrentThreadId();
	}

	//!	@brief		Sleep
	//!	@date		2015-05-15
	void Thread::Sleep(uint32 _uiTime)
	{
		::Sleep(_uiTime);
	}

	//!	@brief		GetHeader
	//!	@date		2015-05-15
	const Thread::Header* Thread::GetHeader()
	{
		uint32 uiThreadId = GetId();
		const uint32 uiThreadCount = s_vHeader.GetSize();
		for(uint32 uiThread = 0; uiThread < uiThreadCount; ++uiThread)
		{
			const Header& kHeader = s_vHeader[uiThread];
			if(kHeader.Identifier == uiThreadId)
			{
				return &kHeader;
			}
		}

		return NULL;
	}
}