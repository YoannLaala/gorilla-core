#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "condition_variable.hpp"

#include <cstdint>

/******************************************************************************
**  Defines
******************************************************************************/
#if defined(PLATFORM_WINDOWS)
    #define THREAD_LOCAL _declspec(thread)
#else
    #define THREAD_LOCAL thread_local
#endif

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class Thread
    {
    public:
        enum class Priority
        {
            LOW = 0,
            NORMAL,
            HIGH,
        };
        typedef void (*ThreadFunction)(void*);

        Thread(const char *name, ThreadFunction function, void *data = nullptr, Priority priority = Priority::NORMAL);
        ~Thread();

        uint64_t get_id();
        void join();

        static uint64_t get_current_id();
        static void sleep_ns(uint64_t time_ns);

    private:
        void *m_handle;
    };
}
