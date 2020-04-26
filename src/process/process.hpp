#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../container/vector.hpp"
#include "../string/string.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class Process
    {
    public:
        Process(const char *executable_path);
        ~Process();

        int32_t execute(Vector<String> *arguments = nullptr);

        template <typename ...ARGUMENTS>
        int32_t execute(const char *argument, ARGUMENTS... arguments);

        inline const String& get_stdout() const;
        inline const String& get_stderr() const;

    private:
        template <typename ...ARGUMENTS>
        int32_t execute(Vector<String> *unpacked_arguments, const char *argument, ARGUMENTS... arguments);

    private:
        String m_executable_path;
        String m_stdout;
        String m_stderr;
    };

    template <typename ...ARGUMENTS>
    int32_t Process::execute(const char *argument, ARGUMENTS... arguments)
    {
        Vector<String> unpacked_arguments;
        return execute(&unpacked_arguments, argument, arguments...);
    }

    template <typename ...ARGUMENTS>
    int32_t Process::execute(Vector<String> *unpacked_arguments, const char *argument, ARGUMENTS... arguments)
    {
        unpacked_arguments->add(argument);
        return execute(unpacked_arguments, arguments...);
    }

    const String& Process::get_stdout() const
    {
        return m_stdout;
    }

    const String& Process::get_stderr() const
    {
        return m_stderr;
    }
}
