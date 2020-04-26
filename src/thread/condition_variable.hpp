#pragma once

/******************************************************************************
**  Forward Declaration
******************************************************************************/
namespace Gorilla
{
    class Mutex;
}

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class ConditionVariable
    {
    public:
        ConditionVariable();
        ~ConditionVariable();

        typedef bool (*Predicate)(void* data);
        void wait(Predicate predicate, void *data);
        void signal();

    private:
        void *m_handle;
    };
}
