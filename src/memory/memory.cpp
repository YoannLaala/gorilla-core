/******************************************************************************
**  Includes
******************************************************************************/
#include "memory.hpp"
#include "../log/log.hpp"
#include <stdlib.h>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla { namespace Memory
{
    //! @brief      allocate
    //! @details    Allocate some memory using malloc
    void* allocate(uint64_t size)
    {
        void *address = malloc((size_t)size);
        LOG_ASSERT(address, "malloc failed");
        return address;
    }

    //! @brief      deallocate
    //! @details    Deallocate the memory using free
    void deallocate(void *address)
    {
        free(address);
    }
}}
