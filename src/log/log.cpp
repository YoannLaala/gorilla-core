/******************************************************************************
**   Includes
******************************************************************************/
#include "log.hpp"
#include "../thread/thread.hpp"
#include <cstdio>
#include <cstdarg>
#include <cstdlib>

/******************************************************************************
**  Defines
******************************************************************************/
#if defined(PLATFORM_WINDOWS)
    #define VSPRINTF(_destination_, _format_, _arguments_)     ::vsprintf_s(_destination_, _format_, _arguments_)
#else
    #define VSPRINTF(_destination_, _format_, _arguments_)     ::vsprintf(_destination_, _format_, _arguments_)
#endif

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      log
    //! @details    log a message with decoration in proper stream
    void log(LogLevel level, const char *filepath, uint32_t line, ...)
    {
        static const char* level_names[] = {
            "Message",
            "Warning",
            "Error",
            "Assert"
        };
        static THREAD_LOCAL char buffer[256];

        // format user log
        va_list arguments;
        va_start(arguments, line);
        const char* format = va_arg(arguments, const char*);
        int32_t length = VSPRINTF(buffer, format, arguments);
        va_end(arguments);

        // early exist
        if (length >= 256)
        {
            log(LogLevel::WARNING, filepath, line, "log skipped because too long");
            return;
        }

        // display log
        std::FILE *stream = level == LogLevel::MESSAGE ? stdout : stderr;
        fprintf(stream, "[%s] %s:%u: %s\n", level_names[(uint32_t)level], filepath, line, buffer);

        // stop execution when asserting
        if (level == LogLevel::ASSERT)
            abort();
    }
}
