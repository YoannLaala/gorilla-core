/******************************************************************************
**  Includes
******************************************************************************/
#include "time.hpp"

#include <Windows.h>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      get_performance_counter
    int64_t Time::get_performance_counter()
    {
        int64_t performance_counter = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&performance_counter);
        return performance_counter;
    }

    //! @brief      get_elapsed_time
    float Time::get_elapsed_time(int64_t start, int64_t end, Unit unit /*= Unit::MILLISECONDS*/)
    {
        static uint64_t factors[] = { 1, (uint64_t)1e3, (uint64_t)1e6, (uint64_t)1e9 };

        static int64_t frequency;
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

        float time = (float)((end - start) * factors[(uint32_t)unit] / (double)frequency);

        return time;
    }
}
