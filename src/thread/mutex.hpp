#pragma once

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class Mutex
    {
        friend class ConditionVariable;
    public:
        Mutex();
        ~Mutex();

        void lock();
        void unlock();

    private:
        void *m_handle;
    };
}
