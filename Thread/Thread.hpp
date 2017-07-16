#ifndef _CORE_THREAD_HPP_
#define _CORE_THREAD_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/TypeDef.hpp>
#include <Core/String/String.hpp>
#include <Core/Container/Vector.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Thread
	{
	public:
		struct EPriority
		{
			enum Type : uint8
			{
				Low = 0,
				Normal,
				High,

				Count,
			};
		};

		struct Header
		{
			String Name;
			uint32 Identifier;
			EPriority::Type Priority;
			uint8 Index;
		};

		#pragma pack(push,8)
		typedef struct tagTHREADNAME_INFO
		{
			uint32 uiType; // Must be 0x1000.
			const char* szName; // Pointer to name (in user addr space).
			uint32 dwThreadID; // Thread ID (-1=caller thread).
			uint32 dwFlags; // Reserved for future use, must be zero.
		} THREADNAME_INFO;
		#pragma pack(pop)

		typedef void (*fnThreadFunction)(void*);
		static uint32 Create(const char* _szName, fnThreadFunction _pFunction, void* _pData = nullptr, EPriority::Type _ePriority = EPriority::Normal);

		static const String& GetName();
		static uint32 GetId();
		static EPriority::Type GetPriority();
		static uint8 GetIndex();

		static bool IsMainThread();

		static void Sleep(uint32 _uiTime);

	private:
		static const Header* GetHeader();

	private:
		static Vector<Header> s_vHeader;
	};
}

#endif