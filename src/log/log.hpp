#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include <cstdint>

/******************************************************************************
**  Defines
******************************************************************************/
static constexpr const char* FORMAT_FILEPATH_EXT(const char *filepath, const char *last_slash)
{
    return
        *filepath == '\0' ? last_slash :
        *filepath == '/'  ? FORMAT_FILEPATH_EXT(filepath + 1, filepath + 1) :
                            FORMAT_FILEPATH_EXT(filepath + 1, last_slash);
}

#define FORMAT_FILEPATH(_file_) FORMAT_FILEPATH_EXT(_file_, _file_)
#define LOG_MESSAGE(...)                Gorilla::log(Gorilla::LogLevel::MESSAGE, FORMAT_FILEPATH(__FILE__), __LINE__, __VA_ARGS__)
#define LOG_WARNING(...)                Gorilla::log(Gorilla::LogLevel::WARNING, FORMAT_FILEPATH(__FILE__), __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)                  Gorilla::log(Gorilla::LogLevel::ERROR, FORMAT_FILEPATH(__FILE__), __LINE__, __VA_ARGS__)
#define LOG_ASSERT(_condition_, ...)    if(!(_condition_)) { Gorilla::log(Gorilla::LogLevel::ASSERT, FORMAT_FILEPATH(__FILE__), __LINE__, __VA_ARGS__); }

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    enum class LogLevel
    {
        MESSAGE = 0,
        WARNING,
        ERROR,
        ASSERT,
    };

    void log(LogLevel LogLevel, const char *filepath, uint32_t line, ...);
}
