/******************************************************************************
**  Includes
******************************************************************************/
#include "process.hpp"
#include <Windows.h>

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    //! @brief      constructor
    Process::Process(const char *executable_path)
        : m_executable_path(executable_path)
    {
        // Noyhing to do
    }

    //! @brief      destructor
    Process::~Process()
    {
        // Nothing to do
    }

    //! @brief      read_handle
    void read_handle(HANDLE handle, String *value_out)
    {
        DWORD available_bytes = 0;
        PeekNamedPipe(handle, NULL, NULL, NULL, &available_bytes, NULL);
        if(available_bytes != (uint32_t)-1 && available_bytes > 0)
        {
            char buffer[512];
            do
            {
                DWORD read_bytes = 0;
                uint32_t bytes_to_read = available_bytes < sizeof(buffer)-1 ? available_bytes : sizeof(buffer)-1;
                ReadFile(handle, buffer, bytes_to_read, &read_bytes, NULL);
                value_out->append(buffer, read_bytes);
                available_bytes -= read_bytes;
            } while (available_bytes);
        }
    }

    //! @brief      execute
    int32_t Process::execute(Vector<String> *arguments /*= nullptr*/)
    {
        String command_line(m_executable_path);
        m_stdout.set("");
        m_stderr.set("");

        if (arguments)
        {
            const uint32_t argument_count = arguments->get_size();
            for (uint32_t i = 0; i < argument_count; ++i)
                command_line.append(' ').append((*arguments)[i]);
        }

        SECURITY_ATTRIBUTES security_attributes;
        ZeroMemory(&security_attributes, sizeof(SECURITY_ATTRIBUTES));
        security_attributes.nLength= sizeof(SECURITY_ATTRIBUTES);
        security_attributes.lpSecurityDescriptor = NULL;
        security_attributes.bInheritHandle = TRUE;

        STARTUPINFO startup_info;
        ZeroMemory(&startup_info, sizeof(STARTUPINFO));
        startup_info.cb = sizeof(STARTUPINFO);
        startup_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        startup_info.wShowWindow = SW_HIDE;

        // create handles to read stdout stderr
        HANDLE handle_stdout, handle_stderr;
        CreatePipe(&handle_stdout, &startup_info.hStdOutput, &security_attributes, 0);
        CreatePipe(&handle_stderr, &startup_info.hStdError, &security_attributes, 0);

        // create the process
        PROCESS_INFORMATION process_information;
        bool result = CreateProcess(
                nullptr                             ,//__in_opt     LPCTSTR lpApplicationName,
                (LPSTR)command_line.get_buffer(),   //__inout_opt  LPWSTR lpCommandLine,
                NULL,                               //__in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
                NULL,                               //__in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
                TRUE,                               //__in         BOOL bInheritHandles,
                0,                                  //__in         DWORD dwCreationFlags,
                NULL,                               //__in_opt     LPVOID lpEnvironment,
                NULL,                               //__in_opt     LPCTSTR lpCurrentDirectory,
                &startup_info,                      //__in         LPSTARTUPINFO lpStartupInfo,
                &process_information                //__out        LPPROCESS_INFORMATION lpProcessInformation
            ) != 0;

        // successfully created the process, wait to finish
        int32_t exit_code = -1;
        if(result)
        {
            uint32_t wait_result = WAIT_TIMEOUT;
            while (wait_result == WAIT_TIMEOUT)
            {
                wait_result = WaitForSingleObject(process_information.hProcess, 1);
                read_handle(handle_stdout, &m_stdout);
                read_handle(handle_stderr, &m_stderr);
            }
            GetExitCodeProcess(process_information.hProcess, (DWORD*)&exit_code);
        }
        else
        {
            read_handle(handle_stderr, &m_stderr);
        }

        // Close all pipe
        CloseHandle(startup_info.hStdInput);
        CloseHandle(startup_info.hStdOutput);
        CloseHandle(startup_info.hStdError);
        CloseHandle(process_information.hProcess);
        CloseHandle(process_information.hThread);
        CloseHandle(handle_stderr);
        CloseHandle(handle_stdout);

        return exit_code;
    }
}
