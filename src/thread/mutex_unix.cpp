/******************************************************************************
**	Includes
******************************************************************************/
#include "mutex.hpp"
#include <pthread.h>

/******************************************************************************
** Class Definition
******************************************************************************/
namespace Gorilla
{
   //!    @brief     Constructor
   Mutex::Mutex()
   {
      pthread_mutex_t* handle = new pthread_mutex_t();
      pthread_mutex_init(handle, nullptr);
      m_handle = handle;
   }

   //!   @brief      Destructor
   Mutex::~Mutex()
   {
      pthread_mutex_t* handle = static_cast<pthread_mutex_t*>(m_handle);
      pthread_mutex_destroy(handle);
      delete handle;
   }

   //!   @brief      lock
   void Mutex::lock()
   {
      pthread_mutex_t* handle = static_cast<pthread_mutex_t*>(m_handle);
      pthread_mutex_lock(handle);
   }

   //!   @brief      unlock
   void Mutex::unlock()
   {
      pthread_mutex_t* handle = static_cast<pthread_mutex_t*>(m_handle);
      pthread_mutex_unlock(handle);
   }
}
