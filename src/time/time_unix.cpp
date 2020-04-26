/******************************************************************************
**  Includes
******************************************************************************/
#include "time.hpp"

#include <time.h>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      GetPerformanceCounter
    int64_t Time::get_performance_counter()
    {
        timespec time_spec;
        clock_gettime(CLOCK_MONOTONIC, &time_spec);
        return time_spec.tv_sec * (uint64_t)1e9 + time_spec.tv_nsec;
    }

    //! @brief      Get the time between 2 PerformanceCounter in ms
    float Time::get_elapsed_time(int64_t start, int64_t end, Unit unit /*= Unit::MILLISECONDS*/)
    {
        static uint64_t factors[] = { (uint64_t)1e9, (uint64_t)1e6, (uint64_t)1e3, 1 };
        return (float)((end - start) / factors[(uint32_t)unit]);
    }
}
