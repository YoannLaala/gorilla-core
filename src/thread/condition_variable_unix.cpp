/******************************************************************************
**  Includes
******************************************************************************/
#include "condition_variable.hpp"

#include "../log/log.hpp"
#include <pthread.h>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    struct Handle
    {
        pthread_mutex_t mutex;
        pthread_cond_t condition_variable;
    };

    //!	@brief      Constructor
    ConditionVariable::ConditionVariable()
    {
        Handle *handle = new Handle();
        pthread_mutex_init(&handle->mutex, nullptr);
        pthread_cond_init(&handle->condition_variable, nullptr);
        m_handle = handle;
    }

    //!	@brief      Constructor
    ConditionVariable::~ConditionVariable()
    {
        Handle *handle = (Handle*)m_handle;
        pthread_cond_destroy(&handle->condition_variable);
        pthread_mutex_destroy(&handle->mutex);
        delete handle;
    }

    //!	@brief      wait
    void ConditionVariable::wait(Predicate predicate, void *data)
    {
        Handle *handle = (Handle*)m_handle;
        pthread_mutex_lock(&handle->mutex);
        while (!predicate(data))
        {
            int32_t error = pthread_cond_wait(&handle->condition_variable, &handle->mutex);
            LOG_ASSERT(error == 0, "failed to wait a condition variable");
        }
        pthread_mutex_unlock(&handle->mutex);
    }

    //!	@brief      signal
    void ConditionVariable::signal()
    {
        Handle *handle = (Handle*)m_handle;
        pthread_mutex_lock(&handle->mutex);
        int32_t error = pthread_cond_signal(&handle->condition_variable);
        LOG_ASSERT(error == 0, "failed to signal a condition variable");
        pthread_mutex_unlock(&handle->mutex);
    }
}
