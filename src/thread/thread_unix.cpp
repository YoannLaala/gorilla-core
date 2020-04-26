/******************************************************************************
**  Includes
******************************************************************************/
#include "thread.hpp"

#include "../log/log.hpp"
#include <pthread.h>
#include <string.h>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    struct Descriptor
    {
        const char* Name;
        Thread::ThreadFunction Function;
        void* Data;
    };

    void* thread_entry(void *data)
    {
        Descriptor *descriptor = (Descriptor*)data;

#if defined(PLATFORM_LINUX)
        pthread_t thread_id = pthread_self();
        pthread_setname_np(thread_id, descriptor->Name);
#else
        pthread_setname_np(descriptor->Name);
#endif
        descriptor->Function(descriptor->Data);
        delete descriptor;

        return nullptr;
    }

    //  brief      constructor
    Thread::Thread(const char *name, ThreadFunction function, void *data /*= NULL*/, Priority priority /*= EPriority::Normal*/)
    {
        pthread_attr_t thread_attributes;
        pthread_attr_init(&thread_attributes);

        static int32_t priorities[] = {0, 15, 30};
        sched_param scheduler_parameter;
        memset(&scheduler_parameter, 0, sizeof(sched_param));
        scheduler_parameter.sched_priority = priorities[(uint32_t)priority];
        pthread_attr_setschedparam(&thread_attributes, &scheduler_parameter);

        // prepare descriptor to launch thread
        Descriptor* descriptor = new Descriptor();
        descriptor->Name = name;
        descriptor->Function = function;
        descriptor->Data = data;

        int32_t error = pthread_create((pthread_t*)&m_handle, &thread_attributes, &thread_entry, descriptor);
        LOG_ASSERT(error == 0, "failed to create thread %", name);
        pthread_attr_destroy(&thread_attributes);
    }

    //  brief      destructor
    Thread::~Thread()
    {
        join();
    }

    //!	@brief      get_id
    uint64_t Thread::get_id()
    {
        return (uint64_t)m_handle;
    }

    //!	@brief      sleep
    void Thread::join()
    {
        if (!m_handle)
            return;

        uint32_t error = pthread_join((pthread_t)m_handle, nullptr);
        LOG_ASSERT(error == 0, "failed to join thread %u", get_id());
        m_handle = nullptr;
    }

    //!	@brief      get_current_id
    uint64_t Thread::get_current_id()
    {
        return (uint64_t)pthread_self();
    }

    //!	@brief      sleep_ns
    void Thread::sleep_ns(uint64_t time)
    {
        struct timespec time_spec;
        time_spec.tv_sec = (uint64_t)(time / 1e9);
        time_spec.tv_nsec = time - time_spec.tv_sec;
        nanosleep(&time_spec, NULL);
    }
}
