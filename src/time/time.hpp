#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include <cstdint>

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class Time
    {
    public:
        enum class Unit
        {
            SECONDS = 0,
            MILLISECONDS,
            MICROSECONDS,
            NANOSECONDS
        };

        static int64_t        get_performance_counter   ();
        static float      get_elapsed_time          (int64_t star, int64_t end, Unit unit = Unit::MILLISECONDS);
    };
}
