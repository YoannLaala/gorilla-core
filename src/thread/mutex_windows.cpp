/******************************************************************************
**  Includes
******************************************************************************/
#include "mutex.hpp"
#include <Windows.h>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      constructor
    Mutex::Mutex()
    {
        CRITICAL_SECTION *critical_section = new CRITICAL_SECTION();
        InitializeCriticalSection(critical_section);
        m_handle = critical_section;
    }

    //! @brief      destructor
    Mutex::~Mutex()
    {
        DeleteCriticalSection((CRITICAL_SECTION*)m_handle);
        delete (CRITICAL_SECTION*)m_handle;
    }

    //! @brief      lock
    void Mutex::lock()
    {
        EnterCriticalSection((CRITICAL_SECTION*)m_handle);
    }

    //! @brief      unlock
    void Mutex::unlock()
    {
        LeaveCriticalSection((CRITICAL_SECTION*)m_handle);
    }
}
