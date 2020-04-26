/******************************************************************************
**  Includes
******************************************************************************/
#include "thread.hpp"

#include "../log/log.hpp"
#include <Windows.h>

/******************************************************************************
**  Defines
******************************************************************************/
#define MS_VC_EXCEPTION 0x406D1388


/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    struct THREADNAME_INFO
    {
        DWORD dwType;     // Must be 0x1000.
        LPCSTR szName;    // Pointer to name (in user addr space).
        DWORD dwThreadID; // Thread ID (-1=caller thread).
        DWORD dwFlags;    // Reserved for future use, must be zero.
    };

    //  brief      Create
    void set_name_internal(uint32_t thread_id, const char *name)
    {
        THREADNAME_INFO thread_name_info;
        thread_name_info.dwType = 0x1000;
        thread_name_info.szName = name;
        thread_name_info.dwThreadID = thread_id;
        thread_name_info.dwFlags = 0;
        __try
        {
            RaiseException(MS_VC_EXCEPTION, 0, sizeof(THREADNAME_INFO) / sizeof(ULONG_PTR), (ULONG_PTR*)&thread_name_info);
        }
        __except(EXCEPTION_CONTINUE_EXECUTION) { }
    }

    //  brief      constructor
    Thread::Thread(const char *name, ThreadFunction function, void *data /*= NULL*/, Priority priority /*= EPriority::Normal*/)
    {
        // create the thread with a default stack size of 1MB
        uint32_t thread_id;
        m_handle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)function, data, 0, (LPDWORD)&thread_id);
        LOG_ASSERT(m_handle, "failed to create thread %s", name);

        const int priorities[] = { THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_HIGHEST };
        BOOL result = ::SetThreadPriority((HANDLE)m_handle, priorities[(uint32_t)priority]);
        LOG_ASSERT(result == TRUE, "failed to set priority on thread %s", name);

        set_name_internal(thread_id, name);
    }

    //  brief      destructor
    Thread::~Thread()
    {
        join();
    }

    //!	@brief      get_id
    uint64_t Thread::get_id()
    {
        return ::GetThreadId((HANDLE)m_handle);
    }

    //!	@brief      sleep
    void Thread::join()
    {
        uint32_t error = WaitForSingleObject((HANDLE)m_handle, INFINITE);
        LOG_ASSERT(error == 0, "failed to join thread %u", get_id());
    }

    //!	@brief      get_current_id
    uint64_t Thread::get_current_id()
    {
        return (uint64_t)::GetCurrentThreadId();
    }

    //!	@brief      sleep_ns
    void Thread::sleep_ns(uint64_t time)
    {
        HANDLE timer = ::CreateWaitableTimer(NULL, TRUE, NULL);
        LOG_ASSERT(timer, "failed to create timer");

        LARGE_INTEGER large_integer;
        large_integer.QuadPart = -(int64_t)time / 100; // 100 nanoseconds intervals

        BOOL result_prepare = SetWaitableTimer(timer, &large_integer, 0, NULL, NULL, FALSE);
        LOG_ASSERT(result_prepare == TRUE, "failed to prepare timer");

        uint32_t result_wait = WaitForSingleObject(timer, INFINITE);
        LOG_ASSERT(result_wait == 0, "failed to wait timer");

        CloseHandle(timer);
    }
}
