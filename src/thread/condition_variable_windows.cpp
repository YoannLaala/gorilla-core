/******************************************************************************
**  Includes
******************************************************************************/
#include "condition_variable.hpp"

#include "../log/log.hpp"
#include <Windows.h>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    struct Handle
    {
        CRITICAL_SECTION critical_section;
        CONDITION_VARIABLE condition_variable;
    };

    //! @brief      constructor
    ConditionVariable::ConditionVariable()
    {
        Handle *handle = new Handle();
        InitializeCriticalSection(&handle->critical_section);
        InitializeConditionVariable(&handle->condition_variable);
        m_handle = handle;
    }

    //! @brief      constructor
    ConditionVariable::~ConditionVariable()
    {
        Handle *handle = (Handle*)m_handle;
        DeleteCriticalSection(&handle->critical_section);
        delete handle;
    }

    //! @brief      wait
    void ConditionVariable::wait(Predicate predicate, void *data)
    {
        Handle *handle = (Handle*)m_handle;
        EnterCriticalSection(&handle->critical_section);
        while (!predicate(data))
        {
            BOOL result = SleepConditionVariableCS(&handle->condition_variable, &handle->critical_section, INFINITE);
            LOG_ASSERT(result == TRUE, "failed to wait a condition variable");
        }
        LeaveCriticalSection(&handle->critical_section);
    }

    //! @brief      signal
    void ConditionVariable::signal()
    {
        Handle *handle = (Handle*)m_handle;
        EnterCriticalSection(&handle->critical_section);
        WakeAllConditionVariable(&handle->condition_variable);
        LeaveCriticalSection(&handle->critical_section);
    }
}