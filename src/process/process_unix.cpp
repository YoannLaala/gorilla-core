/******************************************************************************
**  Includes
******************************************************************************/
#include "process.hpp"

#include "../file_system/file_system.hpp"
#include "../file_system/path.hpp"
#include "../stream/file_reader.hpp"
#include "../thread/thread.hpp"
#include <fcntl.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

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

    //! @brief      read_file
    bool read_file(FileReader *file_reader, const char *filepath, String *value_out)
    {
        if (!file_reader->open(filepath))
            return false;

        const uint32_t file_size = file_reader->get_size();
        value_out->resize(file_size);
        if (file_reader->read((char*)value_out->get_buffer(), file_size) != file_size)
            return false;

        return true;
    }

    //! @brief      execute
    int32_t Process::execute(Vector<String> *arguments /*= nullptr*/)
    {
        int32_t status = -1;
        char **argument_array = nullptr;
        m_stdout.set("");
        m_stderr.set("");

        String temp, directory, stdout_filepath, stderr_filepath;
        FileSystem::get_directory_path(FileSystem::Directory::TEMPORARY, &temp);
        FileSystem::combine(&directory, Time::get_performance_counter());
        if (!FileSystem::create_directory(directory.get_buffer())
            return -1;

        posix_spawn_file_actions_t process_file_action;
        int32_t error = posix_spawn_file_actions_init(&process_file_action);
        if (error)
            goto finish;

        Path::combine(&stdout_filepath, directory, "stdout");
        error = posix_spawn_file_actions_addopen(&process_file_action, STDOUT_FILENO, stdout_filepath.get_buffer(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (error)
            goto finish;

        Path::combine(&stderr_filepath, directory, "stderr");
        error = posix_spawn_file_actions_addopen(&process_file_action, 2, stderr_filepath.get_buffer(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (error)
            goto finish;

        if (arguments)
        {
            const uint32_t argument_count = arguments->get_size();
            argument_array = new char*[argument_count + 2];

            argument_array[0] = (char*)m_executable_path.get_buffer();
            for (uint32_t i = 0; i < argument_count; ++i)
                argument_array[i+1] = (char*)(*arguments)[i].get_buffer();

            argument_array[argument_count+1] = nullptr;
        }
        else
            argument_array = new char*[2] { (char*)m_executable_path.get_buffer(), nullptr};

        pid_t process_id;
        error = posix_spawn(&process_id, m_executable_path.get_buffer(), &process_file_action, nullptr, argument_array, nullptr);
        if (error)
            goto finish;

        error = waitpid(process_id, &status, WNOHANG);
        while (error == 0)
        {
            Thread::sleep_ns(1*(uint64_t)1e6);
            error = waitpid(process_id, &status, WNOHANG);
        }
        if (error == -1)
            goto finish;

    finish:
        if (argument_array)
            delete[] argument_array;

        if (FileSystem::check_directory(directory.get_buffer()))
        {
            FileReader file_reader;
            read_file(&file_reader, stdout_filepath.get_buffer(), &m_stdout);
            read_file(&file_reader, stderr_filepath.get_buffer(), &m_stderr);
            FileSystem::delete_directory(directory.get_buffer());
        }

        return status;
    }
}
