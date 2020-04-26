#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "time.hpp"
#include "../log/log.hpp"

/******************************************************************************
**  Defines
******************************************************************************/
#define PROFILE_START(_name_)   int64_t profile_##_name_ = Gorilla::Time::get_performance_counter()
#define PROFILE_STOP(_name_)    LOG_MESSAGE("[PROFILE] %s %.2fms", #_name_, Gorilla::Time::get_elapsed_time(profile_##_name_, Gorilla::Time::get_performance_counter()))